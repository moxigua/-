
#include "W5500_Functions.h"
#include "W5500_MQTT_Functions.h"
#include "main_threads.h"
#include "socket.h"
#include <wolfssl/wolfcrypt/asn.h>

#define debug

char IsConnnected = 0;

//定义存放MQTT发送和接受的数据数组
byte tx_buf[MAX_BUFFER_SIZE] = {0}, rx_buf[MAX_BUFFER_SIZE] = {0};

/* Tcp连接的一些定义 */
uint8_t MQTT_Server_IP[4] = {192,168,1,111};
uint32_t MQTT_Local_Port = 8086;

byte clean_session = 1;   //发送CONNECT报文时清理标志位
int enable_lwt = 0;   //发送CONNECT报文时遗嘱标志位

word16 keep_alive_sec =DEFAULT_KEEP_ALIVE_SEC;  //保持连接时间
const char* client_id = DEFAULT_CLIENT_ID;      //客户端ID
word32 cmd_timeout_ms = DEFAULT_CMD_TIMEOUT_MS; //获取信息超时时间
byte State = 0;  //是否链接上了主机

//是否正在读取消息
byte IsReadingMessage = 0;


static int mPacketIdLast;



/*判断是否使用TLS*/
//#ifdef ENABLE_MQTT_TLS

#ifdef ENABLE_MQTT_TLS

//是否使用tls验证
int use_tls = 1;
uint32_t MQTT_Server_Port = 1884;

const char GlobalSign[] = {"-----BEGIN CERTIFICATE-----\n\
MIIDXzCCAkegAwIBAgILBAAAAAABIVhTCKIwDQYJKoZIhvcNAQELBQAwTDEgMB4G\n\
A1UECxMXR2xvYmFsU2lnbiBSb290IENBIC0gUjMxEzARBgNVBAoTCkdsb2JhbFNp\n\
Z24xEzARBgNVBAMTCkdsb2JhbFNpZ24wHhcNMDkwMzE4MTAwMDAwWhcNMjkwMzE4\n\
MTAwMDAwWjBMMSAwHgYDVQQLExdHbG9iYWxTaWduIFJvb3QgQ0EgLSBSMzETMBEG\n\
A1UEChMKR2xvYmFsU2lnbjETMBEGA1UEAxMKR2xvYmFsU2lnbjCCASIwDQYJKoZI\n\
hvcNAQEBBQADggEPADCCAQoCggEBAMwldpB5BngiFvXAg7aEyiie/QV2EcWtiHL8\n\
RgJDx7KKnQRfJMsuS+FggkbhUqsMgUdwbN1k0ev1LKMPgj0MK66X17YUhhB5uzsT\n\
gHeMCOFJ0mpiLx9e+pZo34knlTifBtc+ycsmWQ1z3rDI6SYOgxXG71uL0gRgykmm\n\
KPZpO/bLyCiR5Z2KYVc3rHQU3HTgOu5yLy6c+9C7v/U9AOEGM+iCK65TpjoWc4zd\n\
QQ4gOsC0p6Hpsk+QLjJg6VfLuQSSaGjlOCZgdbKfd/+RFO+uIEn8rUAVSNECMWEZ\n\
XriX7613t2Saer9fwRPvm2L7DWzgVGkWqQPabumDk3F2xmmFghcCAwEAAaNCMEAw\n\
DgYDVR0PAQH/BAQDAgEGMA8GA1UdEwEB/wQFMAMBAf8wHQYDVR0OBBYEFI/wS3+o\n\
LkUkrk1Q+mOai97i3Ru8MA0GCSqGSIb3DQEBCwUAA4IBAQBLQNvAUKr+yAzv95ZU\n\
RUm7lgAJQayzE4aGKAczymvmdLm6AC2upArT9fHxD4q/c2dKg8dEe3jgr25sbwMp\n\
jjM5RcOO5LlXbKr8EpbsU8Yt5CRsuZRj+9xTaGdWPoO4zzUhw8lo/s7awlOqzJCK\n\
6fBdRoyV3XpYKBovHd7NADdBj+1EbddTKJd+82cEHhXXipa0095MJ6RMG3NzdvQX\n\
mcIfeg7jLQitChws/zyrVQ4PkX4268NXSb7hLi18YIvDQVETI53O9zJrlAGomecs\n\
Mx86OyXShkDOOyyGeMlhLxS67ttVb9+E7gUJTb0o2HLO02JQZR7rkpeDMdmztcpH\n\
WD9f\n\
-----END CERTIFICATE-----"};


