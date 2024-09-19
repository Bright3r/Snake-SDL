#ifndef NETWORK_H
#define NETWORK_H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
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

bool sendMessage(int sock_fd, char *msg);
int host(int port_number);
int join(char *ip_addr, int port_number);
bool readSocket(int sock_fd, char *buffer);
bool getLobbyStatus(int sock_fd);
void declareWin(int sock_fd);
void declareLoss(int sock_fd);
void declareRestart(int sock_fd);
void *listenForOtherPlayer(void *args);
int createThread(pthread_t *thread, int sock_fd, enum game_stats *game_status, pthread_mutex_t *lock);

#endif
