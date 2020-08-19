#include <stdio.h>
#include <stdlib.h>

#include "client.h"

int main(int argc, char *argv[]) {

    if(argc < 2) {
      printf("Usage: \n");
      printf("./client push <int: id> <string: payload>\n");
      printf("./client pop\n");
      exit(0);
    }

    const char *socket_path = "/tmp/msgq.sock";

    if(strcmp(argv[1], "push") == 0) {
      if(argc < 4) {
        printf("Usage: \n");
        printf("./client push <int: id> <string: payload>\n");
        exit(0);
      }

      push_msg(socket_path, atoi(argv[2]), argv[3]);
    }
    else {
      message_t *msg = pop_msg(socket_path);
      printf("Pop message queue id = %d, payload = %s\n", msg->id, msg->payload);
      free(msg);
    }

    return 0;
}
