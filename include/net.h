#pragma once

#include "common.h"

#define RECV_BUFF 4096
#define TCP 1
#define UDP 2
#define HOST_ONLINE 1
#define OPEN "Open"
#define CLOSED "Closed"
#define FILTERED "Filtered (cant get any response from the host, try incresse timeout with --timeout)"

typedef struct packet
{
  int socket;
  struct sockaddr_in socks_info;
  char data_recv[RECV_BUFF]; 
  char* data_sent;
  int has_file;

  const char* host;
  int port;
  int timeout;
  int type;
} Packet;

void build_socket_and_send(Arg* arg);
Packet* setup_socket(Arg* args);
int send_tcp_packet(Packet* p);
int send_udp_packet(Packet* p);
char* get_status(Packet* p);
void display_status(Packet* p, const char* status);
