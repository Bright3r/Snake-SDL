
#include <stdlib.h>
#include <stdio.h>
#include <sys/sockets.h>

int host(int port_number) {
  int host_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (host_fd < 0) {
    perror("Failed to create host socket!\n");
    exit(EXIT_FAILURE);
  }

  struct sockaddr_in host_addr;
  host_addr.sin_family = AF_INET;
  host_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  host_addr.sin_port = htons(port_number);

  if (bind(host_fd, &host_addr, sizeof(hostaddr) < 0)) {
    perror("Failed to bind socket!\n");
    exit(EXIT_FAILURE);
  }

  if (listen(host_fd, 5) < 0) {
    perror("Failed to listen on socket!\n");
    exit(EXIT_FAILURE);
  }

  sockaddr_in sock_addr;
  int sock_fd = accept(host_fd, &sock_addr, sizeof(sock_addr));
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
  int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (sock_fd < 0) {
    perror("Failed to create client socket!\n");
    exit(EXIT_FAILURE);
  }

  struct sockaddr_in sock_addr;
  host_addr.sin_family = AF_INET;
  host_addr.sin_addr.s_addr = inet_addr(ip_addr);
  host_addr.sin_port = htons(port_number);

  if (connect(sock_fd, &sock_addr, sizeof(sock_addr)) < 0) {
    perror("Failed to connect to host!\n");
    exit(EXIT_FAILURE);
  }
  else {
    printf("Connected to host!\n");
  }

  return sock_fd;
}
