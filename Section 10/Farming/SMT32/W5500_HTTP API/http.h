
#ifndef __HTTP_H__
#define __HTTP_H__

#include "cmsis_os.h"

extern void getHttpString(char *outbuffer, const char *host, 	char *content);
extern int SendHTTPString(uint8_t sn,char* string, uint32_t count);


#endif



