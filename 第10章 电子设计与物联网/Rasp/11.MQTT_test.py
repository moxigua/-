
import paho.mqtt.client as mqtt
import time

user = "thingidp@asyfdev|ForBooks|0|MD5"     	# 用户名
pwd = "b6c4d00a1edf4df6060ee5e10fefdcaa"       	# 密码
endpoint = "asyfdev.iot.gz.baidubce.com"     	# 连接地址
port = 1883                	# 1883为服务端口号，如果是安全认证，端口号需要修改为1884
topic = "$iot/ForBooks/user/temperature"     	# 发布消息主题

# 客户端从服务器接收到连接响应时的回调函数。
def on_connect(client, userdata, flags, rc):
    print("Connected with result code "+str(rc))	# 连接成功
    client.subscribe(topic) # 订阅主题

# 从服务器接收发布消息时的回调函数。
def on_message(client, userdata, msg):
    print(msg.topic+" "+str(msg.payload))   # 将接收到的信息打印出来

client = mqtt.Client()				# 创建MQTT实例
client.on_connect = on_connect		# 绑定 连接成功的 回调函数
client.on_message = on_message		# 绑定 消息到达的 回调函数
client.username_pw_set(user, pwd)  # 设置用户名，密码

# 连接MQTT服务器
client.connect(endpoint, port, 60)	
# 发布主题
client.publish(topic, payload="pi message", qos=0) 
# 开启一个线程接受信息
client.loop_start()

while True:
	# 处理其它事情，例如串口、摄像头等等 
	print('->...')
	time.sleep(30)

