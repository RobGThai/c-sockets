#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/poll.h>

#define TIMEOUT 5

int new_fd(char *filename) {
  int fd = open(filename, O_RDONLY);

  if (fd == -1) {
    perror("open");
    return -1;
  }
  return fd;
}

int main (void) {
  struct pollfd fds[2];
  nfds_t fdCount = 2;
  int ret;

  char buf[10];
  ssize_t s;

  int fd = new_fd("fd1_io");

  if (fd == -1) {
    return 1;
  }
  /* watch stdin for input */
  fds[0].fd = fd; //STDIN_FILENO;
  fds[0].events = POLLIN;

  int fd2 = new_fd("fd2_io");

  if (fd2 == -1) {
    return 1;
  }
  /* watch stdout for ability to write */
  fds[1].fd = fd2; //STDOUT_FILENO;
  fds[1].events = POLLIN;

  do {
    ret = poll(fds, 2, TIMEOUT * 1000);

    if (ret == -1) {
      perror ("poll");
      return 1;
    }

    if (!ret) {
      printf ("%d seconds elapsed.\n", TIMEOUT);
      return 0;
    }

    for (nfds_t i = 0; i < fdCount; i++) {
      if(fds[i].revents & fds[i].events) {
        s = read(fds[i].fd, buf, sizeof(buf));
        if (s == -1) 
          perror("read");

        // Clean up NEWLINE
        if (buf[(int)s -1] == '\n') {
          buf[(int)s -1] = '\0';
        }

        if (s == 0) continue;

        printf("FD[%d] read %zd (%d) bytes: [%s]\n",
               (int) i, s, (int) strlen(buf), buf);

        int x = strcmp(buf, "TERM");
        printf("Compare input %d\n", x);

        if(strcmp(buf, "TERM") == 0) {
          printf("Closing FD[%d].....\n", (int)i);
          // Close the FD
          close(fds[i].fd);
          fdCount--;
        }
      }
    }
  } while(fdCount > 0);

  return 0;
}
