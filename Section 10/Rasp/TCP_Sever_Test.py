
import socket
import time
 
HOST = '0.0.0.0' 	# 监听的IP地址
PORT = 8001   		# 监听的端口号
sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)  
sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR,1)
sock.bind((HOST, PORT))	# 执行监听操作
sock.listen(5)    		# 允许5个客户端连接
while True:  
    connection,address = sock.accept()  # 接受客户端的建立连接的请求
    print('连接地址：{}'.format(address))	# 打印建立连接的地址
    try:  
        connection.settimeout(10)       	# 超时时间
        buf = connection.recv(1024)  	# 接受信息
        if buf:  
            connection.send(b'welcome to server!') 	# 接受到信息，发送欢迎字符串
            print('Connection success!') 	# 打印调试信息
    except socket.timeout:  
        print ('time out')
        connection.send(b'no message.')  
connection.close()  	# 断开连接

