#include "include.h"
#include "picture.h"


#define  A_Y_P_P   1400
#define  A_Y_P     1476
#define  B_Y_P_P_R 1476
#define  B_Y_P     1476
#define  B_Y_P_P   1200
#define  C_Y_P     1506
#define  C_Y_P_P   1506
#define  D_Y_P     1506
#define  D_Y_P_P   1506
//��С�����ţ�������Ե���ǰ���������㣬����ں������Ҳ��

#define  A_X_P_P    1600  
#define  A_X_P      1818
#define  B_X_P_P    3400
#define  B_X_P      3618
#define  B_X_P_P_R  3800

#define  C_X_P_P    4456
#define  C_X_P      4638
#define  D_X_P      5274
#define  D_X_P_P    5600
int ALL_POINT=12;//ִ�е��ܵ���



Robot_State RobotState;
Motor_State MotorState;
POINTS record_points[12] ;             //����һ���ṹ�����飬��ŵ�����ꡢ�Լ����ֳ�Ա����
POINTS *RECORD_PTR = &record_points[0];    //ʹһ��ָ��ָ��ṹ��������������������Ԫ�ص�ַ�� �����������  ��¼ָ��
POINTS *BACK_PTR = NULL;        //�س�ָ�� ��ָ���
u8 Record_time=0;               //�ǵ�ǰ��Ч��¼�ĵ�ĸ��� ��0��ʼ
u8 key=0;

extern int Y_Part_Count;
extern int X_Part_Count;
extern int X_Total_Count;
extern int Y_Total_Count;
float Initial_Height=155;//��ʼ�߶�
float total_time;
float i=1.0;
int task=0;
int First_In=0;
//Current_Height��0-50��   Before_Style_Time   Style_State 		Lifting_Speed 				After_Style_Time 
//�����׵ĸ߶�				�½�ǰͣ��ʱ��		�Ƿ��½�				�½��ٶ�	  	  		�½���ͣ��ʱ��
POINTS record_points[12] = 
{
	//6*5,		2000,		0,		100,		2000,		{0,         0,        1,       0,                      1,   0                 }, //�հײ���
	
  10*5,		2000,		0,		100,		2000,		{C_Y_P,      C_X_P,     1,       C_Y_P,                     1,   C_X_P                 },  //  C 0
	
	5*5,		2000,		1,		100,		2000,		{C_Y_P,      C_X_P,     1,      ABS(C_Y_P-C_Y_P),           1,   ABS(C_X_P-C_X_P)   },  //  C 1
	
	10*5,		2000,		0,		100,		2000,		{B_Y_P_P,    B_X_P_P,   0,       ABS(B_Y_P_P-C_Y_P),        0,   ABS(B_X_P_P-C_X_P)      },  //  B0 ���±�
	
	5*5,		2000,		0,		100,		2000,		{B_Y_P_P_R,  B_X_P_P_R, 1,     ABS(B_Y_P_P_R-B_Y_P_P),        1,   ABS(B_X_P_P_R-B_X_P_P)      },  //  B0 ��
	
	10*5,		2000,		0,		100,		0,		  {B_Y_P_P_R,  B_X_P_P_R, 1,       ABS(B_Y_P_P_R-B_Y_P_P_R),  1,   ABS(B_X_P_P_R-B_X_P_P_R)    },  //  B0 B0������
	
	10*5,		500,		1,		100,		2000,		{B_Y_P,      B_X_P,       0,       ABS(B_Y_P-B_Y_P_P_R),      0,   ABS(B_X_P-B_X_P_P_R)    },  //  B1
	
	10*5,		2000,		1,		100,		2000,		{D_Y_P,      D_X_P,       1,       ABS(D_Y_P-B_Y_P),          1,   ABS(D_X_P-B_X_P)    },  //  D1
	 
	10*5,		2000,		0,		100,		2000,		{A_Y_P_P,    A_X_P_P,     0,       ABS(A_Y_P_P-D_Y_P),        0,   ABS(A_X_P_P-D_X_P)      },  //  A���·�
	
	5*5,		2000,		0,		100,		 0,		  {A_Y_P_P,    A_X_P_P,     1,       ABS(A_Y_P_P-A_Y_P_P),      1,   ABS(A_X_P_P-A_X_P_P)      },  //  A�������������
	
	10*5,		2000,		0,		100,		 0,		  {A_Y_P_P,    A_X_P_P,     1,       ABS(A_Y_P_P-A_Y_P_P),      1,   ABS(A_X_P_P-A_X_P_P)      },  //  A�������ߵ�����
	 
	10*5,		2000,		1,		100,		2000,		{A_Y_P,      A_X_P,       1,       ABS(A_Y_P-A_Y_P_P),        1,  ABS(A_X_P-A_X_P_P)      },  //  A1

	10*5,		2000,		0,		100,		2000,		{C_Y_P,      C_X_P,       1,       ABS(C_Y_P-A_Y_P),          1,  ABS(C_X_P-A_X_P)    },  //  C0
	
	
};