const char client_cert[] = {"-----BEGIN CERTIFICATE-----\n\
MIIEnTCCA4WgAwIBAgICM+QwDQYJKoZIhvcNAQELBQAwbjELMAkGA1UEBhMCQ04x\n\
IzAhBgNVBAMMGm9ubGluZS5pb3RkZXZpY2UuYmFpZHUuY29tMQ4wDAYDVQQKDAVC\n\
QUlEVTEMMAoGA1UECwwDQkNFMRwwGgYJKoZIhvcNAQkBFg1pb3RAYmFpZHUuY29t\n\
MB4XDTIwMTAwNTA2MjAzMFoXDTMwMTAwMzA2MjAzMFowRjEOMAwGA1UECgwFQmFp\n\
ZHUxCzAJBgNVBAYTAkNOMRkwFwYDVQQDDBBhc3lmZGV2L3NzbF90ZXN0MQwwCgYD\n\
VQQLDANCQ0UwggEiMA0GCSqGSIb3DQEBAQUAA4IBDwAwggEKAoIBAQCfF8mmbeG3\n\
yQAhmaQC92CW7wIWBtTLuh8ryqZmrRa3r8aJsZXz9U9NaBw5ECw4WkRfq85ERf9k\n\
yeFzazmbdijX+JACDDRo7kB/JXpHWbv/vOD1z2IfeG1xphnaxVkHLErUHX7PMHYg\n\
7L1JczwrPrIeA1cB6Bs2ZK0Hv5s35rKmo9ebP07e5NheCQtgkRaX9to2T6t11tjO\n\
PtL9SSYHVbzuWIMGGx37kU27UET4HG9WvK5yjvkh9w/Q4RyMAJkylbrgS7y6RbbB\n\
bGKoTeyyZD9ylOo1hnKgEADkghaMTDoxvJdx2mkPSvpECDE1aNq4+fYo6nxi8Yac\n\
kfVdThNBk8sbAgMBAAGjggFrMIIBZzAdBgNVHQ4EFgQUeODpejG8GJ26NgwYf8pa\n\
+HBt11YwDAYDVR0TAQH/BAIwADAfBgNVHSMEGDAWgBSz6xPEME8AwqfrFQ7M2Csn\n\
dhNkVzCBowYDVR0fBIGbMIGYMIGVoIGSoIGPhoGMaHR0cDovL3BraWlvdi5iYWlk\n\
dWJjZS5jb20vdjEvcGtpL2NybD9jbWQ9Y3JsJmZvcm1hdD1QRU0maXNzdWVyPUM9\n\
Q04sQ049b25saW5lLmlvdGRldmljZS5iYWlkdS5jb20sRU1BSUxBRERSRVNTPWlv\n\
dEBiYWlkdS5jb20sTz1CQUlEVSxPVT1CQ0UwQgYIKwYBBQUHAQEENjA0MDIGCCsG\n\
AQUFBzABhiZodHRwOi8vcGtpaW92LmJhaWR1YmNlLmNvbS92MS9wa2kvb2NzcDAO\n\
BgNVHQ8BAf8EBAMCA/gwHQYDVR0lBBYwFAYIKwYBBQUHAwIGCCsGAQUFBwMEMA0G\n\
CSqGSIb3DQEBCwUAA4IBAQASGbFMkl3t25X6TxTjXK/KEuQb4QcQ7oRApmdmAbLQ\n\
k4zbD8CawfgZA9n0btISgdxMlPBCbMeSn/CHRcwMFN5qYpMXg9Ba0VerfNgMF0lv\n\
PnHGTmbeKSLzLRONFkxRvhUbY9jn1LvLzd5Ckx1VlThNLBnnkptCaefJuykm/b8Q\n\
moH2nuSJA/EzsmkQIS3NDypYWjr5d7zBMQV8ican4fZ3NCz6Zw55Z6xc/RuexhoK\n\
hQg9HP177bUVydisKzq0AclKDp5S+e03fhjS8CIh/WXQkg9TQXNgEa3v88KZQ2hw\n\
+ihWB6eWwvCyCEcQLnH8g6Q19E/PXtvb0t1wNKaLpf4m\n\
-----END CERTIFICATE-----"};


