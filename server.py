import socket
import sys


class Server(object):
    def __init__(self, port):
        self.port = port
        self.s = socket.socket()
    
    def set_socket_and_listen(self):
        try:
            self.s.bind(("",self.port))
            self.wait_connections()
        except Exception as e:
            print(e)
    
    def wait_connections(self):
        self.s.listen(1)
        print("Waiting connections on {}".format(self.port))
        while True:
            conn, addr = self.s.accept()
            print("Connected with {}:{}".format(addr[0],str(addr[1])))
            conn.send("Hello from host".encode())
            data = conn.recv(4096)
            print("raw data {}".format(data))
            conn.send("closing..".encode())
            conn.close()



def main():
    if ( len(sys.argv) < 2 ):
        print("usage: python3 server.py <port>")
        sys.exit()

    try:
        server = Server(int(sys.argv[1]))
        server.set_socket_and_listen()
    except Exception as e:
        print(e)


main()
