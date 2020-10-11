import socket
sock=socket.socket()
sock.connect(('192.168.1.106',8001))
sock.send(b'hello!')
ret = sock.recv(1024)
print(ret)
sock.close()



