/*
 * thread.c
 *
 *  Created on: 2021年10月31日
 *      Author: huyongliang
 */

#include "Thread.h"

//不要轻视了线程调度的速度，
//【一旦有可用资源，线程会立刻调度（在下一条代码执行之前就立马调度了）】
//所以如果想要输出一些资源释放的提示性信息，务必先奏后斩（先打印提示，再释放资源）。
//线程间切换速度为几us,远小于1个OS_tick


/*******************************************************信号量实例***************************************************************/
//使用信号量来达到线程间的同步
//信号量可以用来在线程同步以及资源计数

//
///* 用 于 放 置 生 产 的 整 数 数 组 */
//rt_uint32_t array[MAXSEM];
///* 指 向 生 产 者、 消 费 者 在 array 数 组 中 的 读 写 位 置 */
//static rt_uint32_t set, get;
///* 指 向 线 程 控 制 块 的 指 针 */
//static rt_thread_t producer_tid = RT_NULL;
//static rt_thread_t consumer_tid = RT_NULL;
//struct rt_semaphore sem_lock;
//struct rt_semaphore sem_empty, sem_full;
//
//
///* 生 产 者 线 程 入 口 */
//   void producer_thread_entry(void *parameter)
//{
//    int cnt = 0;
///* 运 行 10 次 */
//    while (cnt < 10)
//   {
//     rt_sem_take(&sem_empty, RT_WAITING_FOREVER);//获取到空信号才会开始生产,empty-1
//     /*临界区上锁*/
//     rt_sem_take(&sem_lock, RT_WAITING_FOREVER);//lock-1,上锁
//     array[set % MAXSEM] = cnt + 1;//生产的数组存值
//     rt_kprintf("the producer generates a number: %d\r\n", array[set % MAXSEM]);
//     set++;
//     rt_sem_release(&sem_lock);//lock+1,解锁
//     rt_sem_release(&sem_full);//full+1,
//     cnt++;
//     rt_thread_mdelay(20);//线程挂起
//   }
//    rt_kprintf("the producer exit!\r\n");
//}
//
///* 消 费 者 线 程 入 口 */
//   void consumer_thread_entry(void *parameter)
//{
//       rt_uint32_t sum = 0;
//       while (1)
//       {
//           rt_sem_take(&sem_full, RT_WAITING_FOREVER);//获取到满位才能消费,full-1
//           /* 临 界 区 上 锁  */
//           rt_sem_take(&sem_lock, RT_WAITING_FOREVER);//lock-1,上锁
//           sum += array[get % MAXSEM];
//           rt_kprintf("the consumer[%d] get a number: %d\r\n", (get % MAXSEM), array[get% MAXSEM]);
//           get++;
//           rt_sem_release(&sem_lock);//lock+1，解锁
//           rt_sem_release(&sem_empty);//empty+1,解锁
//           if (get == 10) break;//线程停止
//           rt_thread_mdelay(50);//线程挂起
//       }
//       rt_kprintf("the consumer sum is: %d\r\n", sum);
//       rt_kprintf("the consumer exit!\r\n");
//}
//
//    int producer_consumer(void)
//   {
//   set = 0;
//   get = 0;
//   /* 初 始 化 3 个 信 号 量 */
//   rt_sem_init(&sem_lock, "lock", 1, RT_IPC_FLAG_FIFO);//将静态信号量加入到管理器中
//   rt_sem_init(&sem_empty, "empty", MAXSEM, RT_IPC_FLAG_FIFO);
//   rt_sem_init(&sem_full, "full", 0, RT_IPC_FLAG_FIFO);
//   /* 创 建 生 产 者 线 程 */
//   producer_tid = rt_thread_create("producer",
//           producer_thread_entry, RT_NULL,
//           THREAD_STACK_SIZE,
//           THREAD_PRIORITY - 1, THREAD_TIMESLICE);//会执行5个OS_tick
//
//           if (producer_tid != RT_NULL)
//           rt_thread_startup(producer_tid);
//   /* 创 建 消 费 者 线 程 */
//   consumer_tid = rt_thread_create("consumer",
//           consumer_thread_entry, RT_NULL,
//           THREAD_STACK_SIZE,
//           THREAD_PRIORITY + 1, THREAD_TIMESLICE);
//
//           if (consumer_tid != RT_NULL)
//           rt_thread_startup(consumer_tid);
//    return 0;
//   }
//           /* 导 出 到 msh 命 令 列 表 中 */
//MSH_CMD_EXPORT(producer_consumer, producer_consumer sample);