const char client_private_key[] = {"-----BEGIN RSA PRIVATE KEY-----\n\
MIIEogIBAAKCAQEAnxfJpm3ht8kAIZmkAvdglu8CFgbUy7ofK8qmZq0Wt6/GibGV\n\
8/VPTWgcORAsOFpEX6vOREX/ZMnhc2s5m3Yo1/iQAgw0aO5AfyV6R1m7/7zg9c9i\n\
H3htcaYZ2sVZByxK1B1+zzB2IOy9SXM8Kz6yHgNXAegbNmStB7+bN+aypqPXmz9O\n\
3uTYXgkLYJEWl/baNk+rddbYzj7S/UkmB1W87liDBhsd+5FNu1BE+BxvVryuco75\n\
IfcP0OEcjACZMpW64Eu8ukW2wWxiqE3ssmQ/cpTqNYZyoBAA5IIWjEw6MbyXcdpp\n\
D0r6RAgxNWjauPn2KOp8YvGGnJH1XU4TQZPLGwIDAQABAoIBAG6gl/RHDGJEPvce\n\
Y1I8usm5wXWXonlYHzzQWg2j5twsOp6dAmz19AMbliSH/nfhLRJp4bDQC9wGFeOw\n\
eAqGi7hCe8Eww3ED242Ofg0HdRdMPBIx90Vy9IfGvMvH1zSJtC7zo65nQH18odNl\n\
cj2QB0Ofhu0ofWPBdiChOEoS/b+prCnJa8Zd92fZqN13GugnUP1qeYENp2kwLHOO\n\
ruPP9sEb4uXzIkULnHdw1f+2ygCEufLVRybHVIlBNSLzYtTzGawl3HxN/W2zGCxW\n\
p4YRjEtYZDxVOztxtPRESQUKih7wWyuikCWTP3CKIcd4UEgfzeChUryI+JpJcOyQ\n\
GirCdGECgYEA4z61r+576MhsipTuMHgYRNAUr2VLJCW7rD7CQxP7CDBkuhMuJHg0\n\
LO1fhwjZRJtL3svxBqnWbhD0jYE1dQcMPwkmc8iGUxmb/OMfbpm2wwhkAnp3umBu\n\
rwV08i5fRY8a7G+HBKe/H1RRVFmMkYFWAm0p8DRGo/NNgwAkPp4i768CgYEAszli\n\
vTUReRFY6NYQAscfiap5gIcOkWjP94OSFdS79sZuW8HXBTJN3NDU6oMK7//p6QWa\n\
DHd2PwN1eiTsUcJ6rw26TVBVB4IPxlOlyUbY7fkNEWiKJkexC+lKaQzPxZju3OR0\n\
FlIXRoYNROK5e0FkVBUB2UnnGTMtJlv9PCQwqlUCgYA5GD5p749uUz0CLJ0TzHi8\n\
gVNRJjAU6u4WPsZsOA7wZchJABytyQ3C4/kGYnDETghn9/Gc7PsVOGlZTEd0Gcs3\n\
40KoAI/PPTX1+dKh5TkIE4mdHcwLJcNpcgOJ/aCuR2BKW1MfFk5svrCYcs9gl5BW\n\
l7rbwSamdrDY4yY4HV9FvQKBgH9GmH0/i5oLnLU1F4exew8btk91Gf5fjZj7oanJ\n\
OnbEcijNXnJ9R9gxoZKs3v6TIbzu0nhZ+epD/ZBxWjWxw8g/Gne/PbMoRSWVz51B\n\
p9pPzFz5fWoz7WJFGSfDBmYP9kl+H4aeY5McrZ2m2QvT43jmtjpmBizaMA0WtJ6a\n\
IMlFAoGAOKrEMAn6PfOb0dETlsvhdBZCqnxB178gHGMkFWE6MM6QR3Eluu5XzdAn\n\
ZP4fH4NmN3AsIDpI/zUHfPzxbl/zGOklBlYoiT8Xl3nSKvLbz9V1ZHfKv5Zut8jB\n\
S1ThR22Wbp9fUKlfCEaywxpDKifSGGehWniGZsswZ7uaGpcTy9k=\n\
-----END RSA PRIVATE KEY-----"};

