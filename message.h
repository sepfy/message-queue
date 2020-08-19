#ifndef MESSAGE_H_
#define MESSAGE_H_

#include <stdio.h>
#include <stdlib.h>

#define MSG_PAYLOAD_MAX 128

typedef enum {
  PUSH,
  POP,
} operation;

typedef struct message {

  operation op;
  int id;
  char payload[MSG_PAYLOAD_MAX];
  struct message *next;

} message_t;

typedef struct {

  message_t *front;
  message_t *rear;

} message_queue_t;

message_queue_t* create_message_queue();
message_t* dequeue(message_queue_t *msg_queue);
void enqueue(message_queue_t *msg_queue, message_t *msg);


#endif // MESSAGE_H_