/*******************************************************互斥量实例********************************************************/
//拥有互斥量的线程拥有互斥量的所有权，
//互斥量支持递归访问且能防止线程优先级翻转；
//并且互斥量只能由持有线程释放，而信号量则可以由任何线程释放
//互斥量具有优先级继承的特征

///* 指 向 线 程 控 制 块 的 指 针 */
//static rt_thread_t tid1 = RT_NULL;
//static rt_thread_t tid2 = RT_NULL;
//static rt_thread_t tid3 = RT_NULL;
//static rt_mutex_t mutex = RT_NULL;
//#define THREAD_PRIORITY 10
//#define THREAD_STACK_SIZE 512//一般栈的最大使用为70%
//#define THREAD_TIMESLICE 5
//
////线程1最高优先级，先运行
////延时，然后打印线程2，3的优先级
///* 线 程 1 入 口 */
//static void thread1_entry(void *parameter)
//{
///* 先 让 低 优 先 级 线 程 运 行 */
//    rt_thread_mdelay(100);
///* 此 时 thread3 持 有 mutex， 并 且 thread2 等 待 持 有 mutex */
///* 检 查  thread2 与 thread3 的 优 先 级 情 况 */
//       if (tid2->current_priority != tid3->current_priority)
//    {
///* 优 先 级 不 相 同， 测 试 失 败 */
//    rt_kprintf("the priority of thread2 is: %d\r\n", tid2->current_priority);
//    rt_kprintf("the priority of thread3 is: %d\r\n", tid3->current_priority);
//    rt_kprintf("test failed.\r\n");
//    return;
//    }
//       else
//    {
//    rt_kprintf("the priority of thread2 is: %d\r\n", tid2->current_priority);
//    rt_kprintf("the priority of thread3 is: %d\r\n", tid3->current_priority);
//    rt_kprintf("test OK.\r\n");
//    }
//}
//
////线程2打印优先级信息
////然后延时，运行线程3，尝试获取互斥量
////获取到后释放互斥量
///* 线 程 2 入 口 */
//static void thread2_entry(void *parameter)
//{
//    rt_err_t result;
//    rt_kprintf("the priority of thread2 is: %d\r\n", tid2->current_priority);
//    /* 先 让 低 优 先 级 线 程 运 行 */
//    rt_thread_mdelay(50);
///*
//* 试 图 持 有 互 斥 锁， 此 时 thread3 持 有， 应 把 thread3 的 优 先 级 提 升
//* 到 thread2 相 同 的 优 先 级
//*/
//    result = rt_mutex_take(mutex, RT_WAITING_FOREVER);//句柄   等待时间
//    if (result == RT_EOK)
//   {
//        /* 释 放 互 斥 锁 */
//        rt_mutex_release(mutex);
//   }
//}
//
////线程3打印优先级
////获取互斥量保持闭锁
////线程2尝试获取互斥量
////线程3优先级提高到线程2
///* 线 程 3 入 口 */
//static void thread3_entry(void *parameter)
//{
//    rt_tick_t tick;
//    rt_err_t result;
//    rt_kprintf("the priority of thread3 is: %d\r\n", tid3->current_priority);
//    result = rt_mutex_take(mutex, RT_WAITING_FOREVER);//获取互斥量
//    if (result != RT_EOK)
//   {
//    rt_kprintf("thread3 take a mutex, failed.\r\n");
//   }
///* 做 一 个 长 时 间 的 循 环，500ms */
//    tick = rt_tick_get();//返回当前时刻信息
//    while (rt_tick_get() - tick < (RT_TICK_PER_SECOND / 2));
//    rt_mutex_release(mutex);//释放互斥量
//}
//
//
////互斥量优先级继承，
////解决优先级翻转问题
//int pri_inversion(void)
//{
///* 创 建 互 斥 锁 */
//    mutex = rt_mutex_create("mutex", RT_IPC_FLAG_FIFO);
//    if (mutex == RT_NULL)
//    {
//    rt_kprintf("create dynamic mutex failed.\r\n");
//    return -1;
//    }
///* 创 建 线 程 1 */
//    tid1 = rt_thread_create("thread1",
//        thread1_entry,
//    RT_NULL,
//    THREAD_STACK_SIZE,
//    THREAD_PRIORITY - 1, THREAD_TIMESLICE);
//
//    if (tid1 != RT_NULL)
//    rt_thread_startup(tid1);
//    /* 创 建 线 程 2 */
//    tid2 = rt_thread_create("thread2",
//        thread2_entry,
//    RT_NULL,
//    THREAD_STACK_SIZE,
//    THREAD_PRIORITY, THREAD_TIMESLICE);
//
//    if (tid2 != RT_NULL)
//        rt_thread_startup(tid2);
//    /* 创 建 线 程 3 */
//    tid3 = rt_thread_create("thread3",
//        thread3_entry,
//    RT_NULL,
//    THREAD_STACK_SIZE,
//    THREAD_PRIORITY + 1, THREAD_TIMESLICE);
//
//    if (tid3 != RT_NULL)
//    rt_thread_startup(tid3);
//    return 0;
//}
///* 导 出 到 msh 命 令 列 表 中 */
//    MSH_CMD_EXPORT(pri_inversion, pri_inversion sample);

