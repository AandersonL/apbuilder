// Local api's
#include "../include/common.h"
#include "../include/net.h"
#include "../include/helpers.h"

#include <getopt.h>

int argparse(Arg* arg);
void usage();
void fatal(const char* msg);



int argparse(Arg* arg)
{
  arg->options = malloc(sizeof(Options));
  arg->options->host = NULL;
  arg->options->port = 0;
  arg->options->type = NULL;
  arg->options->timeout = 10;
  int op;
  char* short_op = "h:p:t:d:f:T:";
  struct option options[] =
  { 
    {"host", no_argument, 0, 'h'},
    {"port", no_argument, 0, 'p'},
    {"type", no_argument, 0, 't'},
    {"data", no_argument, 0, 'd'},
    {"file", no_argument, 0, 'f'},
    {"timeout",no_argument, 0, 'T'},
    {0}
  };
  while ( ( op = getopt_long(arg->len,arg->argv,short_op,options, NULL )) != -1  )
  {
    switch(op)
    {
      case 'h':
        if ( !validate_ip(optarg) ) 
        {
          if ( ( arg->options->host = get_ip_by_name( optarg ) ) == NULL )
            fatal("Invalid address");
        } 
        else
          arg->options->host = optarg;
        break;
      case 'p':
        if ( !validate_port(atoi(optarg)) ) fatal("Invalid port number!");
        arg->options->port = atoi(optarg);
        break;
      case 't':
        arg->options->type = optarg;
        break;
      case 'd':
        arg->options->data = optarg;
        break;
      case 'T':
        arg->options->timeout = atoi(optarg);
        break;
      case 'f':
        if (set_data(&arg->options->data,optarg) < 0 ) fatal("Invalid file");
        arg->options->is_file = 1;
        break;

      default:
        usage();
    }
  }

  if ( arg->options->port == 0 )
    arg->options->port = 80;
  if ( arg->options->host == NULL )
    return -1;
  
  return 0;
} 
void usage()
{
  fprintf(stderr,"Usage:\n./apbuilder -h <host> -p <port> <custom_options>\n");
  fprintf(stderr,"\nOptions:\n --host, -h \t\t\tTarget host\n");
  fprintf(stderr," --port, -p \t\t\tTarget port, default: port 80\n");

  fprintf(stderr,"\nCustom options:\n");
  fprintf(stderr," --data, -d \t\t\tCustom data\n");
  fprintf(stderr," --file, -f \t\t\tData file\n");
  fprintf(stderr," --type, -t \t\t\tType: TCP, UDP\n");
  fprintf(stderr," --timeout, -T \t\t\tTimeout, default: 10 seconds\n");
  exit(1);
}

void fatal(const char* msg)
{
  fprintf(stderr,"Error: %s\n\n", msg);
  usage();
} 

int main(int argc, char** argv)
{
  if ( argc < 3 )
    usage();

  Arg* arg = malloc(sizeof(Arg));  

  arg->argv = argv;
  arg->len = argc;
  
  
  if ( argparse(arg) < 0 ) usage();
  build_socket_and_send(arg);
  free(arg->options);
  free(arg);
  return 0;
}

