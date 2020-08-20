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

int do_operation(int cfd, message_queue_t *msg_queue, message_t *msg) {

  switch(msg->op) {
    case PUSH:
      enqueue(msg_queue, msg);
      break;
    case POP:
      free(msg);
      msg = dequeue(msg_queue);
      if(msg == NULL) {
        msg = (message_t*)malloc(sizeof(message_t));
	msg->id = -1;
      }
      if (write(cfd, (void*)msg, sizeof(message_t)) != sizeof(message_t)) {
        perror("wirte");
      }
      free(msg);
      break;
    default:
      break;
  }

}

int run_broker(const char *socket_path) {

    int fd;
    int i, n;
    size_t len;
    struct sockaddr_un un;

    message_queue_t *msg_queue = create_message_queue();

    memset(&un, 0, sizeof(un));
    fd = socket(AF_UNIX, SOCK_STREAM, 0);
    unlink(socket_path);
    un.sun_family = AF_UNIX;
    strcpy(un.sun_path, socket_path);
    len = offsetof(struct sockaddr_un, sun_path) + strlen(socket_path);

    if (bind(fd, (struct sockaddr *)&un, len) == -1) {
      perror("bind");
      return 1;
    }

    if (listen(fd, 128) == -1) {
      perror("listen");
      return 1;
    }

    message_t *msg;

    while(1) {

      int cfd = 0;

      if ((cfd = accept(fd, NULL, NULL)) == -1) {
        perror("accept");
        return 1;
      }


      while(1) {
        msg = (message_t*)malloc(sizeof(message_t));
        n = read(cfd, (void*)msg, sizeof(message_t));

        if (n < 0) {
          perror("read error");
          break;
        }
        else if(n == 0) {
          break;
        }
        do_operation(cfd, msg_queue, msg);
      }

      //for(message_t *msg = msg_queue->front; msg != NULL; msg = msg->next) {
      //  printf("id = %d, payload = %s,", msg->id, msg->payload);
      //}
      //printf("\n");
      close(cfd);
    }

    return 0;
}


int main(int argc, char *argv[]) {
  const char *socket_path = "/tmp/msgq.sock";
  run_broker(socket_path);
  return 0;
}