/********************************************************事件集实例************************************************************************/
//一个线程与多个事件同步,或者多个线程与多个事件同步

//# define THREAD_PRIORITY 9
//# define THREAD_TIMESLICE 5
//# define EVENT_FLAG3 (1 << 3)
//# define EVENT_FLAG5 (1 << 5)
///* 事 件 控 制 块 */
//static struct rt_event event;
//ALIGN(RT_ALIGN_SIZE)
//static char thread1_stack[1024];
//static struct rt_thread thread1;
//
////线程 thread1 的栈空间、线程控制块以及线程 thread1 的入口函数，
////共接收两次事件，第一次永久等待 “事件 3 或事件 5”
////第二次永久等待 “事件 3 与事件 5”
//
///* 线 程 1 入 口 函 数 */
//static void thread1_recv_event(void *param)
//{
//rt_uint32_t e;
///* 第 一 次 接 收 事 件， 事 件 3 或 事 件 5 任 意 一 个 可 以 触 发 线 程 1， 接 收 完 后 清 除 事 件 标 志
//*/
//    if (rt_event_recv(&event, (EVENT_FLAG3 | EVENT_FLAG5),
//    RT_EVENT_FLAG_OR | RT_EVENT_FLAG_CLEAR,
//    RT_WAITING_FOREVER, &e) == RT_EOK)
//   {
//      rt_kprintf("thread1: OR recv event 0x%x\r\n", e);
//   }
//      rt_kprintf("thread1: delay 1s to prepare the second event\r\n");
//      rt_thread_mdelay(1000);
///* 第 二 次 接 收 事 件， 事 件 3 和 事 件 5 均 发 生 时 才 可 以 触 发 线 程 1， 接 收 完 后 清 除 事 件 标
//志 */
//    if (rt_event_recv(&event, (EVENT_FLAG3 | EVENT_FLAG5),
//    RT_EVENT_FLAG_AND | RT_EVENT_FLAG_CLEAR,
//    RT_WAITING_FOREVER, &e) == RT_EOK)
//    {
//      rt_kprintf("thread1: AND recv event 0x%x\r\n", e);
//    }
//      rt_kprintf("thread1 leave.\r\n");
//}
//
//ALIGN(RT_ALIGN_SIZE)
//static char thread2_stack[1024];
//static struct rt_thread thread2;
//
////线程 thread2 的栈空间、线程控制块以及线程 thread1 的入口函数，
////发送 3 次事件，发送事件 3，延时 200ms；
////发送事件 5，延时 200ms；发送事件 3，结束。
//
///* 线 程 2 入 口 */
//static void thread2_send_event(void *param)
//{
//    rt_kprintf("thread2: send event3\r\n");
//    rt_event_send(&event, EVENT_FLAG3);
//    rt_thread_mdelay(200);
//    rt_kprintf("thread2: send event5\r\n");
//    rt_event_send(&event, EVENT_FLAG5);
//    rt_thread_mdelay(200);
//    rt_kprintf("thread2: send event3\r\n");
//    rt_event_send(&event, EVENT_FLAG3);
//    rt_kprintf("thread2 leave.\r\n");
//}
//
//
//int event_sample(void)
//{
//    rt_err_t result;
///* 初 始 化 事 件 对 象 */
//    result = rt_event_init(&event, "event", RT_IPC_FLAG_FIFO);
//    if (result != RT_EOK)
//    {
//        rt_kprintf("init event failed.\r\n");
//        return -1;
//    }
//    rt_thread_init(&thread1,
//                "thread1",
//                thread1_recv_event,
//                RT_NULL,
//                &thread1_stack[0],
//                sizeof(thread1_stack),
//                THREAD_PRIORITY - 1, THREAD_TIMESLICE);
//        rt_thread_startup(&thread1);
//
//        rt_thread_init(&thread2,
//                "thread2",
//                thread2_send_event,
//                RT_NULL,
//                &thread2_stack[0],
//                sizeof(thread2_stack),
//                THREAD_PRIORITY, THREAD_TIMESLICE);
//        rt_thread_startup(&thread2);
//        return 0;
//}
/* 导 出 到 msh 命 令 列 表 中 */
//MSH_CMD_EXPORT(event_sample, event sample);


