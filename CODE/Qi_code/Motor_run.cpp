#include "Motor_run.h"

/*********************��������������޸�*************************/
#define left_gpt12_gptn         GPT12_T5          //��������ѡ����ʹ�õ�GPT12��ʱ��
#define left_gpt12_count_pin    GPT12_T5INB_P10_3 //�����������ü�������
#define left_gpt12_dir_pin      GPT12_T5EUDB_P10_1//�����������ü�����������

#define right_gpt12_gptn        GPT12_T6           //�Ҳ������ѡ����ʹ�õ�GPT12��ʱ��
#define right_gpt12_count_pin   GPT12_T6INA_P20_3 //�Ҳ���������ü�������
#define right_gpt12_dir_pin     GPT12_T6EUDA_P20_0 //�Ҳ���������ü�����������
/****************************************************************/

/*********************��������޸�********************************/
MOTOR_Param LM_Param{ATOM0_CH4_P02_4,    //���MOTOR PWMͨ��1
        ATOM0_CH5_P02_5,    //���MOTOR PWMͨ��1
        MOTOR_Direction_1,  //Ĭ������ת����
        17000,              //�������Ƶ��
        75                  //����ٶȣ�ռ�ձȣ�������
};

MOTOR_Param RM_Param{ATOM0_CH6_P02_6,    //�Ҳ�MOTOR PWMͨ��1
        ATOM0_CH7_P02_7,    //�Ҳ�MOTOR PWMͨ��1
        MOTOR_Direction_1,  //Ĭ������ת����
        17000,              //�������Ƶ��
        75                  //����ٶȣ�ռ�ձȣ�������
};
/****************************************************************/

/*********************��ȷPID�����޸�*************************************************/
//����
double l_kp = 5.5;
double l_ki = 15;
double l_kd = 0.01;
//����
double r_kp = 5.5;
double r_ki = 15;
double r_kd = 0.01;
//
double aTuneStep = 50;      // Ҫ������ʹ�����㹻С��ʵ��ǯλ���е�������ʩ���ش�����
double aTuneNoise = 1;      // step: �������ֵ (>0)
int aTuneLookBack = 20;     // ����ʱ�� ����Ҫ˼��һ�¾��벨���λ���ж�Զ��ͨ������£�1/4��1/2�Ǹ������ѡ��
/****************************************************************/

/*********************ȫ���ٶȿ��Ʋ����޸�***********************/
double left_Setpoint = 50;                         //���Ԥ���ٶ�
double right_Setpoint = 50;                        //�����Ԥ���ٶ�
double left_motor_connect_gptn = 0.30;     //����������������ϵϵ����Ħ��ϵ����
double right_motor_connect_gptn = 0.30;   //�Ҳ��������������ϵϵ����Ħ��ϵ����
/****************************************************************/

bool is_int_run = false; //û�г�ʼ���ܳ�

double left_Input, left_Output; //������룬�������
boolean left_tuning = false; //�Զ�PID�����־λ
uint8_t left_ATuneModeRemember = 2;
PID left_PID(&left_Input, &left_Output, &left_Setpoint, l_kp, l_ki, l_kd, DIRECT); //���PID����
PID_ATune left_aTune(&left_Input, &left_Output); //�����Զ�PID
MOTOR left_motor = MOTOR(LM_Param); //�������

double right_Input, right_Output; //�Ҳ����룬�������
boolean right_tuning = false; //�Զ�PID�����־λ
uint8_t right_ATuneModeRemember = 2;
PID right_PID(&right_Input, &right_Output, &right_Setpoint, r_kp, r_ki, r_kd, DIRECT); //�Ҳ�PID����
PID_ATune right_aTune(&right_Input, &right_Output); //�����Զ�PID
MOTOR right_motor = MOTOR(RM_Param); //�Ҳ�����

void Motor_run_Init ()
{
    if (!is_int_run)
    {
        /* ��ʼ��gpt12����ʵ�ֶ�ȡ������ */
        gpt12_init(left_gpt12_gptn, left_gpt12_count_pin, left_gpt12_dir_pin);
        gpt12_init(right_gpt12_gptn, right_gpt12_count_pin, right_gpt12_dir_pin);
        enableInterrupts(); //���ж�

        /* ��ʼ����������� */
        left_motor.Init();
        right_motor.Init();
        
        /* PID��ʼ�� */
        left_PID.SetOutputLimits(-1000, 1000); //����PID��Χ
        left_PID.SetSampleTime(10); //����PID����ʱ��
        left_PID.SetMode(AUTOMATIC); //����PID�Զ��ۻ�����

        right_PID.SetOutputLimits(-1000, 1000);
        right_PID.SetSampleTime(10);
        right_PID.SetMode(AUTOMATIC);

        if (left_tuning)
        {
            left_tuning = false;
            left_changeAutoTune();
            left_tuning = true;
        }

        if (right_tuning)
        {
            right_tuning = false;
            right_changeAutoTune();
            right_tuning = true;
        }
    }
    is_int_run = true;
}

