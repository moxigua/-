
#include "stm32f10x.h" //使用STM32标准函数库需要添加此头文件

int main(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	/*使能GPIOE时钟*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
	/*配置GPIOE2为推挽输出*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;   // 第二个管脚定义
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;  // 输出频率为10MHz定义
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  // 推挽输出定义
	GPIO_Init(GPIOE, &GPIO_InitStructure);      // 初始化
	/*点亮LED*/
	GPIO_ResetBits(GPIOE, GPIO_Pin_2);
}//主函数结束



