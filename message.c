#include <stdio.h>
#include <stdlib.h>

#include "message.h"

message_queue_t* create_message_queue() {
  message_queue_t *msg_queue = (message_queue_t*)malloc(sizeof(message_queue_t));
  if(msg_queue != NULL) {
    msg_queue->rear = NULL;
    msg_queue->front = NULL;
  }  
  return msg_queue;
}


void enqueue(message_queue_t *msg_queue, message_t *msg) {
  if(msg_queue->front == NULL) {
    msg_queue->front = msg;
    msg_queue->rear = msg;
  }
  else {
    msg_queue->rear->next = msg;
    msg_queue->rear = msg;
  }
}

message_t* dequeue(message_queue_t *msg_queue) {

  message_t *msg = msg_queue->front;

  if(msg_queue->front == msg_queue->rear) {
    msg_queue->front = NULL;
    msg_queue->rear = NULL; 
  }
  else {
    msg_queue->front = msg_queue->front->next;
  }

  return msg;
}