void XY_Action(int n)
{
	//����λ��
	Calculate_SpeedList(RECORD_PTR[n].PosArray.X_Show_Count/2);
  X_PWM_S_Output(RECORD_PTR[n].PosArray.X_Show_Dir);
  vTaskDelay(500);
  Calculate_SpeedList(RECORD_PTR[n].PosArray.Y_Show_Count/2);
  Y_PWM_S_Output(RECORD_PTR[n].PosArray.Y_Show_Dir);
	vTaskDelay(500);
	//����ǰ�ȴ�ʱ��;�Ƿ�����;�����ٶ�;�����ȴ�ʱ��
	Z_Action(RECORD_PTR[n].Current_Height,RECORD_PTR[n].Before_Style_Time, RECORD_PTR[n].Style_State, RECORD_PTR[n].Lifting_Speed, RECORD_PTR[n].After_Style_Time);
}
void RobotStateInit(void)
{
   RobotState.MovingState=STOP;
   RobotState.RecordState.Record_Close_Switch=Ok;
   RobotState.RecordState.Record_Switch=Ok;
   RobotState.RecordState.Show_Switch=Ok;
    //ֹͣ�ƶ�����
}

void FSM(void *pvParameters)
{
    while(1)
    {
			//i=lvbo(0xeb,0x82);
			//i++;
		key=KEY_Scan(1);	
			if(key>0)
			{
				RobotState.MovingState=BACK;
			}
    if(AIR_R_SHORT==1000)
    {
        RobotState.MovingState=STOP;//���������ϲ�Ĭ��ֹͣ״̬
    }
//--------------------------------------FSM
    else if(AIR_R_SHORT==2000&&AIR_R_LONG==1000)
    {
        RobotState.MovingState=MOBILE;
        //��ͷ���ƶ��������ʼ

    }
//--------------------------------------FSM
    else if(AIR_R_SHORT>=1900&&AIR_R_LONG==1500)
    {
        RobotState.MovingState=RECORD;
        //��¼һ��λ��,������λ�ã��ṹ������ x y 
    }
//--------------------------------------FSM
    else if(AIR_L_LONG==1000)
    {
      RobotState.VelocityMode=QUICK_MODE;
			RobotState.RecordState.Show_Switch=Ok;
    }
//--------------------------------------FSM
    else if(AIR_L_LONG==1500)
    {
      RobotState.VelocityMode=FINE_TURNING_MODE;
			RobotState.RecordState.Show_Switch=Ok;
    }
//--------------------------------------FSM
    else if(AIR_L_LONG>=1900)
    {
        RobotState.MovingState=SHOW;//���ݼ�¼�㣬˳������λ��
			  key=0;//�����ʾ��ʼ�Ͳ�������
        RobotState.RecordState.Record_Close_Switch=No;
			  
    }
		if(AIR_R_SHORT>=1900&&AIR_R_LONG>=1900)
    {
        RobotState.MovingState=Display_Speed;//ֻ������ʾ�ٶȺ͵�����ת�ٶȵ�����
			  vTaskSuspend(Show_Handle);
    }
		else 
		{
			vTaskSuspend(Information_Handle);
			Z_PWM_S_Output(UPWARD,0,100);//���� �ߵ�ƽʱ�䣨ʱ��Ϊ0ʱ���ֹͣ�� �ٶȣ�10-100��
			R_PWM_S_Output(BACKWARD,0,0);
		}
		
		
//--------------------------------------FSM
    
    //RobotState.MovingState=MOBILE;
    switch(RobotState.MovingState)
    {
        case STOP://xyzw����ٶȶ�Ϊ0
        {
            MotorState.V_X=0;
            MotorState.V_Y=0;
            MotorState.V_Z=0;
            MotorState.V_W=0;   
        }break;
//--------------------------------------Switch        
        case MOBILE://��һ����ȡ����ֵ�ĺ��������ݰ���ֵ���õ�������ٶ�(Ƶ��) 
        {
          RobotState.RecordState.Record_Switch=Ok;//����֮��ſ��Խ���RECORD״̬
         switch (RobotState.VelocityMode)
         {
             case QUICK_MODE:
             {
             //ɶҲ����
             }break;
             case FINE_TURNING_MODE:
             {
                vTaskDelay(50);//�ȴ��ƶ�ҡ��            
             }break;
         }
        Velocity_Filter(ROCK_L_X,ROCK_L_Y,ROCK_R_Y);//�ٶȹ��˲���ֵ
        Send_Vel(MotorState.V_X,MotorState.V_Y,MotorState.V_Z,MotorState.V_W);//�Ѹ�ֵ��ĵ���ٶȷ��ͳ�ȥ  �жϻᷢ������
        }break;
//--------------------------------------Switch
                
        case RECORD:
//��������״̬�����ж�ĳ��������L_LONG��״̬�����������Ϊ����ֵ����ֹͣ��λ�ã�����һ����¼��֮���Ҫ�Ȱ�R_LONG����������ſ����ٴμ�¼            
        {
//            if(RobotState.RecordState.Record_Switch==Ok&&RobotState.RecordState.Record_Close_Switch==Ok)
//            {
//            RobotState.RecordState.Record_Switch=No;                //��ֹ���˰���֮����һֱ��¼ �ֶ�֮���ֿ��Լ�¼
//            Record_Position(RECORD_PTR);                            //�ṹ����������Ϊ��ǰ������׵�ַ
//            }
   
        }break;
//--------------------------------------Switch 
				 case BACK:           
        {
            Back_To_Start_Demo(&RECORD_PTR[ALL_POINT-1]);//����¼�껹����һ������Ҫ��һ  �Ȼس�,adjust for x,y,to the initial				
						vTaskDelay(500);//The pause time of each point.
   
        }break;
				
        case SHOW:
        {
					  vTaskResume(Show_Handle);
            if(RobotState.RecordState.Show_Switch==Ok)
        {
            int n;
            for(n=0;n<ALL_POINT;n++)
					{
						 task=n;
						 XY_Action(n);//
						 vTaskDelay(500);
					}
					
					RobotState.RecordState.Show_Switch=No;
        }
        
        }break;
				case Display_Speed:
				{
					vTaskResume(Information_Handle);
				}break;
//--------------------------------------Switch
        
    }
    vTaskDelay(5);
    }
}

