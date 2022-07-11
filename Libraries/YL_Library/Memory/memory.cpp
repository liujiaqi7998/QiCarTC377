///*
// * memory.cpp
// *
// *  Created on: 2021��11��28��
// *      Author: huyongliang
// */
//
//#include "memory.h"
//
///***********************************************************��̬�Ѵ�С����**************************************************************/
//
//#define THREAD_PRIORITY 25
//#define THREAD_STACK_SIZE 512
//#define THREAD_TIMESLICE 5
//
////�ѵĴ�С���ɷ�����14bit
////RAMΪ32���ֽ�
//
//
///* �� �� �� �� */
//void thread1_entry(void *parameter)
//{
//    int i;
//    char *ptr = RT_NULL; /* �� �� �� �� ָ �� */
//
////    int *p =(int *) rt_malloc(10); /* ����һ���ڴ�ռ� */
////
////    if(p != RT_NULL) /* ������ɹ� */
////    {
////        rt_memset(p, 0, 10); /* ����p ��ʼ��10���ֽ��ڴ�ռ�д0 */
////    }
////    rt_free(p);
//
//    for (i = 0; ; i++)
//    {
/////* ÿ �� �� �� (1 << i) �� С �� �� �� �� �� �� �� �� */
//            ptr = (char*)rt_malloc(1 << i);
///* �� �� �� �� �� �� */
//        if (ptr != RT_NULL)
//        {
//            rt_kprintf("get memory :%d byte\n", (1 << i));
///* �� �� �� �� �� */
//            rt_free(ptr);
//            rt_kprintf("free memory :%d byte\n", (1 << i));
//            ptr = RT_NULL;
//        }
//        else
//        {
//            rt_kprintf("try to get %d byte memory failed!\n", (1 << i));
//            return;
//        }
////        rt_kprintf("thread %d enter\n",parameter);
//    }
//}
//
//int dynmem_sample(void)
//{
//    rt_thread_t tid = RT_NULL;
///* �� �� �� �� 1 */
//    tid = rt_thread_create("thread1",
//            thread1_entry, RT_NULL,
//            THREAD_STACK_SIZE,
//            THREAD_PRIORITY,THREAD_TIMESLICE);
//    if (tid != RT_NULL)
//        rt_thread_startup(tid);
//
//    return 0;
//}
//
///* �� �� �� msh �� �� �� �� �� */
//MSH_CMD_EXPORT(dynmem_sample, dynmem sample);
//
//
///************************************************************�ڴ��*****************************************************************/
//
//static rt_uint8_t *ptr[50];
//static rt_uint8_t mempool[4096];
//static struct rt_mempool mp;
//
//#define THREAD_PRIORITY      25
//#define THREAD_STACK_SIZE    512
//#define THREAD_TIMESLICE     5
//
///* ָ���߳̿��ƿ��ָ�� */
//static rt_thread_t tid1 = RT_NULL;
//static rt_thread_t tid2 = RT_NULL;
//
///* �߳�1��� */
//static void thread1_mp_alloc(void *parameter)
//{
//    int i;
//    for (i = 0 ; i < 50 ; i++)
//    {
//        if (ptr[i] == RT_NULL)
//        {
//            /* ��ͼ�����ڴ��50�Σ������벻���ڴ��ʱ��
//               �߳�1����ת���߳�2���� */
//            ptr[i] = (rt_uint8_t*)rt_mp_alloc(&mp, RT_WAITING_FOREVER);
//            if (ptr[i] != RT_NULL)
//                rt_kprintf("allocate No.%d\n", i);
//        }
//    }
//}
//
///* �߳�2��ڣ��߳�2�����ȼ����߳�1�ͣ�Ӧ���߳�1�Ȼ��ִ�С�*/
//static void thread2_mp_release(void *parameter)
//{
//    int i;
//
//    rt_kprintf("thread2 try to release block\n");
//    for (i = 0; i < 50 ; i++)
//    {
//        /* �ͷ����з���ɹ����ڴ�� */
//        if (ptr[i] != RT_NULL)
//        {
//            rt_kprintf("release block %d\n", i);
//            rt_mp_free(ptr[i]);
//            ptr[i] = RT_NULL;
//        }
//    }
//}
//
//int mempool_sample(void)
//{
//    int i;
//    for (i = 0; i < 50; i ++) ptr[i] = RT_NULL;
//
//    /* ��ʼ���ڴ�ض��� */
//    rt_mp_init(&mp, "mp1",&mempool[0],(unsigned long)sizeof(mempool), 80); /* 4096 / (80+4) = 48 */
//    //�ڴ�ش�С/(�ڴ���С+32λָ��)
//    /* �����߳�1�������ڴ�� */
//    tid1 = rt_thread_create("thread1", thread1_mp_alloc, RT_NULL,
//                            THREAD_STACK_SIZE,
//                            THREAD_PRIORITY, THREAD_TIMESLICE);
//    if (tid1 != RT_NULL)
//        rt_thread_startup(tid1);
//
//    /* �����߳�2���ͷ��ڴ��*/
//    tid2 = rt_thread_create("thread2", thread2_mp_release, RT_NULL,
//                            THREAD_STACK_SIZE,
//                            THREAD_PRIORITY + 1, THREAD_TIMESLICE);
//    if (tid2 != RT_NULL)
//        rt_thread_startup(tid2);
//
//    return 0;
//}
//
///* ������ msh �����б��� */
//MSH_CMD_EXPORT(mempool_sample, mempool sample);
//
//
//
//
//
//
