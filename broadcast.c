#include <netinet/in.h>
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>

int main() {
  struct sockaddr_in addr;
  unsigned short port = 0;
  const int yes = 1;
  int fd;

  if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
    perror("socket failed");
    return 1;
  }
  if (setsockopt(fd, SOL_SOCKET, SO_BROADCAST, &yes, sizeof(yes)) != 0) {
    perror("setsockopt failed");
    return 1;
  }

  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = htonl(INADDR_BROADCAST);

  do {
    addr.sin_port = port;
    if (sendto(fd, "a", 1, 0, (struct sockaddr*)&addr, sizeof(addr)) >= 0) {
      printf("%u ok\n", port);
      fflush(stdout);
    }
    if ((~port & 0x3FF) == 0x3FF)
      printf("checked %u ports\n", port);
    usleep(1000);
  } while (++port);
}

/*
#include <net/if.h>
#include <stdlib.h>

#define DIE_IF_ERROR(expr) \
  if (!(expr)) {           \
  } else {                 \
    perror(#expr);         \
    exit(EXIT_FAILURE);    \
  }

  unsigned interface;
  DIE_IF_ERROR((interface = if_nametoindex("utun0")) == 0)
  DIE_IF_ERROR(setsockopt(fd, IPPROTO_IP, IP_BOUND_IF, &interface, sizeof(interface)) != 0)
*/