/**
  * @brief (�������ܼ��)  �ı��ĸ��ֳ�Ա�����ĺ���
  * @note  (�ص�ժҪ)   
  * @param (д��Ĳ���)
  *            @arg (��������ֵ) 
  *            @arg (��������ֵ)
  *            @arg (��������ֵ)
  * @param            
  * @retval None
  */
    void Point_Change(POINTS * Points_Ptr)
{


}


//======================================================================�ֶ���
/**
  * @brief (�������ܼ��)�Ѻ�ģң�ص�ҡ��ת��Ϊ����ٶȹ��˲���ֵ
  * @note  (�ص�ժҪ)   
  * @param (д��Ĳ���)
  *            @arg (��������ֵ) 
  *            @arg (��������ֵ)
  *            @arg (��������ֵ)
  * @param            
  * @retval None
  */

void Velocity_Filter(int L_X,int L_Y,int R_Y)
{   
    if(ABS(L_Y-1500)>100)
        {
            MotorState.V_X=-L_Y+1500;
        }
        else
        {MotorState.V_X=0;}
        if(ABS(L_X-1500)>100)
        {
            MotorState.V_Y=L_X-1500;
        }
        else
        {MotorState.V_Y=0;} 
        if(ABS(R_Y-1500)>100)
        {
            MotorState.V_Z=R_Y-1500;
        } 
        else
        {MotorState.V_Z=0;}
        MotorState.V_W=0;//��ת�ٶ�Ϊ0



}

