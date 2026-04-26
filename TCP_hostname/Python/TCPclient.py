import socket
import sys

if(len(sys.argv) < 3) :
    print(f"ERROR: usage is {sys.argv[0]} server port")
    exit()

HOST = sys.argv[1]
PORT = int(sys.argv[2])

with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    s.connect((HOST, PORT))
    server_name = s.recv(1024).decode('utf-8')[13:]
    print(f"Server name is: {server_name}")
    s.close()
