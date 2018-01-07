#pragma once

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <fcntl.h>


#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct options
{
  const char* host;
  int port;
  char* data;
  int is_file;
  const char* type;
  const char* protocol;
  int timeout;
}Options;


typedef struct args
{
  char** argv;
  int len;
  Options* options;
} Arg;

