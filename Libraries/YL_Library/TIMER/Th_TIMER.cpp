///*
// * TIMER.cpp
// *
// *  Created on: 2021��11��27��
// *      Author: huyongliang
// */
//
//#include "Th_TIMER.h"
//
////��ʱ�����̻߳���
////Ӳ��ʱ�����жϻ���
//// ϵͳ�궨�嶼��Libraies/rtthread_libraries/include/rtdef.h�����
////#define RT_USING_TIMER_SOFT
////��OS_tickΪ��λ����������1��OS_tickΪ1ms
////��ʱ��ֻ�Ǻ����������߳�
//
///* ��ʱ���Ŀ��ƿ� */
//static rt_timer_t timer1;
//static rt_timer_t timer2;
//static int cnt = 0;
//static int cnt2 = 0;
//
///* ��ʱ��1��ʱ���� */
//static void timeout1(void *parameter)
//{
//    rt_kprintf("timer1 is timeout %d\n", cnt);
//    //���Ե���ʱ��Ƭ��ȷ����3�α�time2�ȵ���
//    /* ���е�10�Σ�ֹͣ���ڶ�ʱ�� */
//    if (cnt++ >= 9)
//    {
//        rt_timer_stop(timer1);
//        rt_kprintf("timer1 was stopped! \n");
//    }
//
//}
//
///* ��ʱ��2��ʱ���� */
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
//    /* ������ʱ��1  ���ڶ�ʱ�� */
//    timer1 = rt_timer_create("timer1", timeout1,
//                             RT_NULL, 5,
//                             RT_TIMER_FLAG_PERIODIC);//��������
//
//    /* ������ʱ��1 */
//    if (timer1 != RT_NULL)
//        rt_timer_start(timer1);
//
//    /* ������ʱ��2 ���ζ�ʱ�� */
//    timer2 = rt_timer_create("timer2", timeout2,
//                             RT_NULL, 5,
//                             RT_TIMER_FLAG_PERIODIC);//����ִ��
//
//    /* ������ʱ��2 */
//    if (timer2 != RT_NULL)
//        rt_timer_start(timer2);
//
//    return 0;
//}
//
///* ������ msh �����б��� */
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
