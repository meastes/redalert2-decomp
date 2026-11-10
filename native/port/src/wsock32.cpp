// WSOCK32 (Winsock 1.1) on BSD sockets, under the ra2_ names native_units.py
// gives the clashing calls. Translates the Winsock sockaddr_in (no sin_len,
// 16-bit family) and HOSTENT (16-bit type/length), and emulates WSAAsyncSelect
// with a poll thread that posts the window message as Winsock does.
//
// IPX (Scout1209 r7): RA2 1.006's LAN game speaks only IPX (IPXInterfaceClass
// 0x0076A3A0: socket(AF_IPX 6, SOCK_DGRAM, NSPROTO_IPX 1000)). Hosts have no IPX
// stack, so an IPX datagram socket is carried as a UDP socket: each running game
// claims one UDP port of a fixed range (ipx_base_port .. +ipx_port_count) and its
// IPX node address is its IPv4 address and that port (6 bytes, network number 0).
// A send to node FF:FF:FF:FF:FF:FF goes to the UDP broadcast address on every port
// of the range (other machines and other games on this one); any other node is a
// unicast to the address and port it names. A datagram received from ip:port is
// reported as from node ip:port. Only standard sockets calls (and getifaddrs for
// the local address), so other ports can reuse it.
#include "ra2port/win32_port.hpp"

#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <ifaddrs.h>
#include <net/if.h>
#include <map>
#include <mutex>
#include <netdb.h>
#include <netinet/in.h>
#include <poll.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <thread>
#include <unistd.h>
#include <vector>

using namespace ra2port;
typedef unsigned int SOCKET;

