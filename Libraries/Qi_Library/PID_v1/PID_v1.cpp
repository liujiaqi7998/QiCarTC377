/**********************************************************************************************
 * Arduino PID Library - Version 1.1.1
 * by Brett Beauregard <br3ttb@gmail.com> brettbeauregard.com
 * https://github.com/br3ttb/Arduino-PID-Library/
 * This Library is licensed under a GPLv3 License
 **********************************************************************************************/

#include "PID_v1.h"

/*Constructor (...)**********************************************************
 *    The parameters specified here are those for for which we can't set up
 *    reliable defaults, so we need to have the user set them.
 *        这里指定的参数是我们无法设置的参数
 *        可靠的默认值，所以我们需要让用户设置它们。
 ***************************************************************************/
PID::PID (double *Input, double *Output, double *Setpoint, double Kp, double Ki, double Kd, int POn,
        int ControllerDirection)
{
    myOutput = Output;
    myInput = Input;
    mySetpoint = Setpoint;
    inAuto = false;

    PID::SetOutputLimits(-500, 500);				//default output limit corresponds to the arduino pwm limits
                                                //默认输出限制对应于arduino pwm 限制

    SampleTime = 100;							//default Controller Sample Time is 0.1 seconds
                                                //默认控制器采样时间为 0.1 秒，适应电机
    PID::SetControllerDirection(ControllerDirection);
    PID::SetTunings(Kp, Ki, Kd, POn);

    lastTime = systick_getval_ms(STM0) - SampleTime;
}

/*Constructor (...)*********************************************************
 *    To allow backwards compatability for v1.1, or for people that just want
 *    to use Proportional on Error without explicitly saying so
 *        允许向后兼容 v1.1，或者只是想在没有明确说明的情况下使用 Proportional on Error 的人
 ***************************************************************************/

PID::PID (double *Input, double *Output, double *Setpoint, double Kp, double Ki, double Kd, int ControllerDirection) :
        PID::PID(Input, Output, Setpoint, Kp, Ki, Kd, P_ON_E, ControllerDirection)
{

}

/* Compute() **********************************************************************
 *     This, as they say, is where the magic happens.  this function should be called
 *   every time "void loop()" executes.  the function will decide for itself whether a new
 *   pid Output needs to be computed.  returns true when the output is computed,
 *   false when nothing has been done.
 *      正如他们所说，这就是核心计算发生的地方。 这个函数应该被调用
 *      每次“void loop()”执行时。 该功能将自行决定是否有一个新的
 *   pid 需要计算输出。 计算输出时返回true， false 当什么都没做。
 **********************************************************************************/
bool PID::Compute ()
{
    if (!inAuto)
        return false;
    unsigned long now = systick_getval_ms(STM0);
    unsigned long timeChange = (now - lastTime);
    if (timeChange >= SampleTime)
    {
        /*Compute all the working error variables*/
        /*计算所有工作错误变量*/
        double input = *myInput;
        double error = *mySetpoint - input;
        double dInput = (input - lastInput);
        outputSum += (ki * error);

        /*Add Proportional on Measurement, if P_ON_M is specified*/
        /*添加比例测量，如果指定了P_ON_M*/
        if (!pOnE)
            outputSum -= kp * dInput;

        if (outputSum > outMax)
            outputSum = outMax;
        else if (outputSum < outMin)
            outputSum = outMin;

        /*Add Proportional on Error, if P_ON_E is specified*/
        /*添加比例错误，如果指定了P_ON_E*/
        double output;
        if (pOnE)
            output = kp * error;
        else
            output = 0;

        /*Compute Rest of PID Output*/
        /*计算PID输出的剩余部分*/
        output += outputSum - kd * dInput;

        if (output > outMax)
            output = outMax;
        else if (output < outMin)
            output = outMin;
        *myOutput = output;

        /*Remember some variables for next time*/
        /*下次记住一些变量*/
        lastInput = input;
        lastTime = now;
        return true;
    }
    else
        return false;
}

/* SetTunings(...)*************************************************************
 * This function allows the controller's dynamic performance to be adjusted.
 * it's called automatically from the constructor, but tunings can also
 * be adjusted on the fly during normal operation
 ******************************************************************************/