#else
//是否使用tls验证
int use_tls = 0;
uint32_t MQTT_Server_Port = 1883;
#endif

//*****************************************************************************
//
//!  MQTT的一些私有函数
//
//*****************************************************************************
static word16 mqttclient_get_packetid(void)
{
    mPacketIdLast = (mPacketIdLast >= MAX_PACKET_ID) ?
        1 : mPacketIdLast + 1;
    return (word16)mPacketIdLast;
}



/* 互锁 */

osSemaphoreDef (MQTT_Semaphore);                               // MQTT object
#define Lock

void MQTT_Lock(uint32_t ms)
{
	osSemaphoreWait (MQTT_Semaphore, ms);           // 
}

void MQTT_Unlock(void)
{
	if(osSemaphoreRelease(MQTT_Semaphore) != osOK)
		;
	//	printf("\r\n\r\n\r\n\r\n\r\n\r\n\r\n.......................................................................");
	
}

int Init_MQTT_Lock (void) {

  MQTT_Semaphore = osSemaphoreCreate (osSemaphore(MQTT_Semaphore), 1);
  if (!MQTT_Semaphore) {  // Semaphore object not created, handle failure
	
		return 	-1;
		}
		printf("init lock ok.\r\n");
  return(0);
}


#ifdef  ENABLE_MQTT_TLS

static int mqttclient_tls_verify_cb(int preverify, WOLFSSL_X509_STORE_CTX* store)
{
#ifdef debug
		char buffer[WOLFSSL_MAX_ERROR_SZ];
    printf("MQTT TLS Verify Callback: PreVerify %d, Error %d (%s)\r\n", preverify,
        store->error, wolfSSL_ERR_error_string(store->error, buffer));
    printf("  Subject's domain name is %s\r\n", store->domain);

    printf("  have error.disconnect!!!\r\n");
#endif
	//return 0  不允许错误
	//return 1  运行错误，继续
    return 1;  
}

/* Use this callback to setup TLS certificates and verify callbacks */
static int mqttclient_tls_cb(MqttClient* client)
{
    int rc = SSL_FAILURE;
    (void)client;   /*无效参数*/
    //使用TLSv1.2建立连接
    client->tls.ctx = wolfSSL_CTX_new(wolfTLSv1_2_client_method());
		wolfSSL_Debugging_ON();
    if (client->tls.ctx) {
			//配置WOLFSSL_CTX
			wolfSSL_CTX_set_verify(client->tls.ctx, SSL_VERIFY_PEER, mqttclient_tls_verify_cb);
			//加载证书
			rc = wolfSSL_CTX_load_verify_buffer(client->tls.ctx, GlobalSign, strlen(GlobalSign) , SSL_FILETYPE_PEM);
			printf("server ca:%d", rc);
			rc = wolfSSL_CTX_use_PrivateKey_buffer(client->tls.ctx, client_private_key, strlen(client_private_key), SSL_FILETYPE_PEM);
			printf("client private key ca:%d", rc);
			rc = wolfSSL_CTX_use_certificate_buffer(client->tls.ctx, client_cert, strlen(client_cert), SSL_FILETYPE_PEM);
			printf("clinet ca ca:%d", rc);
			
			rc = SSL_SUCCESS;
    }
#ifdef Debug
    printf("MQTT TLS Setup (%d)\r\n", rc);
#endif 
    return rc;
}
#else

