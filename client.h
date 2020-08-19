#include <stdio.h>
#include <string.h>

#include "message.h"

int push_msg(const char *socket_path, int id, char *payload);
message_t* pop_msg(const char *socket_path);