void PID::SetTunings (double Kp, double Ki, double Kd, int POn)
{
    if (Kp < 0 || Ki < 0 || Kd < 0)
        return;

    pOn = POn;
    pOnE = POn == P_ON_E;

    dispKp = Kp;
    dispKi = Ki;
    dispKd = Kd;

    double SampleTimeInSec = ((double) SampleTime) / 1000;
    kp = Kp;
    ki = Ki * SampleTimeInSec;
    kd = Kd / SampleTimeInSec;

    if (controllerDirection == REVERSE)
    {
        kp = (0 - kp);
        ki = (0 - ki);
        kd = (0 - kd);
    }
}

/* SetTunings(...)*************************************************************
 * Set Tunings using the last-rembered POn setting
 ******************************************************************************/
void PID::SetTunings (double Kp, double Ki, double Kd)
{
    SetTunings(Kp, Ki, Kd, pOn);
}

/* SetSampleTime(...) *********************************************************
 * sets the period, in Milliseconds, at which the calculation is performed
 ******************************************************************************/
void PID::SetSampleTime (int NewSampleTime)
{
    if (NewSampleTime > 0)
    {
        double ratio = (double) NewSampleTime / (double) SampleTime;
        ki *= ratio;
        kd /= ratio;
        SampleTime = (unsigned long) NewSampleTime;
    }
}

/* SetOutputLimits(...)****************************************************
 *     This function will be used far more often than SetInputLimits.  while
 *  the input to the controller will generally be in the 0-1023 range (which is
 *  the default already,)  the output will be a little different.  maybe they'll
 *  be doing a time window and will need 0-8000 or something.  or maybe they'll
 *  want to clamp it from 0-125.  who knows.  at any rate, that can all be done
 *  here.
 **************************************************************************/
void PID::SetOutputLimits (double Min, double Max)
{
    if (Min >= Max)
        return;
    outMin = Min;
    outMax = Max;

    if (inAuto)
    {
        if (*myOutput > outMax)
            *myOutput = outMax;
        else if (*myOutput < outMin)
            *myOutput = outMin;

        if (outputSum > outMax)
            outputSum = outMax;
        else if (outputSum < outMin)
            outputSum = outMin;
    }
}

/* SetMode(...)****************************************************************
 * Allows the controller Mode to be set to manual (0) or Automatic (non-zero)
 * when the transition from manual to auto occurs, the controller is
 * automatically initialized
 ******************************************************************************/
void PID::SetMode (int Mode)
{
    bool newAuto = (Mode == AUTOMATIC);
    if (newAuto && !inAuto)
    { /*we just went from manual to auto*/
        PID::Initialize();
    }
    inAuto = newAuto;
}

/* Initialize()****************************************************************
 *	does all the things that need to happen to ensure a bumpless transfer
 *  from manual to automatic mode.
 ******************************************************************************/
void PID::Initialize ()
{
    outputSum = *myOutput;
    lastInput = *myInput;
    if (outputSum > outMax)
        outputSum = outMax;
    else if (outputSum < outMin)
        outputSum = outMin;
}

/* SetControllerDirection(...)*************************************************
 * The PID will either be connected to a DIRECT acting process (+Output leads
 * to +Input) or a REVERSE acting process(+Output leads to -Input.)  we need to
 * know which one, because otherwise we may increase the output when we should
 * be decreasing.  This is called from the constructor.
 ******************************************************************************/
void PID::SetControllerDirection (int Direction)
{
    if (inAuto && Direction != controllerDirection)
    {
        kp = (0 - kp);
        ki = (0 - ki);
        kd = (0 - kd);
    }
    controllerDirection = Direction;
}

/* Status Funcions*************************************************************
 * Just because you set the Kp=-1 doesn't mean it actually happened.  these
 * functions query the internal state of the PID.  they're here for display
 * purposes.  this are the functions the PID Front-end uses for example
 * 仅仅因为您设置了 Kp=-1 并不意味着它确实发生了。 这些
 * 函数查询PID的内部状态。 他们是来展示的
 * 目的。 这是PID前端使用的功能，例如
 ******************************************************************************/
double PID::GetKp ()
{
    return dispKp;
}
double PID::GetKi ()
{
    return dispKi;
}
double PID::GetKd ()
{
    return dispKd;
}
int PID::GetMode ()
{
    return inAuto ? AUTOMATIC : MANUAL;
}
int PID::GetDirection ()
{
    return controllerDirection;
}

