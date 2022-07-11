///*
// * TIMER.cpp
// *
// *  Created on: 2021年11月27日
// *      Author: huyongliang
// */
//
//#include "Th_TIMER.h"
//
////软定时用在线程环境
////硬定时用在中断环境
//// 系统宏定义都在Libraies/rtthread_libraries/include/rtdef.h中添加
////#define RT_USING_TIMER_SOFT
////以OS_tick为单位，这里设置1个OS_tick为1ms
////软定时仍只是函数，不是线程
//
///* 定时器的控制块 */
//static rt_timer_t timer1;
//static rt_timer_t timer2;
//static int cnt = 0;
//static int cnt2 = 0;
//
///* 定时器1超时函数 */
//static void timeout1(void *parameter)
//{
//    rt_kprintf("timer1 is timeout %d\n", cnt);
//    //可以调整时间片以确保第3次比time2先到来
//    /* 运行第10次，停止周期定时器 */
//    if (cnt++ >= 9)
//    {
//        rt_timer_stop(timer1);
//        rt_kprintf("timer1 was stopped! \n");
//    }
//
//}
//
///* 定时器2超时函数 */
//static void timeout2(void *parameter)
//{
////    rt_kprintf("the num is %d \n",cnt);
////    rt_kprintf("one shot timer is timeout \n");
//    rt_kprintf("timer2 is timeout %d\n", cnt2);
//    if (cnt2++ >= 9)
//        {
//            rt_timer_stop(timer2);
//            rt_kprintf("timer2 was stopped! \n");
//        }
//}
//
//int timer_sample(void)
//{
//    /* 创建定时器1  周期定时器 */
//    timer1 = rt_timer_create("timer1", timeout1,
//                             RT_NULL, 5,
//                             RT_TIMER_FLAG_PERIODIC);//周期运行
//
//    /* 启动定时器1 */
//    if (timer1 != RT_NULL)
//        rt_timer_start(timer1);
//
//    /* 创建定时器2 单次定时器 */
//    timer2 = rt_timer_create("timer2", timeout2,
//                             RT_NULL, 5,
//                             RT_TIMER_FLAG_PERIODIC);//单次执行
//
//    /* 启动定时器2 */
//    if (timer2 != RT_NULL)
//        rt_timer_start(timer2);
//
//    return 0;
//}
//
///* 导出到 msh 命令列表中 */
//MSH_CMD_EXPORT(timer_sample, timer sample);
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
