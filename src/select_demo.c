
#include <stddef.h>
#include <stdio.h>
#include <sys/select.h>

fd_set readfds;
int fd1, fd2, nfds;

int main() {
  // Initialize fd1 and fd2 with valid socket file descriptors
  FD_ZERO(&readfds); // Prepare file descriptors set by initializing them all with 0
  FD_SET(fd1, &readfds); // Add fd1 to the set
  FD_SET(fd2, &readfds);

  // nfds should be 1 more than the highest file descriptor number
  nfds = (fd1 > fd2 ? fd1 : fd2) + 1;

  printf("Waiting incoming at FD: %d\n", nfds);
  // Wait for an input on either socket
  if (select(nfds, &readfds, NULL, NULL, NULL) > 0) {
    if (FD_ISSET(fd1, &readfds)) {
      printf("Handle FD1\n");
      // Handle input on fd1
    }
    if (FD_ISSET(fd2, &readfds)) {
      printf("Handle FD2\n");
      // Handle input on fd2
    }
  } else {
    // Handle timeout or error
  }
}
