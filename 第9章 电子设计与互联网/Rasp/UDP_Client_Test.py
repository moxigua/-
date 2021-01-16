import socket
client=socket.socket(socket.AF_INET,socket.SOCK_DGRAM)
while True:
    res=input(">>").strip()
    client.sendto(res.encode("utf-8"),("127.0.0.1",8080))
    date,server_addr=client.recvfrom(1024)
    print(date)
client.close()