/************************************************************邮箱实例**************************************************************/
//邮箱是一种简单的线程间消息传递方式，特点是开销比较低，效率较高。在 RT-Thread 操作系统的实
//现中能够一次传递一个 4 字节大小的邮件，并且邮箱具备一定的存储功能，能够缓存一定数量的邮件数 (邮
//件数由创建、初始化邮箱时指定的容量决定)。邮箱中一封邮件的最大长度是 4 字节，所以邮箱能够用于不
//超过 4 字节的消息传递。

////线程间通信用邮箱
////线程1负责接收邮件，线程2负责发送邮件
//
//#define THREAD_PRIORITY 10
//#define THREAD_TIMESLICE 5
///* 邮 箱 控 制 块 */
//static struct rt_mailbox mb;
///* 用 于 放 邮 件 的 内 存 池 */
//static char mb_pool[128];
//static char mb_str1[] = "I'm a mail!";
//static char mb_str2[] = "this is another mail!";
//static char mb_str3[] = "over";
//
//ALIGN(RT_ALIGN_SIZE)
//static char thread1_stack[1024];
//static struct rt_thread thread1;
///* 线 程 1 入 口 */
//static void thread1_entry(void *parameter)
//{
//    char *str;
//    while (1)
//    {
//    rt_kprintf("thread1: try to recv a mail\n");
///* 从 邮 箱 中 收 取 邮 件 */
//    if (rt_mb_recv(&mb, (unsigned long int *)&str, RT_WAITING_FOREVER) == RT_EOK)
//    {
//        rt_kprintf("thread1: get a mail from mailbox, the content:%s\n", str);
//        if (str == mb_str3)
//            break;
//        /* 延 时 100ms */
//        rt_thread_mdelay(100);
//    }
//    }
///* 执 行 邮 箱 对 象 脱 离 */
//    rt_mb_detach(&mb);
//}
//
//ALIGN(RT_ALIGN_SIZE)
//static char thread2_stack[1024];
//static struct rt_thread thread2;
///* 线 程 2 入 口 */
//static void thread2_entry(void *parameter)
//{
//    rt_uint8_t count;
//    count = 0;
//    while (count < 10)
//    {
//        count ++;
//        if (count & 0x1)
//        {
///* 发 送 mb_str1 地 址 到 邮 箱 中 */
//            rt_mb_send(&mb, (rt_uint32_t)&mb_str1);
//        }
//        else
//        {
///* 发 送 mb_str2 地 址 到 邮 箱 中 */
//            rt_mb_send(&mb, (rt_uint32_t)&mb_str2);
//        }
///* 延 时 200ms */
//        rt_thread_mdelay(200);
//    }
///* 发 送 邮 件 告 诉 线 程 1， 线 程 2 已 经 运 行 结 束 */
//rt_mb_send(&mb, (rt_uint32_t)&mb_str3);
//}
//
//int mailbox_sample(void)
//{
//    rt_err_t result;
///* 初 始 化 一 个 mailbox */
//    result = rt_mb_init(&mb,
//                        "mbt", /* 名 称 是 mbt */
//                        &mb_pool[0], /* 邮 箱 用 到 的 内 存 池 是 mb_pool */
//                        sizeof(mb_pool) / 4, /* 邮 箱 中 的 邮 件 数 目， 因 为 一 封 邮
//                                                                件 占 4 字 节 */
//                        RT_IPC_FLAG_FIFO); /* 采 用 FIFO 方 式 进 行 线 程 等 待 */
//                        if (result != RT_EOK)
//                        {
//                            rt_kprintf("init mailbox failed.\n");
//                            return -1;
//                        }
//                        rt_thread_init(&thread1,
//                                "thread1",
//                                thread1_entry,
//                                RT_NULL,
//                                &thread1_stack[0],
//                                sizeof(thread1_stack),
//                                THREAD_PRIORITY, THREAD_TIMESLICE);
//                        rt_thread_startup(&thread1);
//
//                        rt_thread_init(&thread2,
//                                "thread2",
//                                thread2_entry,
//                                RT_NULL,
//                                &thread2_stack[0],
//                                sizeof(thread2_stack),
//                                THREAD_PRIORITY, THREAD_TIMESLICE);
//                        rt_thread_startup(&thread2);
//
//                        return 0;
//    }
///* 导 出 到 msh 命 令 列 表 中 */
//MSH_CMD_EXPORT(mailbox_sample, mailbox sample);


