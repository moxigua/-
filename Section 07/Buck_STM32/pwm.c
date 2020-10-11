#include "stm32f10x.h" //使用STM32标准函数库需要添加此头文件

int PWM_Period = 179;

void MY_TIM3_Init(u16 arr,u16 psc){
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;   // 初始化结构体
	/*  1.分配时钟  */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	/* 2.初始化定时器相关配置  */
	TIM_TimeBaseStructure.TIM_Period = arr;
	TIM_TimeBaseStructure.TIM_Prescaler = psc;
	/* 这个TIM_ClockDivision 是设置与进行输入捕获相关的分频 */
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //向上计数
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseStructure);
	/* 3.打开定时器  */
	TIM_Cmd(TIM3,ENABLE);
}

void MY_TIM3_GPIO_Init(void){
	GPIO_InitTypeDef 	GPIO_InitStructure;
	/* 1.开启AFIO时钟 */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	/* 2. 根据当前的重映像的模式 配置时钟 和 初始化相关引脚 */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB,ENABLE); 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  // 管脚设置为复用推挽
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; // 输出频率最高50MHz
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6; // PA6、PA7
	GPIO_Init(GPIOA,&GPIO_InitStructure);
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1; // PB0、PB1
//	GPIO_Init(GPIOB,&GPIO_InitStructure);  // 初始化IO口
}

void MY_TIM3_PWM_Init(u16 arr,u16 psc){
	TIM_OCInitTypeDef TIM_OCInitstrcuture; // 初始化结构体
	MY_TIM3_Init(arr,psc);                    // 初始化定时器3
	MY_TIM3_GPIO_Init();                      // 对PWM输出管脚进行初始化
  // 在向上模式中，PWM模式1中有效电平在CNT<CCR时;PWM模式2正好相反
	TIM_OCInitstrcuture.TIM_OCMode = TIM_OCMode_PWM1;  // PWM输出模式进行设置
	TIM_OCInitstrcuture.TIM_OutputState = TIM_OutputState_Enable;//比较输出使能
	TIM_OCInitstrcuture.TIM_OCPolarity = TIM_OCPolarity_High;     //有效电平为高
	TIM_OC1Init(TIM3,&TIM_OCInitstrcuture);  // 设置通道1输出
	//TIM_OC2Init(TIM3,&TIM_OCInitstrcuture);  // 设置通道2输出
	//TIM_OC3Init(TIM3,&TIM_OCInitstrcuture);  // 设置通道3输出
	//TIM_OC4Init(TIM3,&TIM_OCInitstrcuture);  // 设置通道4输出

	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable); //使能预装载寄存器
}

void MY_PWM_Init(void)           
{ 
	// PWM与定时器初始化
	MY_TIM3_PWM_Init(PWM_Period,0);  			// 设置频率为200K
	TIM_SetCompare1(TIM3, 0.05*PWM_Period);   	// 设置初始的CCR3 ，对应引脚为PB0
}

void Update_PWM(int d)
{ 
	// 更新占空比
	TIM_SetCompare1(TIM3,d);   // 设置CCR3 ，对应引脚为PB0
}

void Stop_PWM()
{
	TIM_Cmd(TIM3, DISABLE);
	TIM_CCxCmd(TIM3, TIM_Channel_1, TIM_CCx_Disable);
}

void Restart_PWM(int d)
{
	TIM_Cmd(TIM3, ENABLE);
	TIM_CCxCmd(TIM3, TIM_Channel_1, TIM_CCx_Enable);
	Update_PWM(d);
}

