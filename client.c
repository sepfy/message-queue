#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stddef.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>

#include "message.h"
#include "client.h"

int push_msg(const char *socket_path, int id, char *payload) {

  message_t *msg = (message_t*)malloc(sizeof(message_t));
  msg->id = id;
  msg->op = PUSH;
  snprintf(msg->payload, MSG_PAYLOAD_MAX, "%s", payload);
  msg->next = NULL;

  int fd;
  int i, n;
  size_t len;
  struct sockaddr_un un;

  memset(&un, 0, sizeof(un));

  fd = socket(AF_UNIX, SOCK_STREAM, 0);
  un.sun_family = AF_UNIX;
  strcpy(un.sun_path, socket_path);
  len = offsetof(struct sockaddr_un, sun_path) + strlen(socket_path);
  if (connect(fd, (struct sockaddr *)&un, len) == -1) {
    perror("connect");
    return 1;
  }

  if(write(fd, (void*)msg, sizeof(message_t)) != sizeof(message_t)) {
    perror("wirte");
    return 1;
  }

  free(msg);
  return 0;
}

message_t* pop_msg(const char *socket_path) {

  message_t *msg = (message_t*)malloc(sizeof(message_t));
  msg->id = 0;
  msg->op = POP;
  msg->next = NULL;

  int fd;
  int i, n;
  size_t len;
  struct sockaddr_un un;

  memset(&un, 0, sizeof(un));

  fd = socket(AF_UNIX, SOCK_STREAM, 0);
  un.sun_family = AF_UNIX;
  strcpy(un.sun_path, socket_path);
  len = offsetof(struct sockaddr_un, sun_path) + strlen(socket_path);
  if (connect(fd, (struct sockaddr *)&un, len) == -1) {
    perror("connect");
    return NULL;
  }

  if(write(fd, (void*)msg, sizeof(message_t)) != sizeof(message_t)) {
    perror("wirte");
    return NULL;
  }

  n = read(fd, (void*)msg, sizeof(message_t));
  if (n < 0) {
    perror("read error");
    return NULL;
  }

  return msg;

}