/*******************************************************************消息队列**********************************************/

//#define THREAD_PRIORITY 25
//#define THREAD_TIMESLICE 5
///* 消 息 队 列 控 制 块 */
//static struct rt_messagequeue mq;
///* 消 息 队 列 中 用 到 的 放 置 消 息 的 内 存 池 */
//static rt_uint8_t msg_pool[2048];
//
////线程 thread1 使用的栈空间、线程控制块，以及线程 thread1 的入口函数，
////每 50ms 从消息队列中收取消息，并打印消息内容，20 次后结束。

//ALIGN(RT_ALIGN_SIZE)
//static char thread1_stack[1024];
//static struct rt_thread thread1;
///* 线 程 1 入 口 函 数 */
//static void thread1_entry(void *parameter)
//{
//    char buf = 0;
//    rt_uint8_t cnt = 0;
//    while (1)
//    {
///* 从 消 息 队 列 中 接 收 消 息 */
//        if (rt_mq_recv(&mq, &buf, sizeof(buf), RT_WAITING_FOREVER) == RT_EOK)
//        {
//            rt_kprintf("thread1: recv msg from msg queue, the content:%c\r\n", buf);
//            if (cnt == 19)
//            {
//                break;
//            }
//        }
///* 延 时 50ms */
//        cnt++;
//        rt_thread_mdelay(50);
//    }
//    rt_kprintf("thread1: detach mq \r\n");
//    rt_mq_detach(&mq);
//}
//
////线程 thread2 使用的栈空间、线程控制块，以及线程 thread2 的入口函数，
////每 5ms 向消息队列中发送消息，并打印消息内容，20 次后结束
//ALIGN(RT_ALIGN_SIZE)
//static char thread2_stack[1024];
//static struct rt_thread thread2;
///* 线 程 2 入 口 */
//static void thread2_entry(void *parameter)
//{
//    int result;
//    char buf = 'A';
//    rt_uint8_t cnt = 0;
//    rt_kprintf("\r\n");
//    while (1)
//    {
//    if (cnt == 8)
//    {
//    /* 发 送 紧 急 消 息 到 消 息 队 列 中 */
//    result = rt_mq_urgent(&mq, &buf, 1);
//    if (result != RT_EOK)
//    {
//    rt_kprintf("rt_mq_urgent ERR\r\n");
//    }
//    else
//    {
//    rt_kprintf("thread2: send urgent message - %c\r\n", buf);
//    }
//    }
//    else if (cnt>= 20)/* 发 送 20 次 消 息 之 后 退 出 */
//    {
//    rt_kprintf("message queue stop send, thread2 quit\r\n");
//    break;
//    }
//    else
//    {
//    /* 发 送 消 息 到 消 息 队 列 中 */
//    result = rt_mq_send(&mq, &buf, 1);
//    if (result != RT_EOK)
//    {
//    rt_kprintf("rt_mq_send ERR\r\n");
//    }
//    rt_kprintf("thread2: send message - %c\r\n", buf);
//    }
//    buf++;
//    cnt++;
//    /* 延 时 5ms */
//    rt_thread_mdelay(5);
//    }
//}
//
////消息队列的示例代码，初始化了一个消息队列，初始化并启动了 thread1 与 thread2.
//// 并将函数使用MSH_CMD_EXPORT 导出命令。
//
///* 消 息 队 列 示 例 的 初 始 化 */
//int msgq_sample(void)
//{
//    rt_err_t result;
///* 初 始 化 消 息 队 列 */
//    result = rt_mq_init(&mq,
//        "mqt",
//        &msg_pool[0], /* 内 存 池 指 向 msg_pool */
//        1, /* 每 个 消 息 的 大 小 是 1 字 节 */
//        sizeof(msg_pool), /* 内 存 池 的 大 小 是 msg_pool 的 大
//        小 */
//        RT_IPC_FLAG_FIFO); /* 如 果 有 多 个 线 程 等 待， 按 照 先 来
//        先 得 到 的 方 法 分 配 消 息 */
//        if (result != RT_EOK)
//        {
//        rt_kprintf("init message queue failed.\r\n");
//        return -1;
//        }
//
//        rt_thread_init(&thread1,
//        "thread1",
//        thread1_entry,
//        RT_NULL,
//        &thread1_stack[0],
//        sizeof(thread1_stack),
//        THREAD_PRIORITY, THREAD_TIMESLICE);
//        rt_thread_startup(&thread1);
//
//        rt_thread_init(&thread2,
//        "thread2",
//        thread2_entry,
//        RT_NULL,
//        &thread2_stack[0],
//        sizeof(thread2_stack),
//        THREAD_PRIORITY, THREAD_TIMESLICE);
//        rt_thread_startup(&thread2);
//
//        return 0;
//        }
//
///* 导 出 到 msh 命 令 列 表 中 */
//MSH_CMD_EXPORT(msgq_sample, msgq sample);

