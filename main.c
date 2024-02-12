#include <errno.h>
#include <netdb.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

int main(void) {
  // get addrinfo for the port 6969 to bind it to a socket
  struct addrinfo hints = {.ai_family = AF_INET, .ai_socktype = SOCK_STREAM, .ai_flags = AI_PASSIVE};
  struct addrinfo *addr;

  int error = getaddrinfo(NULL, "6969", &hints, &addr);
  if (error != 0) {
    fprintf(stderr, "[ERROR]: getaddrinfo() failed: %s\n", gai_strerror(error));
    return 1;
  }

  // create a new socket using the addrinfo we got
  int socket_fd = socket(addr->ai_family, addr->ai_socktype, addr->ai_protocol);
  if (socket_fd == -1) {
    fprintf(stderr, "[ERROR]: socket() failed: %s\n", strerror(errno));
    return 1;
  }

  // bind the new socket to the port 6969 using addrinfo we have
  if (bind(socket_fd, addr->ai_addr, addr->ai_addrlen) == -1) {
    fprintf(stderr, "[ERROR]: bind() failed: %s\n", strerror(errno));
    return 1;
  }

  // free addrinfo since we do not need it anymore
  freeaddrinfo(addr);

  // listen on the socket (i don't know what this do tbh)
  if (listen(socket_fd, 100) == -1) {
    fprintf(stderr, "[ERROR]: listen() failed: %s\n", strerror(errno));
    return 1;
  }

  int i = 0;
  do {
    // accept the new connection once it comes if it comes, this operation
    // is blocking by nature, until a new connection is made then it lets the program keep running
    int socket_client_fd = accept(socket_fd, NULL, NULL);
    if (socket_client_fd == -1) {
      fprintf(stderr, "[ERROR]: accept() failed: %s\n", strerror(errno));
      return 1;
    }

    // here we are receiving the request that the client sent to us, and we are putting it in a buffer
    char req[1024];
    if (recv(socket_client_fd, req, 1024, 0) == -1) {
      fprintf(stderr, "[ERROR]: recv() failed: %s\n", strerror(errno));
      return 1;
    }

    // here we are sending the response to the client
    const char *response = "HTTP/1.1 200 OK\r\nConnection: close\r\nContent-Type: text/plain\r\n\r\nHello World how are you guys doing today?\n";
    if (send(socket_client_fd, response, strlen(response), 0) == -1) {
      fprintf(stderr, "[ERROR]: send() failed: %s\n", strerror(errno));
      return 1;
    }

    if (close(socket_client_fd) == -1) {
      fprintf(stderr, "[ERROR]: close() failed: %s\n", strerror(errno));
      return 1;
    }

  } while (++i < 5);

  // close our socket
  close(socket_fd);

  return 0;
}
