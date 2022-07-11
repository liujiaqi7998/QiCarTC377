///*
// * memory.cpp
// *
// *  Created on: 2021年11月28日
// *      Author: huyongliang
// */
//
//#include "memory.h"
//
///***********************************************************动态堆大小分配**************************************************************/
//
//#define THREAD_PRIORITY 25
//#define THREAD_STACK_SIZE 512
//#define THREAD_TIMESLICE 5
//
////堆的大小最多可分配至14bit
////RAM为32个字节
//
//
///* 线 程 入 口 */
//void thread1_entry(void *parameter)
//{
//    int i;
//    char *ptr = RT_NULL; /* 内 存 块 的 指 针 */
//
////    int *p =(int *) rt_malloc(10); /* 申请一段内存空间 */
////
////    if(p != RT_NULL) /* 若申请成功 */
////    {
////        rt_memset(p, 0, 10); /* 将从p 开始的10个字节内存空间写0 */
////    }
////    rt_free(p);
//
//    for (i = 0; ; i++)
//    {
/////* 每 次 分 配 (1 << i) 大 小 字 节 数 的 内 存 空 间 */
//            ptr = (char*)rt_malloc(1 << i);
///* 如 果 分 配 成 功 */
//        if (ptr != RT_NULL)
//        {
//            rt_kprintf("get memory :%d byte\n", (1 << i));
///* 释 放 内 存 块 */
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
///* 创 建 线 程 1 */
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
///* 导 出 到 msh 命 令 列 表 中 */
//MSH_CMD_EXPORT(dynmem_sample, dynmem sample);
//
//
///************************************************************内存池*****************************************************************/
//
//static rt_uint8_t *ptr[50];
//static rt_uint8_t mempool[4096];
//static struct rt_mempool mp;
//
//#define THREAD_PRIORITY      25
//#define THREAD_STACK_SIZE    512
//#define THREAD_TIMESLICE     5
//
///* 指向线程控制块的指针 */
//static rt_thread_t tid1 = RT_NULL;
//static rt_thread_t tid2 = RT_NULL;
//
///* 线程1入口 */
//static void thread1_mp_alloc(void *parameter)
//{
//    int i;
//    for (i = 0 ; i < 50 ; i++)
//    {
//        if (ptr[i] == RT_NULL)
//        {
//            /* 试图申请内存块50次，当申请不到内存块时，
//               线程1挂起，转至线程2运行 */
//            ptr[i] = (rt_uint8_t*)rt_mp_alloc(&mp, RT_WAITING_FOREVER);
//            if (ptr[i] != RT_NULL)
//                rt_kprintf("allocate No.%d\n", i);
//        }
//    }
//}
//
///* 线程2入口，线程2的优先级比线程1低，应该线程1先获得执行。*/
//static void thread2_mp_release(void *parameter)
//{
//    int i;
//
//    rt_kprintf("thread2 try to release block\n");
//    for (i = 0; i < 50 ; i++)
//    {
//        /* 释放所有分配成功的内存块 */
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
//    /* 初始化内存池对象 */
//    rt_mp_init(&mp, "mp1",&mempool[0],(unsigned long)sizeof(mempool), 80); /* 4096 / (80+4) = 48 */
//    //内存池大小/(内存块大小+32位指针)
//    /* 创建线程1：申请内存池 */
//    tid1 = rt_thread_create("thread1", thread1_mp_alloc, RT_NULL,
//                            THREAD_STACK_SIZE,
//                            THREAD_PRIORITY, THREAD_TIMESLICE);
//    if (tid1 != RT_NULL)
//        rt_thread_startup(tid1);
//
//    /* 创建线程2：释放内存池*/
//    tid2 = rt_thread_create("thread2", thread2_mp_release, RT_NULL,
//                            THREAD_STACK_SIZE,
//                            THREAD_PRIORITY + 1, THREAD_TIMESLICE);
//    if (tid2 != RT_NULL)
//        rt_thread_startup(tid2);
//
//    return 0;
//}
//
///* 导出到 msh 命令列表中 */
//MSH_CMD_EXPORT(mempool_sample, mempool sample);
//
//
//
//
//
//