void Motor_run_loop ()
{

    left_Input = gpt12_get(left_gpt12_gptn); //��ȡ������
    gpt12_clear(left_gpt12_gptn); //���������δ����־λ

    right_Input = gpt12_get(right_gpt12_gptn);
    gpt12_clear(right_gpt12_gptn);

    if (left_tuning)
    {
        uint8_t val = (left_aTune.Runtime());
        if (val != 0)
        {
            left_tuning = false;
        }
        if (!left_tuning)
        { //we're done, set the tuning parameters
            l_kp = left_aTune.GetKp();
            l_ki = left_aTune.GetKi();
            l_kd = left_aTune.GetKd();
            left_PID.SetTunings(l_kp, l_ki, l_kd);
            left_AutoTuneHelper(false);
        }
    }

    if (right_tuning)
    {
        uint8_t val = (right_aTune.Runtime());
        if (val != 0)
        {
            right_tuning = false;
        }
        if (!right_tuning)
        { //we're done, set the tuning parameters
            r_kp = right_aTune.GetKp();
            r_ki = right_aTune.GetKi();
            r_kd = right_aTune.GetKd();
            right_PID.SetTunings(r_kp, r_ki, r_kd);
            right_AutoTuneHelper(false);
        }
    }

    left_PID.Compute(); //����PID
    left_motor.Run(left_Output * left_motor_connect_gptn); //���õ��������ת��

    right_PID.Compute();
    right_motor.Run(right_Output * right_motor_connect_gptn);

//        printf("%lf   %lf\n",left_Input,right_Input);

//    ɽ��������ֵ�ʾ������ʾ
//    printf("%c%c%c%c%c%c%c%c", 0x03, 0xFC, (int) left_Input, (int) left_Output, (int) right_Input, (int) right_Output,
//            0xFC, 0x03);

    rt_thread_mdelay(10);
}

void left_changeAutoTune ()
{
    if (!left_tuning)
    {
        //Set the output to the desired starting frequency.
        left_aTune.SetNoiseBand(aTuneNoise); // Ҫ������ʹ�����㹻С��ʵ��ǯλ���е�������ʩ���ش�����
        left_aTune.SetOutputStep(aTuneStep); // step: �������ֵ (>0)
        left_aTune.SetLookbackSec(aTuneLookBack); // secondΪint���ͣ���Ҫ˼��һ�¾��벨���λ���ж�Զ��ͨ������£�1/4��1/2�Ǹ������ѡ��
        left_AutoTuneHelper(true);
        left_tuning = true;
    }
    else
    { //cancel autotune
        left_aTune.Cancel();
        left_tuning = false;
        left_AutoTuneHelper(false);
    }
}

void left_AutoTuneHelper (boolean start)
{
    if (start)
        left_ATuneModeRemember = left_PID.GetMode();
    else
        left_PID.SetMode(left_ATuneModeRemember);
}

void right_changeAutoTune ()
{
    if (!right_tuning)
    {
        //Set the output to the desired starting frequency.
        right_aTune.SetNoiseBand(aTuneNoise); // Ҫ������ʹ�����㹻С��ʵ��ǯλ���е�������ʩ���ش�����
        right_aTune.SetOutputStep(aTuneStep); // step: �������ֵ (>0)
        right_aTune.SetLookbackSec(aTuneLookBack); // secondΪint���ͣ���Ҫ˼��һ�¾��벨���λ���ж�Զ��ͨ������£�1/4��1/2�Ǹ������ѡ��
        right_AutoTuneHelper(true);
        right_tuning = true;
    }
    else
    { //cancel autotune
        right_aTune.Cancel();
        right_tuning = false;
        right_AutoTuneHelper(false);
    }
}

void right_AutoTuneHelper (boolean start)
{
    if (start)
        right_ATuneModeRemember = right_PID.GetMode();
    else
        right_PID.SetMode(right_ATuneModeRemember);
}


void set_speed(int direction ,double sp){
    if (direction == 1){
        left_Setpoint = sp;
    }else{
        right_Setpoint = sp;
    }
}

void add_speed(int direction ,double sp){
    if (direction == 1){
        left_Setpoint = left_Setpoint + sp;
    }else{
        right_Setpoint = right_Setpoint + sp;
    }
}

void reduce_speed(int direction ,double sp){
    if (direction == 1){
        left_Setpoint = left_Setpoint - sp;
    }else{
        right_Setpoint = right_Setpoint - sp;
    }
}
