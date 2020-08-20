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
  int ret = 0;
  size_t len;
  struct sockaddr_un un;

  memset(&un, 0, sizeof(un));

  fd = socket(AF_UNIX, SOCK_STREAM, 0);
  un.sun_family = AF_UNIX;
  strcpy(un.sun_path, socket_path);
  len = offsetof(struct sockaddr_un, sun_path) + strlen(socket_path);

  do {
    if(connect(fd, (struct sockaddr *)&un, len) == -1) {
      perror("connect");
      ret = 1;
      break;
    }

    if(write(fd, (void*)msg, sizeof(message_t)) != sizeof(message_t)) {
      perror("wirte");
      ret = 1;
      break;
    }

  } while(0);

  free(msg);
  return ret;
}

message_t* pop_msg(const char *socket_path) {

  message_t *msg = (message_t*)malloc(sizeof(message_t));
  msg->id = 0;
  msg->op = POP;
  msg->next = NULL;

  int fd;
  size_t len;
  struct sockaddr_un un;

  memset(&un, 0, sizeof(un));

  fd = socket(AF_UNIX, SOCK_STREAM, 0);
  un.sun_family = AF_UNIX;
  strcpy(un.sun_path, socket_path);
  len = offsetof(struct sockaddr_un, sun_path) + strlen(socket_path);

  do {
    if (connect(fd, (struct sockaddr *)&un, len) == -1) {
      perror("connect");
      break;
    }

    if(write(fd, (void*)msg, sizeof(message_t)) != sizeof(message_t)) {
      perror("wirte");
      break;
    }

    int n;
    n = read(fd, (void*)msg, sizeof(message_t));
    if (n < 0) {
      perror("read error");
      break;
    }

  } while(0);

  return msg;

}