namespace {
thread_local int wsa_error = 0;
int set_error()
{
    switch (errno) {
    case EWOULDBLOCK: wsa_error = 10035; break;
    case EADDRINUSE: wsa_error = 10048; break;
    case ECONNRESET: wsa_error = 10054; break;
    case EMSGSIZE: wsa_error = 10040; break;
    default: wsa_error = 10000 + errno; break;
    }
    return -1;
}

// Winsock sockaddr_in: short family, ushort port, ulong addr, char zero[8].
struct WinSockAddrIn { int16_t family; uint16_t port; uint32_t addr; char zero[8]; };

void to_bsd(const void *win, struct sockaddr_in *bsd)
{
    const WinSockAddrIn *w = (const WinSockAddrIn *)win;
    memset(bsd, 0, sizeof *bsd);
    bsd->sin_len = sizeof *bsd;
    bsd->sin_family = (sa_family_t)w->family;
    bsd->sin_port = w->port;
    bsd->sin_addr.s_addr = w->addr;
}
void to_win(const struct sockaddr_in *bsd, void *win)
{
    WinSockAddrIn *w = (WinSockAddrIn *)win;
    memset(w, 0, sizeof *w);
    w->family = bsd->sin_family;
    w->port = bsd->sin_port;
    w->addr = bsd->sin_addr.s_addr;
}

// ---- IPX over UDP ------------------------------------------------------------
// Winsock SOCKADDR_IPX (14 bytes): short family (6), netnum[4], nodenum[6],
// socket (big-endian); IPX_ADDRESS_DATA (24 bytes) for getsockopt(IPX_ADDRESS).
#pragma pack(push, 1)
struct WinSockAddrIpx { int16_t family; uint8_t netnum[4]; uint8_t nodenum[6]; uint16_t socket; };
#pragma pack(pop)
struct WinIpxAddressData { int32_t adapternum; uint8_t netnum[4]; uint8_t nodenum[6]; uint8_t wan, status; int32_t maxpkt; uint32_t linkspeed; };
const int AF_IPX_WIN = 6, NSPROTO_IPX_WIN = 1000;
const int IPX_ADDRESS_OPT = 0x4007, IPX_MAX_ADAPTER_NUM_OPT = 0x400D;
const uint16_t ipx_base_port = 26300;
const int ipx_port_count = 8;
struct IpxSocket { uint16_t number = 0; };
std::mutex ipx_lock;
std::map<SOCKET, IpxSocket> ipx_sockets;
uint16_t ipx_port = 0;          // this game's UDP port (0: not claimed yet)
int ipx_reserve = -1;           // holds the claimed port until the game binds its socket

// RA2_NATIVE_NET_LOG=1 (diagnostics): every IPX datagram sent and received on stderr.
bool net_log()
{
    static int on = -1;
    if (on < 0) on = getenv("RA2_NATIVE_NET_LOG") ? 1 : 0;
    return on != 0;
}
void net_trace(char const *what, uint32_t address, uint16_t port, int length, const char *data)
{
    if (!net_log()) return;
    struct in_addr a;
    a.s_addr = address;
    unsigned magic = length >= 6 ? (unsigned)(uint8_t)data[4] | (unsigned)(uint8_t)data[5] << 8 : 0;
    // After the 4-byte CRC: the CommHeader's magic, code (0 ack, 1 no ack, 2 ack reply) and packet id.
    int code = length >= 7 ? (uint8_t)data[6] : -1;
    unsigned id = 0;
    if (length >= 12) memcpy(&id, data + 8, 4);
    fprintf(stderr, "net: %s %s:%u %d bytes magic %04x code %d id %u\n", what, inet_ntoa(a), (unsigned)port, length, magic, code, id);
}

bool is_ipx(SOCKET s)
{
    std::lock_guard<std::mutex> g(ipx_lock);
    return ipx_sockets.count(s) != 0;
}
// The first up, non-loopback IPv4 address (the one other machines reach), else loopback.
uint32_t ipx_local_address()
{
    uint32_t found = htonl(INADDR_LOOPBACK);
    struct ifaddrs *list = nullptr;
    if (getifaddrs(&list) != 0) return found;
    for (struct ifaddrs *i = list; i; i = i->ifa_next) {
        if (!i->ifa_addr || i->ifa_addr->sa_family != AF_INET) continue;
        if (!(i->ifa_flags & IFF_UP) || (i->ifa_flags & IFF_LOOPBACK)) continue;
        found = ((struct sockaddr_in *)i->ifa_addr)->sin_addr.s_addr;
        break;
    }
    freeifaddrs(list);
    return found;
}
struct sockaddr_in udp_address(uint32_t address, uint16_t port)
{
    struct sockaddr_in a;
    memset(&a, 0, sizeof a);
#if defined(__APPLE__) || defined(__FreeBSD__)
    a.sin_len = sizeof a;
#endif
    a.sin_family = AF_INET;
    a.sin_port = htons(port);
    a.sin_addr.s_addr = address;
    return a;
}
// Claims the first free port of the range for this game (held by a reserving
// socket until the game binds its IPX socket).
uint16_t ipx_claim_port()
{
    std::lock_guard<std::mutex> g(ipx_lock);
    if (ipx_port) return ipx_port;
    for (int i = 0; i < ipx_port_count; ++i) {
        int fd = socket(AF_INET, SOCK_DGRAM, 0);
        if (fd < 0) break;
        struct sockaddr_in a = udp_address(htonl(INADDR_ANY), (uint16_t)(ipx_base_port + i));
        if (bind(fd, (struct sockaddr *)&a, sizeof a) == 0) {
            ipx_reserve = fd;
            ipx_port = (uint16_t)(ipx_base_port + i);
            return ipx_port;
        }
        close(fd);
    }
    return 0;
}
void ipx_node(uint8_t node[6])
{
    uint32_t address = ipx_local_address();
    uint16_t port = htons(ipx_claim_port());
    memcpy(node, &address, 4);
    memcpy(node + 4, &port, 2);
}

struct AsyncSelect { HWND window; UINT message; LONG events; };
std::mutex async_lock;
std::map<SOCKET, AsyncSelect> async_sockets;
bool async_running = false;

void async_thread()
{
    for (;;) {
        std::vector<struct pollfd> fds;
        std::vector<AsyncSelect> targets;
        {
            std::lock_guard<std::mutex> g(async_lock);
            for (auto &e : async_sockets) {
                short ev = 0;
                if (e.second.events & 1) ev |= POLLIN;    // FD_READ
                if (e.second.events & 2) ev |= POLLOUT;   // FD_WRITE
                fds.push_back({(int)e.first, ev, 0});
                targets.push_back(e.second);
            }
        }
        if (fds.empty()) { std::this_thread::sleep_for(std::chrono::milliseconds(10)); continue; }
        if (poll(fds.data(), (nfds_t)fds.size(), 10) <= 0) continue;
        for (size_t i = 0; i < fds.size(); ++i) {
            if (fds[i].revents & POLLIN) post_message(targets[i].window, targets[i].message, (WPARAM)fds[i].fd, 1);
            if (fds[i].revents & POLLOUT) {
                post_message(targets[i].window, targets[i].message, (WPARAM)fds[i].fd, 2);
                // FD_WRITE is edge-triggered on Winsock: report once.
                std::lock_guard<std::mutex> g(async_lock);
                auto it = async_sockets.find((SOCKET)fds[i].fd);
                if (it != async_sockets.end()) it->second.events &= ~2;
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(2));
    }
}
}  // namespace

extern "C" {

int WSAStartup(uint16_t version, void *data)
{
    // WSADATA: wVersion, wHighVersion, szDescription[257], szSystemStatus[129],
    // iMaxSockets, iMaxUdpDg, lpVendorInfo.
    unsigned char *d = (unsigned char *)data;
    if (d) {
        memset(d, 0, 2 + 2 + 257 + 129);
        memcpy(d, &version, 2);
        uint16_t high = 0x0101;
        memcpy(d + 2, &high, 2);
        strcpy((char *)d + 4, "ra2_port BSD sockets");
        strcpy((char *)d + 4 + 257, "Running");
    }
    return 0;
}
int WSACleanup(void) { return 0; }
int WSAGetLastError(void) { return wsa_error; }

int WSAAsyncSelect(SOCKET s, HWND window, UINT message, LONG events)
{
    std::lock_guard<std::mutex> g(async_lock);
    if (events == 0) async_sockets.erase(s);
    else {
        fcntl((int)s, F_SETFL, fcntl((int)s, F_GETFL) | O_NONBLOCK);
        async_sockets[s] = {window, message, events};
        if (!async_running) { async_running = true; std::thread(async_thread).detach(); }
    }
    return 0;
}

int EnumProtocolsA(int *protocols, void *buffer, uint32_t *length)
{
    (void)protocols; (void)buffer; (void)length;
    unimplemented("EnumProtocolsA (IPX/SPX enumeration)");
    wsa_error = 10045;
    return -1;
}

SOCKET ra2_socket(int family, int type, int protocol)
{
    if (family == AF_IPX_WIN) {
        // An IPX datagram socket: a UDP socket that may broadcast (see the file comment).
        if (type != SOCK_DGRAM || protocol != NSPROTO_IPX_WIN) { wsa_error = 10043; return ~0u; }   // WSAEPROTONOSUPPORT
        int s = socket(AF_INET, SOCK_DGRAM, 0);
        if (s < 0) { set_error(); return ~0u; }
        int one = 1;
        setsockopt(s, SOL_SOCKET, SO_BROADCAST, &one, sizeof one);
        std::lock_guard<std::mutex> g(ipx_lock);
        ipx_sockets[(SOCKET)s] = IpxSocket();
        return (SOCKET)s;
    }
    int s = socket(family, type, protocol);
    if (s < 0) { set_error(); return ~0u; }
    return (SOCKET)s;
}
int closesocket(SOCKET s)
{
    {
        std::lock_guard<std::mutex> g(async_lock);
        async_sockets.erase(s);
    }
    {
        std::lock_guard<std::mutex> g(ipx_lock);
        ipx_sockets.erase(s);
    }
    return close((int)s) == 0 ? 0 : set_error();
}
int ra2_bind(SOCKET s, const void *address, int length)
{
    if (is_ipx(s)) {
        // The game's own socket number (non-zero) takes this game's port of the range;
        // a socket bound to number 0 (the net card query) takes any port.
        const WinSockAddrIpx *w = (const WinSockAddrIpx *)address;
        uint16_t number = ntohs(w->socket);
        uint16_t port = 0;
        if (number != 0) {
            port = ipx_claim_port();
            if (port == 0) { wsa_error = 10048; return -1; }   // WSAEADDRINUSE: the range is full
            std::lock_guard<std::mutex> g(ipx_lock);
            if (ipx_reserve >= 0) { close(ipx_reserve); ipx_reserve = -1; }
        }
        struct sockaddr_in a = udp_address(htonl(INADDR_ANY), port);
        if (bind((int)s, (struct sockaddr *)&a, sizeof a) != 0) return set_error();
        std::lock_guard<std::mutex> g(ipx_lock);
        ipx_sockets[s].number = number;
        return 0;
    }
    (void)length;
    struct sockaddr_in a;
    to_bsd(address, &a);
    return bind((int)s, (struct sockaddr *)&a, sizeof a) == 0 ? 0 : set_error();
}
int ra2_sendto(SOCKET s, const char *buffer, int length, int flags, const void *to, int tolength)
{
    if (is_ipx(s)) {
        const WinSockAddrIpx *w = (const WinSockAddrIpx *)to;
        static const uint8_t broadcast[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
        if (memcmp(w->nodenum, broadcast, 6) == 0) {
            bool sent = false;
            for (int i = 0; i < ipx_port_count; ++i) {
                struct sockaddr_in a = udp_address(htonl(INADDR_BROADCAST), (uint16_t)(ipx_base_port + i));
                if (sendto((int)s, buffer, (size_t)length, flags, (struct sockaddr *)&a, sizeof a) >= 0) sent = true;
            }
            net_trace(sent ? "broadcast" : "broadcast FAILED", htonl(INADDR_BROADCAST), ipx_base_port, length, buffer);
            return sent ? length : set_error();
        }
        uint32_t address;
        uint16_t port;
        memcpy(&address, w->nodenum, 4);
        memcpy(&port, w->nodenum + 4, 2);
        struct sockaddr_in a = udp_address(address, ntohs(port));
        ssize_t n = sendto((int)s, buffer, (size_t)length, flags, (struct sockaddr *)&a, sizeof a);
        net_trace(n < 0 ? "send FAILED" : "send", address, ntohs(port), length, buffer);
        return n < 0 ? set_error() : (int)n;
    }
    (void)tolength;
    struct sockaddr_in a;
    to_bsd(to, &a);
    ssize_t n = sendto((int)s, buffer, (size_t)length, flags, (struct sockaddr *)&a, sizeof a);
    return n < 0 ? set_error() : (int)n;
}
// One delivery per datagram, as IPX gives: a Mac with two interfaces on one subnet (Ethernet and
// Wi-Fi) receives every broadcast twice, the second copy late; a late copy of an unacknowledged
// packet (a NET_PROGRESS_REPORT) then undid the newer state behind it and the joiner's load wait
// dropped the host (Scout1209 r12). A datagram identical to one from the same sender in the last
// second is dropped; a game resend of an acknowledged packet comes after the retry delay and an
// identical copy inside the second is one the receiver already has.
namespace {
struct RecentDatagram { uint32_t address; uint16_t port; int length; uint64_t hash; std::chrono::steady_clock::time_point when; };
RecentDatagram recent_datagrams[64];
int recent_next = 0;
uint64_t datagram_hash(const char *data, ssize_t n)
{
    uint64_t h = 1469598103934665603ull;                       // FNV-1a
    for (ssize_t i = 0; i < n; ++i) { h ^= (uint8_t)data[i]; h *= 1099511628211ull; }
    return h;
}
bool duplicate_datagram(const struct sockaddr_in &a, const char *data, ssize_t n)
{
    auto now = std::chrono::steady_clock::now();
    uint64_t h = datagram_hash(data, n);
    for (const RecentDatagram &r : recent_datagrams)
        if (r.length == (int)n && r.hash == h && r.address == a.sin_addr.s_addr && r.port == a.sin_port
            && now - r.when < std::chrono::seconds(1))
            return true;
    recent_datagrams[recent_next] = {a.sin_addr.s_addr, a.sin_port, (int)n, h, now};
    recent_next = (recent_next + 1) % 64;
    return false;
}
}
int ra2_recvfrom(SOCKET s, char *buffer, int length, int flags, void *from, int *fromlength)
{
    struct sockaddr_in a;
    socklen_t al = sizeof a;
    ssize_t n = recvfrom((int)s, buffer, (size_t)length, flags, (struct sockaddr *)&a, &al);
    if (n < 0) return set_error();
    if (is_ipx(s)) {
        while (duplicate_datagram(a, buffer, n)) {
            net_trace("recv duplicate dropped", a.sin_addr.s_addr, ntohs(a.sin_port), (int)n, buffer);
            al = sizeof a;
            n = recvfrom((int)s, buffer, (size_t)length, flags, (struct sockaddr *)&a, &al);
            if (n < 0) return set_error();
        }
        net_trace("recv", a.sin_addr.s_addr, ntohs(a.sin_port), (int)n, buffer);
        // The sender's node is its IPv4 address and UDP port (see the file comment).
        if (from) {
            WinSockAddrIpx w;
            memset(&w, 0, sizeof w);
            w.family = AF_IPX_WIN;
            memcpy(w.nodenum, &a.sin_addr.s_addr, 4);
            memcpy(w.nodenum + 4, &a.sin_port, 2);
            {
                std::lock_guard<std::mutex> g(ipx_lock);
                w.socket = htons(ipx_sockets[s].number);
            }
            memcpy(from, &w, sizeof w);
        }
        if (fromlength) *fromlength = (int)sizeof(WinSockAddrIpx);
        return (int)n;
    }
    if (from) to_win(&a, from);
    if (fromlength) *fromlength = 16;
    return (int)n;
}
int ra2_setsockopt(SOCKET s, int level, int name, const char *value, int length)
{
    // IPX's packet type and filter (IPX_PTYPE, IPX_FILTERTYPE) mean nothing over UDP.
    if (level == NSPROTO_IPX_WIN && is_ipx(s)) return 0;
    if (level == 0xFFFF) level = SOL_SOCKET;
    return setsockopt((int)s, level, name, value, (socklen_t)length) == 0 ? 0 : set_error();
}
int ra2_getsockopt(SOCKET s, int level, int name, char *value, int *length)
{
    if (level == NSPROTO_IPX_WIN && is_ipx(s)) {
        if (name == IPX_MAX_ADAPTER_NUM_OPT && *length >= 4) {   // one adapter
            int32_t one = 1;
            memcpy(value, &one, 4);
            *length = 4;
            return 0;
        }
        if (name == IPX_ADDRESS_OPT && *length >= (int)sizeof(WinIpxAddressData)) {
            WinIpxAddressData data;
            memcpy(&data, value, sizeof data);          // adapternum comes in
            memset(data.netnum, 0, sizeof data.netnum);
            ipx_node(data.nodenum);
            data.wan = 0;
            data.status = 1;
            data.maxpkt = 1400;
            data.linkspeed = 1000000;
            memcpy(value, &data, sizeof data);
            *length = (int)sizeof data;
            return 0;
        }
        wsa_error = 10042;   // WSAENOPROTOOPT
        return -1;
    }
    if (level == 0xFFFF) level = SOL_SOCKET;
    socklen_t l = (socklen_t)*length;
    int r = getsockopt((int)s, level, name, value, &l);
    *length = (int)l;
    return r == 0 ? 0 : set_error();
}
int ra2_gethostname(char *name, int length) { return gethostname(name, (size_t)length) == 0 ? 0 : set_error(); }

// Winsock 1.1 HOSTENT (shared/winsock_interface.hpp): 16-bit type and length.
struct WinHostent { char *h_name; char **h_aliases; int16_t h_addrtype; int16_t h_length; char **h_addr_list; };
WinHostent *ra2_gethostbyname(const char *name)
{
    static thread_local WinHostent result;
    static thread_local char *addrs[17];
    static thread_local char storage[16][4];
    static thread_local char *aliases[1];
    struct hostent *h = gethostbyname(name);
    if (!h) { wsa_error = 11001; return nullptr; }   // WSAHOST_NOT_FOUND
    int i = 0;
    for (; h->h_addr_list[i] && i < 16; ++i) {
        memcpy(storage[i], h->h_addr_list[i], 4);
        addrs[i] = storage[i];
    }
    addrs[i] = nullptr;
    aliases[0] = nullptr;
    result.h_name = h->h_name;
    result.h_aliases = aliases;
    result.h_addrtype = (int16_t)h->h_addrtype;
    result.h_length = 4;
    result.h_addr_list = addrs;
    return &result;
}
char *ra2_inet_ntoa(uint32_t addr)
{
    struct in_addr a;
    a.s_addr = addr;
    return inet_ntoa(a);
}
uint32_t ra2_htonl(uint32_t v) { return htonl(v); }
uint32_t ra2_ntohl(uint32_t v) { return ntohl(v); }
uint16_t ra2_htons(uint16_t v) { return htons(v); }
uint16_t ra2_ntohs(uint16_t v) { return ntohs(v); }

}  // extern "C"