void Send_Vel(int Vx,int Vy,int Vz,int Vw)//ֻ���ж϶���
{
  if(Vx>0)//�����˶�
  {
    X_PWM_S_Output(UPWARD);  
  }
 if(Vx<0)
  {
    X_PWM_S_Output(BACKWARD);
  }
 if(Vy>0)//����
  {
    Y_PWM_S_Output(UPWARD);

  } 
 if(Vy<0)
  {
    Y_PWM_S_Output(BACKWARD);       
  }
  
 if(Vz>0)
  {
	//	vTaskDelay(100);
  	Z_PWM_S_Output(UPWARD,10,100);
		vTaskDelay(50); 
		Z_PWM_S_Output(UPWARD,0,0);
	}
  
 if(Vz<0)
  {
//		vTaskDelay(100);
  	Z_PWM_S_Output(BACKWARD,10,100);
		vTaskDelay(50); 
		Z_PWM_S_Output(BACKWARD,0,0);
	}
}

////======================================================================�Զ���

void Record_Position(POINTS * Record_Ptr)
{
              //��¼ÿ�ε��λ�úͷ���
              Record_Ptr[Record_time].PosArray.X_Position   = X_Total_Count;        //ÿ�μ�¼Ҫ��¼һ��ȫ�ֵľ��� �س�ʱʹ��
              Record_Ptr[Record_time].PosArray.X_Show_Count = ABS(X_Part_Count);    //ʾ���������ֵҪ��֤Ϊ�� 
              Record_Ptr[Record_time].PosArray.Y_Position   = Y_Total_Count;
              Record_Ptr[Record_time].PosArray.Y_Show_Count = ABS(Y_Part_Count);
                if(X_Part_Count>0)//�ֲ����򣬿����ж�ÿ�θ���������
                    {
                        Record_Ptr[Record_time].PosArray.X_Show_Dir = UPWARD;
                        X_Part_Count=0;                                 //��ǰ�ֲ�����ֵ����
                    }
                else if(X_Part_Count<0)
                    {
                        Record_Ptr[Record_time].PosArray.X_Show_Dir = BACKWARD;
                        X_Part_Count=0;
                    }
            
                if(Y_Part_Count>0)
                    {
                        Record_Ptr[Record_time].PosArray.Y_Show_Dir = UPWARD;
                        Y_Part_Count=0;
                    }
                else if(Y_Part_Count<0)
                    {
                        Record_Ptr[Record_time].PosArray.Y_Show_Dir = BACKWARD;
                        Y_Part_Count=0;
                    }
//                  ++Record_Ptr;     //ָ���Լ��ƶ�������
                    Record_time++;      //������������Ч���ȼ���ֵ  
}


