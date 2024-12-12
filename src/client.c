#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

int main(int argc, char *argv[]) {
  if(argc != 2) {
    printf("Usage: %s <ip of the host>\n", argv[0]);
    return 0;
  }

  uint portNumber = 5555;
  struct sockaddr_in serverInfo = {0};
  serverInfo.sin_family = AF_INET;
  serverInfo.sin_addr.s_addr = inet_addr(argv[1]);
  serverInfo.sin_port = htons(portNumber);

  int fd = socket(AF_INET, SOCK_STREAM, 0);
  if(fd == -1) {
    perror("socket");
    return -1;
  }
  printf("Oepn socket on FD: %d\n", fd);

  if(connect(fd, (struct sockaddr*) &serverInfo, sizeof(serverInfo)) == -1) {
    perror("connect");
    close(fd);
    return -1;
  }
  
  return 0;
}
