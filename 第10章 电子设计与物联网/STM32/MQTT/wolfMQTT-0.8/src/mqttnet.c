

#include "wolfmqtt/mqtt_client.h"  
#include "wolfmqtt/mqttnet.h"
#include "socket.h"      //底层的socket通信  
#include "W5500_MQTT_functions.h"   //自己定义的东西
#include "stm32f10x.h"   //stm32f103头文件
#include "W5500_functions.h"  //需要用到DNS，来解析Iot的域名

/*tcp层连接服务器*/
static int NetConnect(void *context, const char* host, word16 port,
    int timeout_ms)
{
  uint8_t dns_socket = Default_SOCK_DNS,errorCount = 0;
	socket(dns_socket,Sn_MR_TCP,MQTT_Server_Port,Sn_MR_ND);
	DNS(dns_socket,(char *)host,MQTT_Server_IP);   //DNS解析 iot 服务器
	while(1)
	{
		/*开始连接服务器*/
		switch(getSn_SR(MQTT_Client))		// 获取socket 的状态
		{
			case SOCK_INIT:             	// socket处于初始化完成(打开)状态
				if(connect(MQTT_Client, MQTT_Server_IP,port) != SOCK_OK)			// 配置Sn_CR为CONNECT，并向TCP服务器发出连接请求
				{
					osDelay(3000);
					if(errorCount >3)
					{
						return -1;						//连接失败返回-1
					}
					errorCount ++;
				}
				else
				{
					break;
				}
			break;
			case SOCK_ESTABLISHED:			// Socket处于连接建立状态
				if(getSn_IR(MQTT_Client) & Sn_IR_CON)	
         {
						setSn_IR(MQTT_Client, Sn_IR_CON);  // 清理中断标志位
         }
				return  0;                // 连接成功直接返回
			case SOCK_CLOSE_WAIT:				
				disconnect(MQTT_Client);				//断开连接
			break;
			case SOCK_CLOSED:						// Socket处于关闭状态
				close(MQTT_Client);
				socket(MQTT_Client,Sn_MR_TCP,MQTT_Local_Port++,Sn_MR_ND);		// 打开Socket0，并配置为TCP无延时模式，打开一个本地端口
			break;
		} 
	}
}

/*发送数据*/
static int NetWrite(void *context, const byte* buf, int buf_len,
    int timeout_ms)
{
  int ret; 
	uint32_t sentsize = 0;
	while(buf_len != sentsize)
	{
		ret = send(MQTT_Client, (unsigned char *)buf+sentsize, buf_len-sentsize); 
		if(ret < 0) 
		{
			close(MQTT_Client); 
			return ret;
		}
		sentsize += ret;
	}
	return ret;
}

static int NetRead(void *context, byte* buf, int buf_len,
    int timeout_ms)
{
 int ret=0;
	int size = 0;
	while(timeout_ms--){
		if((size =getSn_RX_RSR(MQTT_Client)) > 0){
				if(size > buf_len) size = buf_len; 
					ret = recv(MQTT_Client, buf, size); 
				if(ret < 0)
				{
					return MQTT_CODE_ERROR_NETWORK;
				}
				else if(ret == 0)
				{
					return MQTT_CODE_ERROR_TIMEOUT;
				}
				return ret;
		}
		else{
			osDelay(1);//延时1ms
		}
	}
}

/*断开连接*/
static int NetDisconnect(void *context)
{
	disconnect(MQTT_Client);
  close(MQTT_Client);
	return 0;
}

/* 注册tcp连接，发送，接受，断开的回掉函数 */
int MqttClientNet_Init(MqttNet* net)
{
    if (net) {
        XMEMSET(net, 0, sizeof(MqttNet));
        net->connect = NetConnect;
        net->read = NetRead;
        net->write = NetWrite;
        net->disconnect = NetDisconnect;
			  net->context = NULL;
    }
    return 0;
}

/* Mqtt客户端回到默认状态 */
int MqttClientNet_DeInit(MqttNet* net)
{
	if (net){
        WOLFMQTT_FREE(net->context);
        XMEMSET(net, 0, sizeof(MqttNet));
				
    }
    return 0;
}


