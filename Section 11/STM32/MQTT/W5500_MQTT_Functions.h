


#ifndef __W5500_MQTT_Functions_H__
#define __W5500_MQTT_Functions_H__


#include "wolfmqtt/mqtt_client.h"
#include "wolfmqtt/mqttnet.h"
#include "cmsis_os.h"

#define Debug

#define MQTT_Client 4 
//定义MQTT最大传输数据的大小
#define MAX_BUFFER_SIZE   1024 
#define PRINT_BUFFER_SIZE 1024
//设置超时时间
#define DEFAULT_CMD_TIMEOUT_MS  600   // 3000
#define DEFAULT_CON_TIMEOUT_MS  50000    // 5000

/* Configuration */
#define MAX_PACKET_ID           ((1 << 16) - 4)
#define DEFAULT_MQTT_QOS    MQTT_QOS_0
#define DEFAULT_KEEP_ALIVE_SEC   60
#define DEFAULT_CLIENT_ID   "Iot_Hub_Client2"

#define DEFAULT_USERNAME "thingidp@asyfdev|ForBooks|0|MD5"   //用户名 -> 根据自己创建的百度云的Iot Hub实例决定的
#define DEFAULT_PASSWORD "b6c4d00a1edf4df6060ee5e10fefdcaa"   //密码  -> 根据自己创建的百度云的Iot Hub实例决定的
#define DEFAULT_HOST "asyfdev.iot.gz.baidubce.com"  //默认服务器的域名
#define DEFAULT_SUBSCRIBE_TOPIC "$iot/ForBooks/user/led"              //订阅主题
#define DEFAULT_PUBLIC_TOPIC    "$iot/ForBooks/user/temperature"      //发布主题  

#define TEST_MESSAGE   "test"

extern char IsConnnected; 


extern word16 keep_alive_sec;

//定义MQTT客户端和链接的结构体
static MqttClient client;
static MqttNet net;

//为1表示连接上了服务器，0为没有连接上服务器
extern byte State;
//是否正在读取消息
extern byte IsReadingMessage;

extern  uint32_t MQTT_Server_Port;
extern uint8_t MQTT_Server_IP[4];
extern uint32_t MQTT_Server_Port;
extern uint32_t MQTT_Local_Port;
extern char *MQTT_HostName;

/**                
 * @ingroup MQTT配置函数
 * @brief 	MQTT客户端的初始化
 * @details 
 * @param void
 */
extern int MQTTClient_Init(void);
/**                
 * @ingroup MQTT使用函数
 * @brief 	MQTT客户端链接服务器
 * @details 
 * @param host->  MQTT服务器的域名
 */
extern int MQTTClient_Connect(char *host);
/**                
 * @ingroup MQTT使用函数
 * @brief 	MQTT客户端发布信息
 * @details 
 * @param topicName->  发布信息的 topic
 * @param MqttQoS->  消息发布服务质量
			    MQTT_QOS_0 = 0,  At most once delivery 
					MQTT_QOS_1 = 1,  At least once delivery 
					MQTT_QOS_2 = 2,  Exactly once delivery 
					MQTT_QOS_3 = 3,  Reserved - must not be used 
 * @param message->  消息的内容
 * @param topicName->  消息的长度
 */
extern int MQTTClient_Publish(char *topicName,MqttQoS	qos,char *message,uint32_t length);
/**                
 * @ingroup MQTT使用函数
 * @brief 	MQTT客户端消息的订阅
 * @details 
 * @param topicName->  订阅消息的 topic
 * @param MqttQoS->  消息发布服务质量
			    MQTT_QOS_0 = 0,  At most once delivery 
					MQTT_QOS_1 = 1,  At least once delivery 
					MQTT_QOS_2 = 2,  Exactly once delivery 
					MQTT_QOS_3 = 3,  Reserved - must not be used 
 */
extern int MQTTclient_Subscribe(char *topicName,MqttQoS	qos);
/**                
 * @ingroup MQTT使用函数
 * @brief 	MQTT客户端消息的取消订阅
 * @details 
 * @param topicName->  取消订阅消息的 topic
 * @param MqttQoS->  消息发布服务质量
			    MQTT_QOS_0 = 0,  At most once delivery 
					MQTT_QOS_1 = 1,  At least once delivery 
					MQTT_QOS_2 = 2,  Exactly once delivery 
					MQTT_QOS_3 = 3,  Reserved - must not be used 
 */
extern int MQTTclient_Unsubscribe_Topics(char *topicName,MqttQoS	qos);
/**                
 * @ingroup MQTT使用函数
 * @brief 	MQTT读取消息的Loop
 * @details 
 * @param void
 */
extern int MQTTclient_ReadLoop(void);
/**                
 * @ingroup MQTT使用函数
 * @brief 	MQTT客户端断开链接
 * @details 
 * @param  void
 */
extern int MQTTclient_Disconnect(void);

/**                
 * @ingroup MQTT使用函数
 * @brief 	MQTT客户端链接服务器
 * @details 
 * @param host->  MQTT服务器的域名
 * @param user->  用户名
 * @param pwd->  密码
 */
extern int MQTTClient_Connect_With_Name(char *host, char * user, char * pwd);

static osSemaphoreId MQTT_Semaphore; 
extern int Init_MQTT_Lock (void);

#endif

