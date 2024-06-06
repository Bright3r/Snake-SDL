
#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>

enum game_stats {Won, Lost, Ongoing, NonCompetitive};

typedef struct thread_args {
  int sock_fd;
  pthread_mutex_t *lock;
  enum game_stats *game_status;
} thread_args;

bool sendMessage(int sock_fd, char *msg) {
  if (send(sock_fd, msg, strlen(msg) + 1, 0) < 0) {
    perror("Failed to send message through socket!\n");
    return false;
  }
  else {
    printf("Message sent: %s\n", msg);
    return true;
  }
}

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

  sendMessage(sock_fd, "RandomGuy");  // announce who joined
  return sock_fd;
}

bool readSocket(int sock_fd, char *buffer) {
  if (recv(sock_fd, buffer, sizeof(buffer), 0) <= 0) {
    perror("Lobby no longer exists!\n");
    exit(EXIT_FAILURE);
  }
  else {
    printf("Received msg: %s\n", buffer);
    return true;
  }

  return false;
}

bool getLobbyStatus(int sock_fd) {
  char buffer[100] = {0};
  if (!readSocket(sock_fd, buffer)) {
    perror("Lobby no longer exists!\n");
    exit(EXIT_FAILURE);
  }
  else {
    if (strcmp(buffer, "Starting") == 0) {
      return true;
    }
  }

  return false;
}

void declareWin(int sock_fd) {
  sendMessage(sock_fd, "Victory");
}

void declareLoss(int sock_fd) {
  sendMessage(sock_fd, "Defeat");
}

void declareRestart(int sock_fd) {
  sendMessage(sock_fd, "Restart");
}

void *listenForOtherPlayer(void *args) {
  // Parse thread args
  thread_args *my_args = (thread_args *) args;
  int sock_fd = my_args->sock_fd;
  pthread_mutex_t *lock = my_args->lock;
  enum game_stats *game_status = my_args->game_status;
  free(my_args);

  bool isRunning = true;
  while (isRunning) {
    char buffer[100] = {0};
    if (!readSocket(sock_fd, buffer)) {
      perror("Connection Terminated!\n");
      exit(EXIT_FAILURE);
    }
    else {
      pthread_mutex_lock(lock);
      if (*game_status == Ongoing) {
        if (strcmp(buffer, "Victory") == 0) {
          *game_status = Lost;
          isRunning = false;
        }
        else if (strcmp(buffer, "Defeat") == 0) {
          *game_status = Won;
          isRunning = false;
        }
      }
      else {  // Kill thread if game already over
        isRunning = false;
      }

      pthread_mutex_unlock(lock);
    }
  }

  pthread_exit(NULL);
}

int createThread(pthread_t *thread, int sock_fd, enum game_stats *game_status, pthread_mutex_t *lock) {
  thread_args *args = (thread_args *) malloc(sizeof(thread_args));
  args->sock_fd = sock_fd;
  args->game_status = game_status;
  args->lock = lock;

  return pthread_create(thread, NULL, listenForOtherPlayer, (void *) args);
}
