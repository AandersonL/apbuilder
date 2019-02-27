# apBuilder - Another Packet Builder

apbuilder is TCP/UDP packet builder written with raw sockets, wich allow you to check services and send custom data to a remote host.

## Install

For compile for your system and install, just type
  
  * `./install.sh`

## Usage

    $ apbuilder
        Usage:
      ./apbuilder -h <host> -p <port> <custom_options>

      Options:
       --host, -h 			Target host
       --port, -p 			Target port, default: port 80

      Custom options:
       --data, -d 			Custom data
       --file, -f 			Data file
       --type, -t 			Type: TCP, UDP
       --timeout, -T 		Timeout, default: 10 seconds 
 
 
### Examples
 

There is a simple python3 server that you can use for tests
      
     python3 server.py <port_num>
   
   
Checking a service


    apbuilder -h localhost -p 22
    
Checking a service with timeout

    apbuilder -h localhost -p 445 -T 30
   
Sending custom data

    apbuilder -h localhost -p 8000 -d "Hello from apbuilder"
    
Read a file and send his bytes

    apbuilder -h localhost -p 4422 -f my_data_file
 
Read a file and send some bytes using UDP connection

    apbuilder -h localhost -p 4423 -f my_data_file -t udp


