// WSOCK32 (Winsock 1.1) on BSD sockets, under the ra2_ names native_units.py
// gives the clashing calls. Translates the Winsock sockaddr_in (no sin_len,
// 16-bit family) and HOSTENT (16-bit type/length), and emulates WSAAsyncSelect
// with a poll thread that posts the window message as Winsock does.
#include "ra2port/win32_port.hpp"

#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <map>
#include <mutex>
#include <netdb.h>
#include <netinet/in.h>
#include <poll.h>
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
    return close((int)s) == 0 ? 0 : set_error();
}
int ra2_bind(SOCKET s, const void *address, int length)
{
    (void)length;
    struct sockaddr_in a;
    to_bsd(address, &a);
    return bind((int)s, (struct sockaddr *)&a, sizeof a) == 0 ? 0 : set_error();
}
int ra2_sendto(SOCKET s, const char *buffer, int length, int flags, const void *to, int tolength)
{
    (void)tolength;
    struct sockaddr_in a;
    to_bsd(to, &a);
    ssize_t n = sendto((int)s, buffer, (size_t)length, flags, (struct sockaddr *)&a, sizeof a);
    return n < 0 ? set_error() : (int)n;
}
int ra2_recvfrom(SOCKET s, char *buffer, int length, int flags, void *from, int *fromlength)
{
    struct sockaddr_in a;
    socklen_t al = sizeof a;
    ssize_t n = recvfrom((int)s, buffer, (size_t)length, flags, (struct sockaddr *)&a, &al);
    if (n < 0) return set_error();
    if (from) to_win(&a, from);
    if (fromlength) *fromlength = 16;
    return (int)n;
}
int ra2_setsockopt(SOCKET s, int level, int name, const char *value, int length)
{
    if (level == 0xFFFF) level = SOL_SOCKET;
    return setsockopt((int)s, level, name, value, (socklen_t)length) == 0 ? 0 : set_error();
}
int ra2_getsockopt(SOCKET s, int level, int name, char *value, int *length)
{
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