/*没有TLS验证，所以一直返回0*/
static int mqttclient_tls_cb(MqttClient* client)
{
    (void)client;
    return 0;
}
#endif /* ENABLE_MQTT_TLS */


//*****************************************************************************
//
//!  MQTT  消息处理的回掉函数
//
//*****************************************************************************
static int mqttclient_message_cb(MqttClient *client, MqttMessage *msg,
    byte msg_new, byte msg_done)
{
    byte buf[PRINT_BUFFER_SIZE+1];
    word32 len;

    (void)client; /*这个参数在这里没有用到 */

		if (msg_new) {  //如果这个消息不是重发的
        len = msg->topic_name_len;
        if (len > PRINT_BUFFER_SIZE) {
            len = PRINT_BUFFER_SIZE;
        }
        XMEMCPY(buf, msg->topic_name, len);
        buf[len] = '\0';
        /*打印Topic主题信息 */
        printf("MQTT Message: Topic %s, Qos %d, Len %u\r\n",
            buf, msg->qos, msg->total_len);
    }
    /* 打印Topic消息体 */
    len = msg->buffer_len;
    if (len > PRINT_BUFFER_SIZE) {
        len = PRINT_BUFFER_SIZE;
    }
    XMEMCPY(buf, msg->buffer, len);
    buf[len] = '\0'; 
    printf("Payload (%d - %d): %s\r\n",
        msg->buffer_pos, msg->buffer_pos + len, buf);
    if (msg_done) {
        printf("MQTT Message: Done\r\n");
    }
    /* Return negative to terminate publish processing */
    return MQTT_CODE_SUCCESS;
}


//*****************************************************************************
//
//!  MQTT 的公开函数
//
//*****************************************************************************

/**                
 * @ingroup MQTT配置函数
 * @brief 	MQTT客户端的初始化
 * @details 
 * @param void
 */
int MQTTClient_Init(void)
{
	int rc;
	uint8_t memsize[2][8] = {{2,2,2,2,2,2,2,2},{2,2,2,2,2,2,2,2}};	
	//配置W5500中socket的缓存区的大小
	if(ctlwizchip(CW_INIT_WIZCHIP,(void*)memsize) == -1)
	{
		 printf("WIZCHIP Initialized fail.\r\n");
		 return -1;
	}	
//设置心跳包
setsockopt(MQTT_Client,SO_KEEPALIVEAUTO,(void *)2);	
	/* Initialize Network */
	rc = MqttClientNet_Init(&net);
#ifdef Debug
	printf("MQTT Net Init: %s (%d)\r\n",
			MqttClient_ReturnCodeToString(rc), rc);
#endif		
	if (rc != MQTT_CODE_SUCCESS) {
			goto exit;
	}
/* Initialize MqttClient structure */
    //tx_buf = (byte*)WOLFMQTT_MALLOC(MAX_BUFFER_SIZE);
    //rx_buf = (byte*)WOLFMQTT_MALLOC(MAX_BUFFER_SIZE);
    rc = MqttClient_Init(&client, &net, mqttclient_message_cb,
        tx_buf, MAX_BUFFER_SIZE, rx_buf, MAX_BUFFER_SIZE,
        cmd_timeout_ms);
#ifdef Debug	
    printf("MQTT Init: %s (%d)\r\n",
        MqttClient_ReturnCodeToString(rc), rc);
#endif	
    if (rc != MQTT_CODE_SUCCESS) {
        goto exit;
    }
		return 0;
exit:
		State = 0;
    /* Cleanup network */
    MqttClientNet_DeInit(&net);
    return -1;
}
/**                
 * @ingroup MQTT使用函数
 * @brief 	MQTT客户端链接服务器
 * @details 
 * @param host->  MQTT服务器的域名
 */
