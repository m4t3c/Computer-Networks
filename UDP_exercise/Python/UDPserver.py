import socket
import sys
import time

HOST = '127.0.0.1'
PORT = int(sys.argv[1])

with socket.socket(socket.AF_INET, socket.SOCK_DGRAM) as s:
    s.bind((HOST, PORT))
    data, addr = s.recvfrom(1024)
    print('Here is the message: %s'%data.decode('utf-8'))
    s.sendto('I got your message'.encode('utf-8'), addr)
    time.sleep(1)
