#include "Motor_run.h"

/*********************编码器电机参数修改*************************/
#define left_gpt12_gptn         GPT12_T5          //左侧编码器选择所使用的GPT12定时器
#define left_gpt12_count_pin    GPT12_T5INB_P10_3 //左侧编码器设置计数引脚
#define left_gpt12_dir_pin      GPT12_T5EUDB_P10_1//左侧编码器设置计数方向引脚

#define right_gpt12_gptn        GPT12_T6           //右侧编码器选择所使用的GPT12定时器
#define right_gpt12_count_pin   GPT12_T6INA_P20_3 //右侧编码器设置计数引脚
#define right_gpt12_dir_pin     GPT12_T6EUDA_P20_0 //右侧编码器设置计数方向引脚
/****************************************************************/

/*********************电机参数修改********************************/
MOTOR_Param LM_Param{ATOM0_CH4_P02_4,    //左侧MOTOR PWM通道1
        ATOM0_CH5_P02_5,    //左侧MOTOR PWM通道1
        MOTOR_Direction_1,  //默认正旋转方向
        17000,              //电机控制频率
        75                  //最大速度（占空比），保护
};

MOTOR_Param RM_Param{ATOM0_CH6_P02_6,    //右侧MOTOR PWM通道1
        ATOM0_CH7_P02_7,    //右侧MOTOR PWM通道1
        MOTOR_Direction_1,  //默认正旋转方向
        17000,              //电机控制频率
        75                  //最大速度（占空比），保护
};
/****************************************************************/

/*********************精确PID参数修改*************************************************/
//左轮
double l_kp = 5.5;
double l_ki = 15;
double l_kd = 0.01;
//右轮
double r_kp = 5.5;
double r_ki = 15;
double r_kd = 0.01;
//
double aTuneStep = 50;      // 要尽可能使噪声足够小且实现钳位，有点类似于施密特触发器
double aTuneNoise = 1;      // step: 输出步长值 (>0)
int aTuneLookBack = 20;     // 回溯时间 ，需要思考一下距离波峰的位置有多远。通常情况下，1/4到1/2是个不错的选择
/****************************************************************/

/*********************全局速度控制参数修改***********************/
double left_Setpoint = 50;                         //左侧预期速度
double right_Setpoint = 50;                        //右左侧预期速度
double left_motor_connect_gptn = 0.30;     //左侧编码器与驱动联系系数（摩擦系数）
double right_motor_connect_gptn = 0.30;   //右侧编码器与驱动联系系数（摩擦系数）
/****************************************************************/

bool is_int_run = false; //没有初始化跑车

double left_Input, left_Output; //左侧输入，输出数据
boolean left_tuning = false; //自动PID处理标志位
uint8_t left_ATuneModeRemember = 2;
PID left_PID(&left_Input, &left_Output, &left_Setpoint, l_kp, l_ki, l_kd, DIRECT); //左侧PID对象
PID_ATune left_aTune(&left_Input, &left_Output); //左轮自动PID
MOTOR left_motor = MOTOR(LM_Param); //左侧驱动

double right_Input, right_Output; //右侧输入，输出数据
boolean right_tuning = false; //自动PID处理标志位
uint8_t right_ATuneModeRemember = 2;
PID right_PID(&right_Input, &right_Output, &right_Setpoint, r_kp, r_ki, r_kd, DIRECT); //右侧PID对象
PID_ATune right_aTune(&right_Input, &right_Output); //右轮自动PID
MOTOR right_motor = MOTOR(RM_Param); //右侧驱动

void Motor_run_Init ()
{
    if (!is_int_run)
    {
        /* 初始化gpt12函数实现读取编码器 */
        gpt12_init(left_gpt12_gptn, left_gpt12_count_pin, left_gpt12_dir_pin);
        gpt12_init(right_gpt12_gptn, right_gpt12_count_pin, right_gpt12_dir_pin);
        enableInterrupts(); //打开中断

        /* 初始化电机驱动器 */
        left_motor.Init();
        right_motor.Init();
        
        /* PID初始化 */
        left_PID.SetOutputLimits(-1000, 1000); //设置PID范围
        left_PID.SetSampleTime(10); //设置PID采样时间
        left_PID.SetMode(AUTOMATIC); //开启PID自动累积运算

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

    left_Input = gpt12_get(left_gpt12_gptn); //读取编码器
    gpt12_clear(left_gpt12_gptn); //清除编码器未读标志位

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

    left_PID.Compute(); //计算PID
    left_motor.Run(left_Output * left_motor_connect_gptn); //设置电机驱动器转速

    right_PID.Compute();
    right_motor.Run(right_Output * right_motor_connect_gptn);

//        printf("%lf   %lf\n",left_Input,right_Input);

//    山外调试助手的示波器显示
//    printf("%c%c%c%c%c%c%c%c", 0x03, 0xFC, (int) left_Input, (int) left_Output, (int) right_Input, (int) right_Output,
//            0xFC, 0x03);

    rt_thread_mdelay(10);
}

void left_changeAutoTune ()
{
    if (!left_tuning)
    {
        //Set the output to the desired starting frequency.
        left_aTune.SetNoiseBand(aTuneNoise); // 要尽可能使噪声足够小且实现钳位，有点类似于施密特触发器
        left_aTune.SetOutputStep(aTuneStep); // step: 输出步长值 (>0)
        left_aTune.SetLookbackSec(aTuneLookBack); // second为int类型，需要思考一下距离波峰的位置有多远。通常情况下，1/4到1/2是个不错的选择
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
        right_aTune.SetNoiseBand(aTuneNoise); // 要尽可能使噪声足够小且实现钳位，有点类似于施密特触发器
        right_aTune.SetOutputStep(aTuneStep); // step: 输出步长值 (>0)
        right_aTune.SetLookbackSec(aTuneLookBack); // second为int类型，需要思考一下距离波峰的位置有多远。通常情况下，1/4到1/2是个不错的选择
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
