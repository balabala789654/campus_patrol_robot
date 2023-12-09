#include "main.h"
/**************************
   ____                    
// | __ \                   
// | |  |  _   _  __   _ __  
// | |  | | | / _` | /_ __ \
// | |__| | || (_| | | |__||
// |____/ |_| \__,_| \_____/
                           
                        
***************************/



int main(void)
{ 
	BSP_Init();
//	TIM_Cmd(TIM3,ENABLE);
	GREEN_ACT_ON;
	delay_ms(500);
	GREEN_ACT_OFF;
	delay_ms(500);
	GPIO_ResetBits(GPIOE,GPIO_Pin_5);//¹¤¿Ø»ú
	GPIO_ResetBits(GPIOE,GPIO_Pin_6);//µ×ÅÌ
	GREEN_ACT_ON;
	delay_ms(500);
	GREEN_ACT_OFF;
	startTask();

	while(1);
}




