#include "include.h"
float t1;
float speed;
void Information(void *pvParameters)
{
		
		u8 string[100] = {0};	
		u8 strong[100] = {0};
    u16 result;
		vTaskSuspend(NULL);
  while(1)
  {
		//printf("%d",100);
		Z_PWM_S_Output(BACKWARD,10,10);

		result=lvbo(0xeb,0x82);    //A0 A1Ϊ���������Զ�          
    if(result >= 0x8000 && result <= 0xffff)
    {
			result = 0xffff - result;   //��ֵΪ��ȡ����ֵ��ʹ��A0 A1�����Ӷ���
    }
    else if(result >= 0xffff)
    {
      result = 0;
    }
//--------------------------------------�������            
    t1=4.096*2*result/65535;     //ת���ɵ�ѹ
//		  t1=4.096*2*result;     //ת���ɵ�ѹ 65535
		
		if(ABS(ROCK_R_Y-1500)>100)  //262140
		{
			R_PWM_S_Output(UPWARD,10,20);//period=1000/20
			if(t1<1.0)
			{
				t1=t1*2.0;
			}
//			speed=18;
//			sprintf((char *)string,"rpm:%.2f",speed);//0300 
			sprintf((char *)strong,"V:%.2f",20.0);//0300  	
		//	OLED_ShowString   (0,32,string,16);
			OLED_ShowString   (0,48,strong,16);
//			
			Z_PWM_S_Output(BACKWARD,0,0);
			vTaskDelay(2000);
			Z_PWM_S_Output(BACKWARD,10,10);		
		}

		else if(ABS(ROCK_R_Y-1500)<100)
		{
		if(t1>1.0&&t1<2.0)
		{
			t1=t1/2.0;
		}
		else if(t1>2.0&&t1<3.0)
		{
			t1=(t1-1.0)/2.0;
		}
			R_PWM_S_Output(UPWARD,10,90);

			sprintf((char *)strong,"V:%.2f",90.0);//0300  
			//OLED_ShowString   (0,32,string,16);
			OLED_ShowString   (0,48,strong,16);
		}
    		
    OLED_ShowGB_String(0,0,"�㶫��ҵ��ѧ",12);
    OLED_ShowGB_String(0,0,"���������Ŷ�",12);
    OLED_ShowGB_String(0,16,"��ǰת��Ϊ",12);
    OLED_ShowString   (60,16,":",16);
    	
     delay_ms(500);//��˸����
    vTaskDelay(100);
   }                 
}
