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
  getaddrinfo(NULL, "6969", &hints, &addr);

  // create a new socket using the addrinfo we got
  int socket_fd = socket(addr->ai_family, addr->ai_socktype, addr->ai_protocol);

  // bind the new socket to the port 6969 using addrinfo we have
  bind(socket_fd, addr->ai_addr, addr->ai_addrlen);

  // free addrinfo since we do not need it anymore
  freeaddrinfo(addr);

  // listen on the socket (i don't know what this do tbh)
  listen(socket_fd, 10);

  // create the client sockaddr, this is somehow required by the accept function? even though we do not use it in here, but anyways
  struct sockaddr client_addr;
  socklen_t client_addrlen = sizeof(client_addr);

  int i = 0;
  do {
    // accept the new connection once it comes if it comes, this operation
    // is blocking by nature, until a new connection is made then it lets the program keep running
    int socket_client_fd = accept(socket_fd, &client_addr, &client_addrlen);

    // here we are receiving the request that the client sent to us, and we are putting it in a buffer
    char req[1024];
    recv(socket_client_fd, req, 1024, 0);

    // here we are sending the response to the client
    const char *response = "HTTP/1.1 200 OK\r\nConnection: close\r\nContent-Type: text/plain\r\n\r\nHello World how are you guys doing today?\n";
    send(socket_client_fd, response, strlen(response), 0);
    close(socket_client_fd);
  } while (i++ < 5);

  // close our socket
  close(socket_fd);

  return 0;
}