/*******************************************************空闲线程钩子函数****************************************************/
//空闲线程钩子函数可以让系统在空闲的时候执行一些非紧急事务

//#define THREAD_PRIORITY      20
//#define THREAD_STACK_SIZE    1024
//#define THREAD_TIMESLICE     5
//
///* 指向线程控制块的指针 */
//static rt_thread_t tid = RT_NULL;
//
///* 空闲函数钩子函数执行次数 */
//volatile static int hook_times = 0;
//
///* 空闲线程钩子函数 */
//static void idle_hook()
//{
//    if (0 == (hook_times % 10000))
//    {
//        rt_kprintf("enter idle hook %d times.\n", hook_times);
//    }
//
//    hook_times++;
//}
//
///* 线程入口 */
//static void thread_entry(void *parameter)
//{
//    int i = 5;
//    while (i--)
//    {
//        hook_times = 0;
//        /* 休眠500ms */
//        rt_kprintf("thread1 delay 50 OS Tick.\n", hook_times);
//        rt_thread_mdelay(500);
//    }
//    rt_kprintf("delete idle hook.\n");
//
//    /* 删除空闲钩子函数 */
//    rt_thread_idle_delhook(idle_hook);
//    rt_kprintf("thread1 finish.\n");
//}
//
//int idle_hook_sample(void)
//{
//    /* 设置空闲线程钩子 */
//    rt_thread_idle_sethook(idle_hook);
//
//    /* 创建线程 */
//    tid = rt_thread_create("thread1",
//                           thread_entry, RT_NULL,
//                           THREAD_STACK_SIZE,
//                           THREAD_PRIORITY, THREAD_TIMESLICE);
//    if (tid != RT_NULL)
//        rt_thread_startup(tid);
//
//    return 0;
//}
///* 导出到 msh 命令列表中 */
//MSH_CMD_EXPORT(idle_hook_sample, idle hook sample);


