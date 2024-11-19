#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include </usr/include/asm-generic/socket.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include </usr/include/linux/stat.h>
#include <sys/stat.h>
#include <sys/wait.h>


#define BUF_SIZE 2048
#define BACKLOG 10
#define DEBUG 0



void handle_client(int cfd);

void handle_response(char **response, const char *status, char *message);

void handle_message(char *path ,char ** status_value , char** message, int cfd);
