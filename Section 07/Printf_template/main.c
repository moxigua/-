
#include <stdio.h>
#include "stdout_USART.h"

int main(void)
{
    stdout_init();
    printf("printf test.\r\n");
    while (1);
}//主函数结束




