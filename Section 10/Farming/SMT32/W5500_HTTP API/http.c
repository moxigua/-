
#include "http.h"
#include "string.h"
#include "ctype.h"
#include "stdint.h"
#include "rtc/rtc.h"
#include "W5500_functions.h"
#include "socket.h"

// 定义HTTP头的格式
typedef struct HEADER{
  const char *name;
	uint8_t nameLength;
	const char *value;
	uint8_t valueLength;
}Header;

void getHttpString(char *outbuffer, const char *host, 	char *content)		
{
	unsigned char i;
	const char *HTTPMethod = "POST";
	const char *path = "/setCollections";    //必须（/）开头
	Header headers[5];
	char len[10] = {0}; 
		
/* 生成请求行 */
	sprintf(outbuffer,"%s %s",HTTPMethod,path);
	strcat(outbuffer," HTTP/1.1\r\n"); // 注意空格
	/* 生成请求头 */
	headers[0].name = "Host";
	headers[0].nameLength = strlen("Host");
	headers[0].value = host;
	headers[0].valueLength = strlen(host);
	headers[1].name = "Content-type";
	headers[1].nameLength = strlen("Content-type");
	headers[1].value = "application/JSON;charset=UTF-8";
	headers[1].valueLength = strlen("application/JSON;charset=UTF-8");
	headers[2].name = "Connection";
	headers[2].nameLength = strlen("Connection");
	headers[2].value = "keep-alive";
	headers[2].valueLength = strlen("keep-alive");
	for(i=0; i<3; i++)  // 例子中只包含3个头
	{
		if(headers[i].name){
			strcat(outbuffer,headers[i].name);    
			strcat(outbuffer,": ");
			strcat(outbuffer,headers[i].value);    
			strcat(outbuffer,"\r\n");
		}
	}
	sprintf(len,"%d", strlen(content));
	strcat(outbuffer,"Content-Length: ");    //Content-Length
	strcat(outbuffer,len);
	strcat(outbuffer,"\r\n");

	/* 加上一个换行隔开头域与正文 */
	strcat(outbuffer,"\r\n");
	/* 添加请求正文 */
	if(content != NULL)
		strcat(outbuffer,content);
	strcat(outbuffer,"\r\n");  // 注意回车
//  合成HTTP字符串完成
}



int SendHTTPString(uint8_t sn,char* string, uint32_t count)	
{
	while (1) {
		uint32_t size = 0, httpStringSize;
		uint32_t sentsize = 0;
		int ret = 0;
		switch (getSn_SR(sn)) {
		case SOCK_ESTABLISHED:          //是否与服务器建立连接
			if (getSn_IR(sn) & Sn_IR_CON) {	//连接成功标志位是否置位
				setSn_IR(sn, Sn_IR_CON);  //清除连接成功标志位
				sentsize = 0;
				httpStringSize = count;
				/*发送数据，之后数据全部发送完成*/
				while (httpStringSize != sentsize) {
					ret = send(sn, 
								(uint8_t*)string + sentsize, 
								httpStringSize - sentsize);
					if (ret < 0) {
						close(sn);   // 关闭socket n
						return ret;  // 发送错误返回
					}
					sentsize += ret;
				}
			}
			if ((size = getSn_RX_RSR(sn)) > 0) { //是否收到服务器返回的数据
				if (size > 2047) size = 2047; //socket最大缓存是2048个字节
				ret = recv(sn, (uint8_t*)string, size); //接受数据
				if (ret <= 0) return ret; //小于0说明发生错误			 
				if (getSn_RX_RSR(sn) == 0) { 
//如果此时接受缓存区的数据个数是0，说明接受数据完成可以关闭连接了
					osDelay(100);      //需要延时一段时间再来判断是否有数据进来
					if (getSn_RX_RSR(sn) == 0) {
						disconnect(sn);  //断开连接
						close(sn);
						return 0;        //返回成功
					}
				}
			}
			break;
		/*服务请关闭连接*/
		case SOCK_CLOSE_WAIT:
			if ((ret = disconnect(sn)) != SOCK_OK) return ret;
			close(sn);
			return 0;
	     /*socket n初始化完成*/
		case SOCK_INIT:
			if ((ret = connect(sn, server_tcp_ip, 5000)) != SOCK_OK) 
return ret;	//尝试连接服务器，server_tcp_ip通过DNS获得
			break;
		/*socket n处于关闭状态*/
		case SOCK_CLOSED:
			close(sn);
			if ((ret = socket(sn, Sn_MR_TCP, local_port++, 0x00)) != sn) 
return ret; //初始化socket n
			break;
		default:
			break;
		}
	}
}




