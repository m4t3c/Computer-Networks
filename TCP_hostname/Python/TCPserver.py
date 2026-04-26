import socket
import sys
import time

if (len(sys.argv) < 2):
    print(f'ERROR usage is {sys.argv[0]} port')
    exit()

HOST = '127.0.0.1'
PORT = int(sys.argv[1])

with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    s.bind((HOST, PORT))
    s.listen()
    conn, addr = s.accept()
    server_name = socket.gethostname()
    conn.sendall(f'Welcome form {server_name}'.encode('utf-8'))
