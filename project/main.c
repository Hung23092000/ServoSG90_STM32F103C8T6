#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
int Map(int x, int inMin, int inMax, int outMin, int outMax) {//// khai bao ham noi suy tuyen tinh, tinh
  return (x - inMin) * (outMax - outMin) / (inMax - inMin) + outMin;//// do rong xung PWM theo goc quay servo
}
void PWM_Configure(int goc){
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_InitStructure.GPIO_Mode   = GPIO_Mode_AF_PP;// chon che do port: alternate function push-pull
	GPIO_InitStructure.GPIO_Pin    = GPIO_Pin_1;//chon chan Servo
	GPIO_InitStructure.GPIO_Speed  = GPIO_Speed_50MHz;  // chon toc do chan: 50MHz, chi anh huong do manh nhe
	GPIO_Init(GPIOA,&GPIO_InitStructure);//luu lai cau hinh
	TIM_TimeBaseInitTypeDef TIM_InitStucture;
	TIM_InitStucture.TIM_Prescaler   = 72-1;//bo chia tan
	TIM_InitStucture.TIM_Period      = 20000 - 1;//bien dem 
	TIM_InitStucture.TIM_CounterMode = TIM_CounterMode_Up;//dem len 
	TIM_InitStucture.TIM_ClockDivision = 0;
	TIM_Cmd(TIM2,ENABLE);//bat enable cho TIM2
	int T; //goc qui doi 
	T = Map(goc,0,180,500,2500);//tao ra do rong xung
	TIM_OCInitTypeDef TIM_OCInitStructure;// khai bao bien cau truc kenh 1 cua tim2
	TIM_OCInitStructure.TIM_Pulse    = T;//
	TIM_OCInitStructure.TIM_OCMode   = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; // enable kenh 1
	TIM_OCInitStructure.TIM_OCPolarity  = TIM_OCPolarity_High;// chon che do nhan suon dem: tich cuc thuan
	TIM_OC1Init(TIM2,&TIM_OCInitStructure);// khoi tao kenh1
	
}
//tao delay
void delay(int time){
	for(int i = 0; i< time; i++){
		for(int j = 0; j<0x2AFF; j++);
	}
}

int main(){
	while(1){
		for(int i = 0; i<180; i+=20){
			PWM_Configure(i);//quy tu 0 toi 180 do moi lan tang len 20 
			delay(100);
		}
		for (int i = 180; i >= 0; --i) {
         PWM_Configure(i);
         delay(5);
    }
	}
}