void Back_To_Start_Demo(POINTS * Back_Ptr)//�������һ����ĵ�ַ ��Ϊ���һ��������������µ�X_Total_Count Y_Total_Count
{
    if(Back_Ptr->PosArray.X_Position>0)
       {
                Calculate_SpeedList(ABS(Back_Ptr->PosArray.X_Position)/2);
                X_PWM_S_Output(BACKWARD);
       }
       if (Back_Ptr->PosArray.X_Position<0)
       {        
                Calculate_SpeedList(ABS(Back_Ptr->PosArray.X_Position)/2);
                X_PWM_S_Output(UPWARD);
       }
       vTaskDelay(50);
       if(Back_Ptr->PosArray.Y_Position>0)
       {         
                Calculate_SpeedList(ABS(Back_Ptr->PosArray.Y_Position)/2);//�������Ϊ��  �س�
                Y_PWM_S_Output(BACKWARD);
       }
       if (Back_Ptr->PosArray.Y_Position<0)
       {

                Calculate_SpeedList(ABS(Back_Ptr->PosArray.Y_Position)/2);
                Y_PWM_S_Output(UPWARD);             
       }
		
       
}

//��ף�Current_Height=18���յ��ף�Current_Height=30
//�߶�8cm���½��ٶ�100����360rpm��ʱ��1.6�����꣬�½�����Ҫ9.6Ȧ
//total_time=9.6/3.6/Lifting_Speed*60*1000
//�߶ȿ��ƺ������߶ȷ�Χ��18-80mm��
void Hight_Control(int Current_Height,int Lifting_Speed)
{
	total_time=9.6/3.6/Lifting_Speed*60*1000;
	int adjust_time;
//	Initial_Height=0;
	if(Current_Height>Initial_Height)
	{
		adjust_time=total_time*(Current_Height-Initial_Height)/80;
		Z_PWM_S_Output(UPWARD,10,Lifting_Speed);
		R_PWM_S_Output(BACKWARD,10,50);
		vTaskDelay(adjust_time);
		Z_PWM_S_Output(UPWARD,0,0);
		R_PWM_S_Output(BACKWARD,0,0);
	}
	else if(Current_Height<Initial_Height)
	{
		adjust_time=total_time*(Initial_Height-Current_Height)/80;
		Z_PWM_S_Output(BACKWARD,10,Lifting_Speed);
		R_PWM_S_Output(BACKWARD,10,50);
		vTaskDelay(adjust_time);
		Z_PWM_S_Output(BACKWARD,0,0);
		R_PWM_S_Output(BACKWARD,0,0);
	}
	  Initial_Height=Current_Height;
}

void Z_Action(int Current_Height,int Before_Style_Time,int Style_State,int Lifting_Speed,int After_Style_Time)//Lifting_Speed=100
{
	float Drilling_Time;
	int pre_time,current_height;
	int ABS_Adjust_Time;
	current_height=Current_Height;
	Hight_Control(current_height,Lifting_Speed);
	
	//pre_time=total_time/80*(current_height-25);
	//Initial_Time=total_time/40*current_height;
	Drilling_Time=500;//����ԭ����½�ʱ��
	ABS_Adjust_Time=total_time*(Current_Height-3*5)/80;//ÿһ�λص����Գ�ʼ���ʱ��
	if(Style_State==1)
	{
		vTaskDelay(Before_Style_Time);//����ǰ�ĵȴ�ʱ��
		vTaskDelay(100);
		Z_PWM_S_Output(BACKWARD,10,Lifting_Speed);
		R_PWM_S_Output(BACKWARD,10,50);
		vTaskDelay(Drilling_Time+ABS_Adjust_Time);//�������ʱ��
		Z_PWM_S_Output(BACKWARD,0,0);
		vTaskDelay(After_Style_Time);//�����ȴ�����ʱ��
		Z_PWM_S_Output(UPWARD,10,Lifting_Speed);
		vTaskDelay(Drilling_Time+ABS_Adjust_Time);//����ʱ��
		Z_PWM_S_Output(UPWARD,0,0);
		R_PWM_S_Output(UPWARD,0,0);
		//Initial_Height=10;
	}
	else
	{
		R_PWM_S_Output(BACKWARD,10,50);
		vTaskDelay(Before_Style_Time);
		R_PWM_S_Output(UPWARD,0,0);
		vTaskDelay(100);
	}
	
}