int MQTTClient_Connect(char *host)
{
	int rc;
	//一些配置
	const char* topicName = DEFAULT_SUBSCRIBE_TOPIC;
	const char* username = DEFAULT_USERNAME;
	const char* password = DEFAULT_PASSWORD;
	MqttQoS	qos = DEFAULT_MQTT_QOS;
  /* Define connect parameters */
	MqttConnect connect;
	MqttMessage lwt_msg;
	/* Connect to broker */
    rc = MqttClient_NetConnect(&client, host, MQTT_Server_Port,
        DEFAULT_CON_TIMEOUT_MS, use_tls, mqttclient_tls_cb);
#ifdef Debug		
    printf("MQTT Socket Connect: %s (%d)\r\n", MqttClient_ReturnCodeToString(rc), rc);
#endif		
    if (rc != MQTT_CODE_SUCCESS) {
        goto exit;
    }
		if(rc == MQTT_CODE_SUCCESS)
		{
		    XMEMSET(&connect, 0, sizeof(MqttConnect));
				connect.keep_alive_sec = DEFAULT_KEEP_ALIVE_SEC;
				connect.clean_session = clean_session;
				connect.client_id = client_id;
				/* Last will and testament sent by broker to subscribers
						of topic when broker connection is lost */
				XMEMSET(&lwt_msg, 0, sizeof(lwt_msg));
				connect.lwt_msg = &lwt_msg;
				connect.enable_lwt = enable_lwt;
				if (enable_lwt) {
						/* Send client id in LWT payload */
						lwt_msg.qos = qos;
						lwt_msg.retain = 0;
						lwt_msg.topic_name = topicName;
						lwt_msg.buffer = (byte*)client_id;
						lwt_msg.total_len = (word16)XSTRLEN(client_id);
				}
				/* Optional authentication */
				connect.username = username;
				connect.password = password;

				/* Send Connect and wait for Connect Ack */
				rc = MqttClient_Connect(&client, &connect);
#ifdef Debug				
				printf("MQTT Connect: %s (%d)\r\n",
						MqttClient_ReturnCodeToString(rc), rc);
#endif				
				if (rc == MQTT_CODE_SUCCESS) {
					State = 1;
/* Validate Connect Ack info */
#ifdef Debug					
					printf("MQTT Connect Ack: Return Code %u, Session Present %d\r\n",
						connect.ack.return_code,
						(connect.ack.flags & MQTT_CONNECT_ACK_FLAG_SESSION_PRESENT) ?
								1 : 0
				);
#endif
					return 0;
		}
	}
		
exit:
		State = 0;
    /* Cleanup network */
    MqttClientNet_DeInit(&net);
    return -1;
}





/**                
 * @ingroup MQTT使用函数
 * @brief 	MQTT客户端链接服务器
 * @details 
 * @param host->  MQTT服务器的域名
 */
