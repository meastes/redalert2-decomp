// SPDX-License-Identifier: GPL-3.0-or-later
// BSD sockets for the port on the x86_64 Windows build (README "Windows
// build"): the subset port/src/wsock32.cpp uses (UDP datagrams, poll, the local
// IPv4 address), in the BSD layout (sockaddr with sa_len), carried on Winsock 2
// by platform/win/posix_win.cpp. The functions are ra2_bsd_* under their POSIX
// names, so they never meet the Winsock exports of the same names (the port
// defines the game's own Winsock 1.1 entry points: closesocket, WSAStartup, ...).
// Socket option levels and names are Winsock's values, which the game passes
// and which BSD shares for the options used (SO_BROADCAST, SO_REUSEADDR, ...).
// <arpa/inet.h>, <netinet/in.h>, <sys/socket.h>, <netdb.h>, <poll.h>,
// <ifaddrs.h> and <net/if.h> all include this header.
#ifndef RA2_BSD_SOCKET_H
#define RA2_BSD_SOCKET_H
#include <stddef.h>
#include <stdint.h>
#include <sys/types.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef uint8_t sa_family_t;
typedef uint16_t in_port_t;
typedef uint32_t in_addr_t;
typedef int socklen_t;

struct sockaddr { uint8_t sa_len; sa_family_t sa_family; char sa_data[14]; };
struct in_addr { in_addr_t s_addr; };
struct sockaddr_in { uint8_t sin_len; sa_family_t sin_family; in_port_t sin_port; struct in_addr sin_addr; char sin_zero[8]; };
struct hostent { char *h_name; char **h_aliases; int h_addrtype; int h_length; char **h_addr_list; };
struct pollfd { int fd; short events; short revents; };
typedef unsigned int nfds_t;
struct ifaddrs {
    struct ifaddrs *ifa_next;
    char *ifa_name;
    unsigned int ifa_flags;
    struct sockaddr *ifa_addr;
    struct sockaddr *ifa_netmask;
    struct sockaddr *ifa_dstaddr;
    void *ifa_data;
};

#define AF_UNSPEC 0
#define AF_INET 2
#define PF_INET AF_INET
#define SOCK_STREAM 1
#define SOCK_DGRAM 2
#define IPPROTO_UDP 17
#define SOL_SOCKET 0xFFFF
#define SO_REUSEADDR 0x0004
#define SO_BROADCAST 0x0020
#define SO_SNDBUF 0x1001
#define SO_RCVBUF 0x1002
#define INADDR_ANY ((in_addr_t)0x00000000)
#define INADDR_LOOPBACK ((in_addr_t)0x7F000001)
#define INADDR_BROADCAST ((in_addr_t)0xFFFFFFFF)
#define INADDR_NONE ((in_addr_t)0xFFFFFFFF)
#define POLLIN 0x0300                // Winsock POLLRDNORM | POLLRDBAND
#define POLLOUT 0x0010               // Winsock POLLWRNORM
#define POLLERR 0x0001
#define POLLHUP 0x0002
#define POLLNVAL 0x0004
#define IFF_UP 0x1
#define IFF_BROADCAST 0x2
#define IFF_LOOPBACK 0x8

int ra2_bsd_socket(int family, int type, int protocol);
int ra2_bsd_bind(int fd, const struct sockaddr *address, socklen_t length);
ssize_t ra2_bsd_sendto(int fd, const void *buffer, size_t length, int flags, const struct sockaddr *to, socklen_t tolength);
ssize_t ra2_bsd_recvfrom(int fd, void *buffer, size_t length, int flags, struct sockaddr *from, socklen_t *fromlength);
int ra2_bsd_setsockopt(int fd, int level, int name, const void *value, socklen_t length);
int ra2_bsd_getsockopt(int fd, int level, int name, void *value, socklen_t *length);
int ra2_bsd_gethostname(char *name, size_t length);
struct hostent *ra2_bsd_gethostbyname(const char *name);
char *ra2_bsd_inet_ntoa(struct in_addr address);
int ra2_bsd_poll(struct pollfd *fds, nfds_t count, int timeout_ms);
int ra2_bsd_getifaddrs(struct ifaddrs **list);
void ra2_bsd_freeifaddrs(struct ifaddrs *list);
// close() of a socket: the socket is closed; any other descriptor goes to the CRT.
int ra2_bsd_close(int fd);

static inline uint32_t ra2_bsd_swap32(uint32_t v) { return __builtin_bswap32(v); }
static inline uint16_t ra2_bsd_swap16(uint16_t v) { return __builtin_bswap16(v); }

#define socket ra2_bsd_socket
#define bind ra2_bsd_bind
#define sendto ra2_bsd_sendto
#define recvfrom ra2_bsd_recvfrom
#define setsockopt ra2_bsd_setsockopt
#define getsockopt ra2_bsd_getsockopt
#define gethostname ra2_bsd_gethostname
#define gethostbyname ra2_bsd_gethostbyname
#define inet_ntoa ra2_bsd_inet_ntoa
#define poll ra2_bsd_poll
#define getifaddrs ra2_bsd_getifaddrs
#define freeifaddrs ra2_bsd_freeifaddrs
#define close ra2_bsd_close
#define htonl ra2_bsd_swap32
#define ntohl ra2_bsd_swap32
#define htons ra2_bsd_swap16
#define ntohs ra2_bsd_swap16

#ifdef __cplusplus
}
#endif
#endif
