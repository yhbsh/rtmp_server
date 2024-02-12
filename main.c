#include <errno.h>
#include <netdb.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  if (argc != 3) {
    perror("USAGE[]: ./main MAX_REQ PORT");
    return 1;
  }

  // get addrinfo for the port 6969 to bind it to a socket
  struct addrinfo hints = {.ai_family = AF_INET, .ai_socktype = SOCK_STREAM, .ai_flags = AI_PASSIVE};
  struct addrinfo *addr;

  int error = getaddrinfo(NULL, argv[2], &hints, &addr);
  if (error != 0) {
    fprintf(stderr, "[ERROR]: getaddrinfo() failed: %s\n", gai_strerror(error));
    return 1;
  }

  // create a new socket using the addrinfo we got
  int socket_fd = socket(addr->ai_family, addr->ai_socktype, addr->ai_protocol);
  if (socket_fd == -1) {
    perror("socket()");
    return 1;
  }

  int yes = 1;
  if (setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1) {
    perror("setsockopt()");
    return 1;
  }

  // bind the new socket to the port 6969 using addrinfo we have
  if (bind(socket_fd, addr->ai_addr, addr->ai_addrlen) == -1) {
    perror("bind()");
    return 1;
  }

  // free addrinfo since we do not need it anymore
  freeaddrinfo(addr);

  // listen on the socket (i don't know what this do tbh)
  if (listen(socket_fd, 100) == -1) {
    perror("listen()");
    return 1;
  }

  int i = 0;

  do {
    // accept the new connection once it comes if it comes, this operation
    // is blocking by nature, until a new connection is made then it lets the program keep running
    int socket_client_fd = accept(socket_fd, NULL, NULL);
    if (socket_client_fd == -1) {
      perror("accept()");
      return 1;
    }

    // Handshake
    char c0[1];
    char c1[1536];
    char c2[1536];

    if (recv(socket_client_fd, c0, 1, 0) == -1) {
      perror("recv()");
      return 1;
    }

    if (recv(socket_client_fd, c1, 1536, 0) == -1) {
      perror("recv()");
      return 1;
    }

    if (send(socket_client_fd, c0, 1, 0) == -1) {
      perror("send()");
      return 1;
    }

    if (send(socket_client_fd, c1, 1536, 0) == -1) {
      perror("send()");
      return 1;
    }

    if (recv(socket_client_fd, c2, 1536, 0) == -1) {
      perror("recv()");
      return 1;
    }

    if (send(socket_client_fd, c2, 1536, 0) == -1) {
      perror("send()");
      return 1;
    }

    if (close(socket_client_fd) == -1) {
      perror("close()");
      return 1;
    }

  } while (++i < atoi(argv[1]));

  // close our socket
  close(socket_fd);

  return 0;
}