int MQTTClient_Connect_With_Name(char *host, char * user, char * pwd)
{
	int rc;
	//一些配置
	const char* topicName = DEFAULT_SUBSCRIBE_TOPIC;
	const char* username = user;
	const char* password = pwd;
	MqttQoS	qos = DEFAULT_MQTT_QOS;
  /* Define connect parameters */
	MqttConnect connect;
	MqttMessage lwt_msg;
	/* Connect to broker */
    rc = MqttClient_NetConnect(&client, host, MQTT_Server_Port,
        DEFAULT_CON_TIMEOUT_MS, use_tls, mqttclient_tls_cb);
#ifdef Debug		
    printf("MQTT Socket Connect: %s (%d)\r\n", MqttClient_ReturnCodeToString(rc), rc);
#endif		
    if (rc != MQTT_CODE_SUCCESS) {
        goto exit;
    }
		if(rc == MQTT_CODE_SUCCESS)
		{
		    XMEMSET(&connect, 0, sizeof(MqttConnect));
				connect.keep_alive_sec = DEFAULT_KEEP_ALIVE_SEC;
				connect.clean_session = clean_session;
				connect.client_id = client_id;
				/* Last will and testament sent by broker to subscribers
						of topic when broker connection is lost */
				XMEMSET(&lwt_msg, 0, sizeof(lwt_msg));
				connect.lwt_msg = &lwt_msg;
				connect.enable_lwt = enable_lwt;
				if (enable_lwt) {
						/* Send client id in LWT payload */
						lwt_msg.qos = qos;
						lwt_msg.retain = 0;
						lwt_msg.topic_name = topicName;
						lwt_msg.buffer = (byte*)client_id;
						lwt_msg.total_len = (word16)XSTRLEN(client_id);
				}
				/* Optional authentication */
				connect.username = username;
				connect.password = password;

				/* Send Connect and wait for Connect Ack */
				rc = MqttClient_Connect(&client, &connect);
#ifdef Debug				
				printf("MQTT Connect: %s (%d)\r\n",
						MqttClient_ReturnCodeToString(rc), rc);
#endif				
				if (rc == MQTT_CODE_SUCCESS) {
					State = 1;
/* Validate Connect Ack info */
#ifdef Debug					
					printf("MQTT Connect Ack: Return Code %u, Session Present %d\r\n",
						connect.ack.return_code,
						(connect.ack.flags & MQTT_CONNECT_ACK_FLAG_SESSION_PRESENT) ?
								1 : 0
				);
#endif
					return 0;
		}
	}
		
exit:
		State = 0;
    /* Cleanup network */
    MqttClientNet_DeInit(&net);
    return -1;
}



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
int MQTTClient_Publish(char *topicName,MqttQoS	qos,char *message,uint32_t length)
{
		int rc;
		MqttPublish publish;

		/* Publish Topic */
		XMEMSET(&publish, 0, sizeof(MqttPublish));
	  publish.retain = 0;        //不保留PUBLISH的消息
		publish.qos = qos;
	  publish.duplicate = 0;     //是最新的消息，不是重发的消息
		publish.topic_name = topicName;
		publish.packet_id = mqttclient_get_packetid();
		publish.buffer = (byte*)message;
		//publish.total_len = (word16)XSTRLEN(TEST_MESSAGE);
		publish.total_len = length;
#ifdef Lock
		MQTT_Lock(osWaitForever);
#endif 	
		rc = MqttClient_Publish(&client, &publish);
#ifdef Lock
		MQTT_Unlock();
#endif
#ifdef Debug
		printf("MQTT Publish: Topic %s, %s (%d)\r\n",
				publish.topic_name, MqttClient_ReturnCodeToString(rc), rc);
#endif
		if (rc != MQTT_CODE_SUCCESS) {
				goto exit;
		}
		return 0;
exit:
		State = 0;
    /* Cleanup network */
    MqttClientNet_DeInit(&net);
    return -1;			
}


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
int MQTTclient_Subscribe(char *topicName,MqttQoS	qos)
{
		int rc;
		int i ;
		MqttSubscribe subscribe;
		MqttTopic topics[1], *topic;
		/* Build list of topics */
		topics[0].topic_filter = topicName;
		topics[0].qos = qos;
		/* Subscribe Topic */
		XMEMSET(&subscribe, 0, sizeof(MqttSubscribe));
		subscribe.packet_id = mqttclient_get_packetid();
		subscribe.topic_count = sizeof(topics)/sizeof(MqttTopic);
		subscribe.topics = topics;
#ifdef Lock
		MQTT_Lock(osWaitForever);
#endif 	
		rc = MqttClient_Subscribe(&client, &subscribe);
#ifdef Lock
		MQTT_Unlock();
#endif
		printf("MQTT Subscribe: %s (%d)\r\n",
				MqttClient_ReturnCodeToString(rc), rc);
		if (rc != MQTT_CODE_SUCCESS) {
				goto exit;
		}
		for (i = 0; i < subscribe.topic_count; i++) {
				topic = &subscribe.topics[i];
				printf("  Topic %s, Qos %u, Return Code %u\r\n",
						topic->topic_filter, topic->qos, topic->return_code);
		}
		return 0;
exit:
		State = 0;
    /* Cleanup network */
    MqttClientNet_DeInit(&net);
    return -1;					
}


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
int MQTTclient_Unsubscribe_Topics(char *topicName,MqttQoS	qos)
{
		int rc;
		MqttUnsubscribe unsubscribe;
		MqttTopic topics[1];
		/* Build list of topics */
		topics[0].topic_filter = topicName;
		topics[0].qos = qos;
		/* Unsubscribe Topics */
		XMEMSET(&unsubscribe, 0, sizeof(MqttUnsubscribe));
		unsubscribe.packet_id = mqttclient_get_packetid();
		unsubscribe.topic_count = sizeof(topics)/sizeof(MqttTopic);
		unsubscribe.topics = topics;
#ifdef Lock
		MQTT_Lock(osWaitForever);
#endif 	
		rc = MqttClient_Unsubscribe(&client, &unsubscribe);
#ifdef Lock
		MQTT_Unlock();
#endif
		printf("MQTT Unsubscribe: %s (%d)\r\n",
				MqttClient_ReturnCodeToString(rc), rc);
		if (rc != MQTT_CODE_SUCCESS) {
				goto exit;
		}
		return 0;
exit:
		State = 0;
    /* Cleanup network */
    MqttClientNet_DeInit(&net);
    return -1;					
}

