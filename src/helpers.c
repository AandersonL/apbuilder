#include "../include/helpers.h"

int validate_ip(const char* ip)
{
  struct sockaddr_in socks_aux;  
  return inet_pton(AF_INET, ip, &socks_aux); 
}

int validate_port(int port)
{
  return (port < 65535 && port > 0); 
}

char* get_ip_by_name(const char* address)
{
  struct hostent* host_info = gethostbyname(address);
  if ( host_info )
    return inet_ntoa(*(struct in_addr*) host_info->h_addr);
 
  return NULL; 
}

int set_data(char** data, const char* fileName)
{
  unsigned buff;
  FILE* f = fopen(fileName, "rb");
  if ( !f )
    return -1;

  fseek(f, 0, SEEK_END);
  buff = ftell(f);
  fseek(f, 0, SEEK_SET);
  *data = malloc(sizeof(char)* buff + 1);
  fread(*data, buff, 1, f);
  fclose(f);
  
  return 0; 
}