/********************************************************系统调度钩子函数*******************************************************/
//这个钩子废了，不要也罢

//这个钩子函数在系统进行任务切换时运行，通过这个钩子函数，
//我们可以了解到系统任务调度时的一些信息。
//系统调度钩子函数只有设置，没有删除。
//并且和空闲线程钩子函数不同的是，系统调度钩子函数只能设置1个


///* 针对每个线程的计数器 */
//volatile rt_uint32_t count[2];
//
//
///* 线程1、2共用一个入口，但入口参数不同 */
//static void thread_entry(void *parameter)
//{
//    rt_uint32_t value;
//    value = (rt_uint32_t)parameter;
//
//    while (1)
//    {
//        rt_enter_critical();
//        value++;
//        rt_exit_critical();
//        value--;
//        /* 关闭中断 */
//        rt_kprintf("thread %d is running \n", value);
//        rt_thread_mdelay(1000); //延时一段时间
//
//    }
//}
//
//static rt_thread_t tid1 = RT_NULL;
//static rt_thread_t tid2 = RT_NULL;
//
//static void hook_of_scheduler(struct rt_thread* from, struct rt_thread* to)
//{
//    rt_kprintf("i");
//    rt_kprintf("\n");
////    rt_enter_critical();
////    rt_kprintf("from: %s -->  to: %s \r", from->name, to->name);
//}
//
//int scheduler_hook(void)
//{
//    /* 设置调度器钩子 */
//    rt_scheduler_sethook(hook_of_scheduler); /* 传入函数名称以作指针 */
//
//    /* 创建线程1 */
//    tid1 = rt_thread_create("thread1",
//                            thread_entry, (void *)1,
//                            THREAD_STACK_SIZE,
//                            THREAD_PRIORITY, THREAD_TIMESLICE);
//    if (tid1 != RT_NULL)
//        rt_thread_startup(tid1);
//
//    /* 创建线程2 */
//    tid2 = rt_thread_create("thread2",
//                            thread_entry, (void *)2,
//                            THREAD_STACK_SIZE,
//                            THREAD_PRIORITY, THREAD_TIMESLICE - 5);
//    if (tid2 != RT_NULL)
//        rt_thread_startup(tid2);
//
//    return 0;
//}
//
///* 导出到 msh 命令列表中 */
//MSH_CMD_EXPORT(scheduler_hook, scheduler_hook sample);


////注册Shell函数
///* 参数个数，参数 */
//void hellooo(int argc, char* argv[])
//{
//    rt_kprintf("hello RT-Thread!\n");
//}
////MSH_CMD_EXPORT(调用函数 , 说明);
//MSH_CMD_EXPORT(hellooo , say hello to RT-Thread);
//
////导出有参数的命令时，函数的入参为 int argc 和 char**argv。
////argc 表示参数的个数，argv 表示命令行参数字符串指针数组指针
//static void atcmd(int argc, char**argv)
//{
//
//}
//
//MSH_CMD_EXPORT(atcmd, atcmd sample: atcmd <server|client>);

//rt_kprintf("static thread 1 is running.\n");