/**                
 * @ingroup MQTT使用函数
 * @brief 	MQTT读取消息的Loop
 * @details 
 * @param topicName->  需要的去的那个Topic的消息
 * @param MqttQoS->  消息服务质量
			    MQTT_QOS_0 = 0,  At most once delivery 
					MQTT_QOS_1 = 1,  At least once delivery 
					MQTT_QOS_2 = 2,  Exactly once delivery 
					MQTT_QOS_3 = 3,  Reserved - must not be used 
 */
int MQTTclient_ReadLoop(void)
{
		int rc;
	  uint8_t readCount = 0 ,temp =1;
		/* Read Loop */
#ifdef Debug
		printf("MQTT Waiting for message...\r\n");
#endif
		while (IsConnnected) {
				/* Try and read packet */
				IsReadingMessage = 1;	
				if(IsConnnected)
					rc = MqttClient_WaitMessage(&client, cmd_timeout_ms);				
				IsReadingMessage = 0;
				if (rc == MQTT_CODE_ERROR_TIMEOUT) {
					//Ping
					readCount ++;
					temp = 1;
					if(cmd_timeout_ms/1000)
					{
						  temp =  cmd_timeout_ms/1000;
					}
					if(readCount > DEFAULT_KEEP_ALIVE_SEC/2/temp)
					{
						readCount = 0;
#ifdef Lock
				MQTT_Lock(osWaitForever);
#endif 	
						rc = MqttClient_Ping(&client);
#ifdef Lock
				MQTT_Unlock();
#endif
						if (rc != MQTT_CODE_SUCCESS) {
#ifdef Debug									
								printf("MQTT Ping Keep Alive Error: %s (%d)\r\n",
										MqttClient_ReturnCodeToString(rc), rc);
#endif									
						}
						else
						{
#ifdef Debug
							printf("Ping....\r\n");
#endif
						}
					}
				}
				else if (rc != MQTT_CODE_SUCCESS) {
						/* There was an error */
						printf("MQTT Message Wait: %s (%d)\r\n",
								MqttClient_ReturnCodeToString(rc), rc);
						break;
				}
		}
		/* Check for error */
		if (rc != MQTT_CODE_SUCCESS) {
				goto exit;
		}
		return 0;
exit:
    /* Cleanup network */
		IsConnnected = false;
    MqttClientNet_DeInit(&net);
    return -1;					
}
/**                
 * @ingroup MQTT使用函数
 * @brief 	MQTT客户端断开链接
 * @details 
 * @param void
 */	
 int MQTTclient_Disconnect(void)
{
		int rc;
		State = 0;
	
		/* Disconnect */
#ifdef Lock
		MQTT_Lock(osWaitForever);
#endif
		rc = MqttClient_Disconnect(&client);
		printf("MQTT Disconnect: %s (%d)\r\n",
				MqttClient_ReturnCodeToString(rc), rc);
#ifdef Lock
				MQTT_Unlock();
#endif
	    /* Cleanup network */
    MqttClientNet_DeInit(&net);
    return 0;					
}



