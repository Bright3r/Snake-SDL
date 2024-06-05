
#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int host(int port_number) {
  printf("Hosting\n");

  int host_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (host_fd < 0) {
    perror("Failed to create host socket!\n");
    exit(EXIT_FAILURE);
  }

  struct sockaddr_in host_addr;
  host_addr.sin_family = AF_INET;
  host_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  host_addr.sin_port = htons(port_number);

  if (bind(host_fd, (struct sockaddr *) &host_addr, sizeof(host_addr)) < 0) {
    perror("Failed to bind socket!\n");
    exit(EXIT_FAILURE);
  }

  if (listen(host_fd, 5) < 0) {
    perror("Failed to listen on socket!\n");
    exit(EXIT_FAILURE);
  }

  struct sockaddr_in sock_addr;
  socklen_t sock_addr_len = (socklen_t) sizeof(sock_addr);
  int sock_fd = accept(host_fd, (struct sockaddr *) &sock_addr, &sock_addr_len);
  if (sock_fd < 0) {
    perror("Failed to accept connection!\n");
    exit(EXIT_FAILURE);
  }
  else {
    printf("Connected to client!\n");
  }

  return sock_fd;
}

int join(char *ip_addr, int port_number) {
  printf("Joining\n");

  int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (sock_fd < 0) {
    perror("Failed to create client socket!\n");
    exit(EXIT_FAILURE);
  }

  struct sockaddr_in sock_addr;
  sock_addr.sin_family = AF_INET;
  sock_addr.sin_addr.s_addr = inet_addr(ip_addr);
  sock_addr.sin_port = htons(port_number);

  if (connect(sock_fd, (struct sockaddr *) &sock_addr, sizeof(sock_addr)) < 0) {
    perror("Failed to connect to host!\n");
    exit(EXIT_FAILURE);
  }
  else {
    printf("Connected to host!\n");
  }

  char *msg = "RandomGuy69";
  if (send(sock_fd, msg, 12, 0) < 0) {
    perror("Failed to send message through socket!\n");
    exit(EXIT_FAILURE);
  }
  else {
    printf("Joined Lobby!\n");
  }

  return sock_fd;
}

bool getLobbyStatus(int sock_fd) {
  char buffer[100] = {0};
  if (recv(sock_fd, buffer, sizeof(buffer), 0) < 0) {
    perror("Lobby no longer exists!\n");
    exit(EXIT_FAILURE);
  }
  else {
    printf("Received msg: %s\n", buffer);
    if (strcmp(buffer, "Starting") == 0) {
      return true;
    }
  }

  return false;
}
