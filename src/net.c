#include "../include/net.h"

void build_socket_and_send(Arg* arg)
{
  Packet* p;
  if ( ( p = setup_socket(arg) ) == NULL )
  {
    fprintf(stdout,"Some error happen in socket creation!");
    exit(1);
  }
  
  if ( p->type == TCP )
    send_tcp_packet(p);
  else
    send_udp_packet(p);
}

Packet* setup_socket(Arg* args)
{
  Packet* p;
  p = malloc(sizeof(Packet));

  if ( args->options->type == NULL || (!strcmp("TCP", args->options->type) || !strcmp("tcp", args->options->type) ) ) { 
    p->socket = socket(AF_INET, SOCK_STREAM,0);
    p->type = TCP;
  }
  else if ( !strcmp("UDP", args->options->type) || !strcmp("udp", args->options->type) ) {
    p->socket = socket(AF_INET, SOCK_DGRAM,0);
    p->type = UDP;
  }
  
  if ( args->options->data != NULL ) {
    p->data_sent = args->options->data;
    p->has_file = args->options->is_file;
  }
  p->socks_info.sin_family = AF_INET;
  p->socks_info.sin_port = htons(args->options->port);
  p->socks_info.sin_addr.s_addr = inet_addr(args->options->host); 
  
  p->host = args->options->host;
  p->port = args->options->port;
  p->timeout = args->options->timeout;
  memset(p->data_recv, 0, RECV_BUFF);
  return p;
}

int send_tcp_packet(Packet* p)
{
  fcntl(p->socket, F_SETFL, O_NONBLOCK);
  fprintf(stdout, "Sending one tcp packet to %s:%d...\n", p->host,p->port);
  char* port_state = NULL;
  
  connect(p->socket, (struct sockaddr*) &p->socks_info, sizeof(p->socks_info));

  port_state = get_status(p);

  fcntl(p->socket, F_SETFL, F_LOCK); // re-enable block-mode in socket

  display_status(p, port_state);
  if ( p->data_sent != NULL && !strcmp(port_state,"Open"))
  {
    fprintf(stdout, "Sending your custom data to %s:%d...", p->host,p->port);
    write(p->socket, p->data_sent, strlen(p->data_sent));
    fprintf(stdout, "\nData sent! waiting response...\n");
    read(p->socket, p->data_recv, sizeof(p->data_recv));
    fprintf(stdout, "\nResponse:\n\n %s\n", p->data_recv);
  }

  clean(p);
  fprintf(stdout, "Connection closed\n");
  return HOST_ONLINE;
}

int send_udp_packet(Packet* p)
{
  
  socklen_t sin_size = sizeof( struct sockaddr );
  fprintf(stdout, "Sending one udp packet to %s:%d..\n", p->host, p->port);
  if ( p->data_sent == NULL ) p->data_sent = "..";
  sendto(p->socket, p->data_sent, strlen(p->data_sent), 0 , ( struct sockaddr*)&p->socks_info, sizeof(p->socks_info));
   
  fprintf(stdout, "Packet sent!, waiting response...\n\n");
  get_status(p);
  recvfrom(p->socket, p->data_recv, strlen(p->data_recv), 0, ( struct sockaddr * )&p->socks_info,  &sin_size); 
  fprintf(stdout,"\nServer response:\n\t%s\n", p->data_recv); 
  
  clean(p);
  return 0;
}


char* get_status(Packet* p)
{
  fd_set fdset;
  FD_ZERO(&fdset);
  FD_SET(p->socket, &fdset);
  struct timeval timer;
  timer.tv_sec = p->timeout;
  timer.tv_usec = 0;
  if (setsockopt(p->socket, SOL_SOCKET, SO_RCVTIMEO,&timer,sizeof(timer)) < 0) {
    fprintf(stderr, "Cant set timeout, the connection can be stucked...");
  }
  if ( select(p->socket + 1 ,NULL, &fdset, NULL, &timer) == 1 )
  {
    int so_error;
    socklen_t len = sizeof(so_error);
    getsockopt(p->socket, SOL_SOCKET, SO_ERROR, &so_error, &len);
    if ( so_error == 0 )
      return OPEN;
    else
      return CLOSED;
  } 
  return FILTERED;
}


void clean(Packet* p)
{
  close(p->socket);
  if ( p->has_file )
    free(p->data_sent);

  free(p);
}

void display_status(Packet* p, const char* port_state)
{ 
  fprintf(stdout,"\n \t----- [+] Connection info [+] -----\n\n Host: %s\t Port: %d\t state: %s\n", p->host, p->port, port_state);
  fprintf(stdout,"\n \t-----------------------------------\n\n"); 
}
