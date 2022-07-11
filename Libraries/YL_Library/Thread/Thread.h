///*
// * thread.h
// *
// *  Created on: 2021年10月31日
// *      Author: huyongliang
// */
//
//#ifndef CODE_THREAD_THREAD_H_
//#define CODE_THREAD_THREAD_H_
//
//#include "rtthread.h"
//#include "rthw.h"
//
//#define THREAD_PRIORITY 6
////tc264应该只能支持32个优先级
//#define THREAD_STACK_SIZE 512
//#define THREAD_TIMESLICE 5
////时间片，只在线程优先级相同时发生作用
////时间片约束单次线程运行时长，
////单位是1个OS tick
//#define MAXSEM 5
//
////创建动态线程
////编写线程代码-创建线程-运行线程
//
////创建静态线程
////创建线程堆-创建线程控制块
////编写线程代码-创建线程-运行线程
//
////若系统不使用外扩RAM调度线程时，线程控制块和线程栈不从外扩分配
////动态线程速度不会受到影响
//
////空闲线程的钩子函数要注意任何时刻都不会被挂起
////空闲线程可以设置4个钩子函数
//
//
////信号量
//int producer_consumer(void);
//
////互斥量
//int pri_inversion(void);
//
////事件集
//int event_sample(void);
//
////邮箱
//int mailbox_sample(void);
//
////消息队列
//int msgq_sample(void);
//
////空闲线程
////钩子函数中执行的相关代码必须保证空闲线程在任何时刻都不会被挂起
//int idle_hook_sample(void);
//
////系统调度钩子函数
//int scheduler_hook(void);
//
//
///*******************************************************************线程调度**********************************************/
////用来关闭系统调度,保护临界区
///***********关闭中断**********///在关闭中断前，应先定义变量将中断状态记录下来，然后开启中断时再将中断状态导入
////中断锁
////    rt_base_t level;
////        /* 关闭中断 */
////        level = rt_hw_interrupt_disable();//使用中断锁来同步中断
////这个函数用于关闭中断并返回关闭中断前的中断状态。
////        /* 恢复中断 */
////        rt_hw_interrupt_enable(level);
////恢复关闭中断前的运行状态
//
//
///**********禁止调度**********/
////调度器上锁
////void rt_enter_critical(void); /* 进入临界区*/
////和中断锁不同的是允许系统响应外部中断
////所以要注意临界区的资源未被修改
//
////调度器解锁
////void rt_exit_critical(void); /* 退出临界区*/
//
////上锁和解锁嵌套使用，每上锁一次就要相应的解锁，最多嵌套65535次
////使用调度器锁之后，系统将不再具有优先级关系
//
///***************************************************************信号量****************************************************/
////互斥特性保护临界区
//
////信号量控制块
////struct rt_semaphore
////{
////   struct rt_ipc_object parent;/*继承自ipc_object类*/
////   rt_uint16_t value;   /* 信号量的值  */
////};
/////* rt_sem_t是指向semaphore结构体的指针类型 */
////typedef struct rt_semaphore* rt_sem_t;
//
////创建信号量
////rt_sem_t rt_sem_create (const char* name, rt_uint32_t value, rt_uint8_t flag);
//
////删除信号量
////rt_err_t rt_sem_delete (rt_sem_t sem);
//
////初始化信号量对象
////rt_err_t rt_sem_init (rt_sem_t sem, const char* name, rt_uint32_t value, rt_uint8_t flag);
//
////脱离信号量
////rt_err_t rt_sem_detach (rt_sem_t sem);
////使用该函数后，内核先唤醒所有挂在该信号量等待队列上的线程，然后将该信号量从内核对象管理器中删除。
//
////获取信号量
////rt_err_t rt_sem_take (rt_sem_t sem, rt_int32_t time);
//
////无等待获取信号量
////rt_err_t rt_sem_trytake(rt_sem_t sem);
//
////释放信号量
////rt_err_t rt_sem_release(rt_sem_t sem);
////当线程完成资源的访问后，应尽快释放它持有的信号量，使得其他线程能获得该信号量
//
//
///****************************************************************************互斥量****************************************/
//
////创建互斥量
////rt_mutex_t rt_mutex_create (const char* name, rt_uint8_t flag);
//
////删除互斥量
////rt_err_t rt_mutex_delete (rt_mutex_t mutex);
//
////初始化互斥量
////rt_err_t rt_mutex_init (rt_mutex_t mutex, const char* name, rt_uint8_t flag);
////静态互斥量对象的内存是在系统编译时由编译器分配的，一般放于数据段或ZI段中。
////在使用这类静态互斥量对象前，需要先进行初始化。
//
//
////脱离互斥量
////rt_err_t rt_mutex_detach (rt_mutex_t mutex);
////使用该函数接口后，内核先唤醒所有挂在该互斥量上的线程，
////然后系统将该互斥量从内核对象管理器链表中删除。
//
//
////获取互斥量
////rt_err_t rt_mutex_take (rt_mutex_t mutex, rt_int32_t time);
////线程通过互斥量申请服务获取互斥量的所有权。线程对互斥量的所有权是独占的，
////某一个时刻一个互斥量只能被一个线程持有。
//
////释放互斥量
////rt_err_t rt_mutex_release(rt_mutex_t mutex);
////当线程完成互斥资源的访问后，应尽快释放它占据的互斥量，使得其他线程能及时获取该互斥量。
//
///*************************************************************事件管理****************************************************/
//
////创建事件
////rt_event_t rt_event_create (const char* name, rt_uint8_t flag);
////调用该函数接口时，系统会从动态内存堆中分配事件对象，然后进行对象的初始化，IPC对象初始化，并把set设置成0。
//
////删除事件
////rt_err_t rt_event_delete (rt_event_t event);
////系统不再使用事件对象时，通过删除事件对象控制块来释放系统资源
//
////初始化事件
////rt_err_t rt_event_init(rt_event_t event, const char* name, rt_uint8_t flag);
////调用该接口时，需指定静态事件对象的句柄（即指向事件控制块的指针），
////然后系统会初始化事件对象，并加入到系统对象容器中进行管理。
//
////脱离事件
////rt_err_t rt_event_detach(rt_event_t event);
////用户调用这个函数时，系统首先唤醒所有挂在该事件等待队列上的线程，
////然后将该事件从内核对象管理器中删除。
//
////接收事件
////rt_err_t rt_event_recv(rt_event_t event, rt_uint32_t set, rt_uint8_t option,
////rt_int32_t timeout, rt_uint32_t* recved);
//
////发送事件
////rt_err_t rt_event_send(rt_event_t event, rt_uint32_t set);
////通过参数set指定的事件标志来设定event对象的事件标志值，然后遍历等待在event事件对象上的等待线程链表，
////判断是否有线程的事件激活要求与当前event对象事件标志值匹配，如果有，则唤醒该线程。
//
//
//
//
//
//
//
//#endif /* CODE_THREAD_THREAD_H_ */
//
