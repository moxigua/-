import socket
server=socket.socket(socket.AF_INET,socket.SOCK_DGRAM)
server.bind(("127.0.0.1",8080))
while True:
    date,mag=server.recvfrom(1024)
    print(date)
    server.sendto(date.upper(),mag)
server.close()
