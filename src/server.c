#include <stdio.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include "protocol.h"

void handle_client(int fd) {
  char buf[4096] = {0};
  proto_hdr_t *hdr = (proto_hdr_t*)buf;

  hdr->type = htonl(PROTO_HELLO); // pack the type
  hdr->len = sizeof(int);
  int reallen = hdr->len;
  hdr->len = htons(hdr->len); // pack the len

  /* Learning space ::
   * At this line hdr is a pointer of type `proto_hdr_t` as specified at the start of `handle_client` function.
   * By doing hdr[1], we are peeking forward the size of `proto_hdr_t`.
   * Since we want to send protocol version number of "1" to client, we cast the pointer to `int*`.
   */
  int *data = (int*)&hdr[1]; 
  *data = htonl(1); // protocol version one, packed
  write(fd, hdr, sizeof(proto_hdr_t) + reallen);
}


int main() {
  uint portNumber = 5555;
  struct sockaddr_in serverInfo = {0};
  struct sockaddr_in clientInfo = {0};
  uint clientSize = 0;
  serverInfo.sin_family = AF_INET;
  serverInfo.sin_addr.s_addr = 0;
  serverInfo.sin_port = htons(portNumber);

  int fd = socket(AF_INET, SOCK_STREAM, 0);
  if(fd == -1) {
    perror("socket");
    return -1;
  }
  printf("Oepn socket on FD: %d\n", fd);

  //bind
  if(bind(fd, (struct sockaddr *)&serverInfo, sizeof(serverInfo)) == -1) {
    perror("bind");
    printf("Binding failed!!!\n");
    close(fd);
    return -1;
  }

  //listen
  printf("Listening on FD: %d // Port:%d\n", fd, portNumber);
  if(listen(fd, 0) == -1) {
    perror("listen");
    close(fd);
    return -1;
  }

  while(1) {
    printf("Waiting for new connection ... \n");
    //accept
    int cfd = accept(fd, (struct sockaddr *)&clientInfo, &clientSize);
    if(cfd == -1) {
      perror("accept");
      close(fd);
      return -1;
    }
    printf("Accepted on CFD: %d\n", cfd);
    handle_client(cfd);
    close(cfd);
  }

  close(fd);

  return 0;
}
