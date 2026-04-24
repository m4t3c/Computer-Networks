import socket
import sys

HOST = sys.argv[1]
PORT = int(sys.argv[2])

with socket.socket(socket.AF_INET, socket.SOCK_DGRAM) as s:
    msg = input('Please enter the message: ')
    s.sendto(msg.encode('utf-8'), (HOST,PORT))
    data = s.recvfrom(1024)
print('Received: %s'% data[0].decode('utf-8'))