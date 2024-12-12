#include <stdio.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>

typedef enum {
  PROTO_HELLO,
} proto_type_e;

typedef struct {
  proto_type_e type;
  unsigned int len;
} proto_hdr_t;

void handle_client(int fd) {
  char buf[4096] = {0};
  proto_hdr_t *hdr = (proto_hdr_t*)buf;

  hdr->type = PROTO_HELLO;
  hdr->len = sizeof(int);

  int *data = (int *) &hdr[1];
  *data = 1; // protocol version one
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

  //accept
  int cfd = accept(fd, (struct sockaddr *)&clientInfo, &clientSize);
  if(cfd == -1) {
    perror("accept");
    close(fd);
    return -1;
  }

  printf("Accepted on CFD: %d\n", cfd);
  close(cfd);

  return 0;
}
