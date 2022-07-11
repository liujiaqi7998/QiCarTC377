/*
 * thread.c
 *
 *  Created on: 2021��10��31��
 *      Author: huyongliang
 */

#include "Thread.h"

//��Ҫ�������̵߳��ȵ��ٶȣ�
//��һ���п�����Դ���̻߳����̵��ȣ�����һ������ִ��֮ǰ����������ˣ���
//���������Ҫ���һЩ��Դ�ͷŵ���ʾ����Ϣ����������ն���ȴ�ӡ��ʾ�����ͷ���Դ����
//�̼߳��л��ٶ�Ϊ��us,ԶС��1��OS_tick


/*******************************************************�ź���ʵ��***************************************************************/
//ʹ���ź������ﵽ�̼߳��ͬ��
//�ź��������������߳�ͬ���Լ���Դ����

//
///* �� �� �� �� �� �� �� �� �� �� �� */
//rt_uint32_t array[MAXSEM];
///* ָ �� �� �� �ߡ� �� �� �� �� array �� �� �� �� �� д λ �� */
//static rt_uint32_t set, get;
///* ָ �� �� �� �� �� �� �� ָ �� */
//static rt_thread_t producer_tid = RT_NULL;
//static rt_thread_t consumer_tid = RT_NULL;
//struct rt_semaphore sem_lock;
//struct rt_semaphore sem_empty, sem_full;
//
//
///* �� �� �� �� �� �� �� */
//   void producer_thread_entry(void *parameter)
//{
//    int cnt = 0;
///* �� �� 10 �� */
//    while (cnt < 10)
//   {
//     rt_sem_take(&sem_empty, RT_WAITING_FOREVER);//��ȡ�����źŲŻῪʼ����,empty-1
//     /*�ٽ�������*/
//     rt_sem_take(&sem_lock, RT_WAITING_FOREVER);//lock-1,����
//     array[set % MAXSEM] = cnt + 1;//�����������ֵ
//     rt_kprintf("the producer generates a number: %d\r\n", array[set % MAXSEM]);
//     set++;
//     rt_sem_release(&sem_lock);//lock+1,����
//     rt_sem_release(&sem_full);//full+1,
//     cnt++;
//     rt_thread_mdelay(20);//�̹߳���
//   }
//    rt_kprintf("the producer exit!\r\n");
//}
//
///* �� �� �� �� �� �� �� */
//   void consumer_thread_entry(void *parameter)
//{
//       rt_uint32_t sum = 0;
//       while (1)
//       {
//           rt_sem_take(&sem_full, RT_WAITING_FOREVER);//��ȡ����λ��������,full-1
//           /* �� �� �� �� ��  */
//           rt_sem_take(&sem_lock, RT_WAITING_FOREVER);//lock-1,����
//           sum += array[get % MAXSEM];
//           rt_kprintf("the consumer[%d] get a number: %d\r\n", (get % MAXSEM), array[get% MAXSEM]);
//           get++;
//           rt_sem_release(&sem_lock);//lock+1������
//           rt_sem_release(&sem_empty);//empty+1,����
//           if (get == 10) break;//�߳�ֹͣ
//           rt_thread_mdelay(50);//�̹߳���
//       }
//       rt_kprintf("the consumer sum is: %d\r\n", sum);
//       rt_kprintf("the consumer exit!\r\n");
//}
//
//    int producer_consumer(void)
//   {
//   set = 0;
//   get = 0;
//   /* �� ʼ �� 3 �� �� �� �� */
//   rt_sem_init(&sem_lock, "lock", 1, RT_IPC_FLAG_FIFO);//����̬�ź������뵽��������
//   rt_sem_init(&sem_empty, "empty", MAXSEM, RT_IPC_FLAG_FIFO);
//   rt_sem_init(&sem_full, "full", 0, RT_IPC_FLAG_FIFO);
//   /* �� �� �� �� �� �� �� */
//   producer_tid = rt_thread_create("producer",
//           producer_thread_entry, RT_NULL,
//           THREAD_STACK_SIZE,
//           THREAD_PRIORITY - 1, THREAD_TIMESLICE);//��ִ��5��OS_tick
//
//           if (producer_tid != RT_NULL)
//           rt_thread_startup(producer_tid);
//   /* �� �� �� �� �� �� �� */
//   consumer_tid = rt_thread_create("consumer",
//           consumer_thread_entry, RT_NULL,
//           THREAD_STACK_SIZE,
//           THREAD_PRIORITY + 1, THREAD_TIMESLICE);
//
//           if (consumer_tid != RT_NULL)
//           rt_thread_startup(consumer_tid);
//    return 0;
//   }
//           /* �� �� �� msh �� �� �� �� �� */
//MSH_CMD_EXPORT(producer_consumer, producer_consumer sample);


/*******************************************************������ʵ��********************************************************/
//ӵ�л��������߳�ӵ�л�����������Ȩ��
//������֧�ֵݹ�������ܷ�ֹ�߳����ȼ���ת��
//���һ�����ֻ���ɳ����߳��ͷţ����ź�����������κ��߳��ͷ�
//�������������ȼ��̳е�����

///* ָ �� �� �� �� �� �� �� ָ �� */
//static rt_thread_t tid1 = RT_NULL;
//static rt_thread_t tid2 = RT_NULL;
//static rt_thread_t tid3 = RT_NULL;
//static rt_mutex_t mutex = RT_NULL;
//#define THREAD_PRIORITY 10
//#define THREAD_STACK_SIZE 512//һ��ջ�����ʹ��Ϊ70%
//#define THREAD_TIMESLICE 5
//
////�߳�1������ȼ���������
////��ʱ��Ȼ���ӡ�߳�2��3�����ȼ�
///* �� �� 1 �� �� */
//static void thread1_entry(void *parameter)
//{
///* �� �� �� �� �� �� �� �� �� �� */
//    rt_thread_mdelay(100);
///* �� ʱ thread3 �� �� mutex�� �� �� thread2 �� �� �� �� mutex */
///* �� ��  thread2 �� thread3 �� �� �� �� �� �� */
//       if (tid2->current_priority != tid3->current_priority)
//    {
///* �� �� �� �� �� ͬ�� �� �� ʧ �� */
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
////�߳�2��ӡ���ȼ���Ϣ
////Ȼ����ʱ�������߳�3�����Ի�ȡ������
////��ȡ�����ͷŻ�����
///* �� �� 2 �� �� */
//static void thread2_entry(void *parameter)
//{
//    rt_err_t result;
//    rt_kprintf("the priority of thread2 is: %d\r\n", tid2->current_priority);
//    /* �� �� �� �� �� �� �� �� �� �� */
//    rt_thread_mdelay(50);
///*
//* �� ͼ �� �� �� �� ���� �� ʱ thread3 �� �У� Ӧ �� thread3 �� �� �� �� �� ��
//* �� thread2 �� ͬ �� �� �� ��
//*/
//    result = rt_mutex_take(mutex, RT_WAITING_FOREVER);//���   �ȴ�ʱ��
//    if (result == RT_EOK)
//   {
//        /* �� �� �� �� �� */
//        rt_mutex_release(mutex);
//   }
//}
//
////�߳�3��ӡ���ȼ�
////��ȡ���������ֱ���
////�߳�2���Ի�ȡ������
////�߳�3���ȼ���ߵ��߳�2
///* �� �� 3 �� �� */
//static void thread3_entry(void *parameter)
//{
//    rt_tick_t tick;
//    rt_err_t result;
//    rt_kprintf("the priority of thread3 is: %d\r\n", tid3->current_priority);
//    result = rt_mutex_take(mutex, RT_WAITING_FOREVER);//��ȡ������
//    if (result != RT_EOK)
//   {
//    rt_kprintf("thread3 take a mutex, failed.\r\n");
//   }
///* �� һ �� �� ʱ �� �� ѭ ����500ms */
//    tick = rt_tick_get();//���ص�ǰʱ����Ϣ
//    while (rt_tick_get() - tick < (RT_TICK_PER_SECOND / 2));
//    rt_mutex_release(mutex);//�ͷŻ�����
//}
//
//
////���������ȼ��̳У�
////������ȼ���ת����
//int pri_inversion(void)
//{
///* �� �� �� �� �� */
//    mutex = rt_mutex_create("mutex", RT_IPC_FLAG_FIFO);
//    if (mutex == RT_NULL)
//    {
//    rt_kprintf("create dynamic mutex failed.\r\n");
//    return -1;
//    }
///* �� �� �� �� 1 */
//    tid1 = rt_thread_create("thread1",
//        thread1_entry,
//    RT_NULL,
//    THREAD_STACK_SIZE,
//    THREAD_PRIORITY - 1, THREAD_TIMESLICE);
//
//    if (tid1 != RT_NULL)
//    rt_thread_startup(tid1);
//    /* �� �� �� �� 2 */
//    tid2 = rt_thread_create("thread2",
//        thread2_entry,
//    RT_NULL,
//    THREAD_STACK_SIZE,
//    THREAD_PRIORITY, THREAD_TIMESLICE);
//
//    if (tid2 != RT_NULL)
//        rt_thread_startup(tid2);
//    /* �� �� �� �� 3 */
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
///* �� �� �� msh �� �� �� �� �� */
//    MSH_CMD_EXPORT(pri_inversion, pri_inversion sample);

/********************************************************�¼���ʵ��************************************************************************/
//һ���߳������¼�ͬ��,���߶���߳������¼�ͬ��

//# define THREAD_PRIORITY 9
//# define THREAD_TIMESLICE 5
//# define EVENT_FLAG3 (1 << 3)
//# define EVENT_FLAG5 (1 << 5)
///* �� �� �� �� �� */
//static struct rt_event event;
//ALIGN(RT_ALIGN_SIZE)
//static char thread1_stack[1024];
//static struct rt_thread thread1;
//
////�߳� thread1 ��ջ�ռ䡢�߳̿��ƿ��Լ��߳� thread1 ����ں�����
////�����������¼�����һ�����õȴ� ���¼� 3 ���¼� 5��
////�ڶ������õȴ� ���¼� 3 ���¼� 5��
//
///* �� �� 1 �� �� �� �� */
//static void thread1_recv_event(void *param)
//{
//rt_uint32_t e;
///* �� һ �� �� �� �� ���� �� �� 3 �� �� �� 5 �� �� һ �� �� �� �� �� �� �� 1�� �� �� �� �� �� �� �� �� �� ־
//*/
//    if (rt_event_recv(&event, (EVENT_FLAG3 | EVENT_FLAG5),
//    RT_EVENT_FLAG_OR | RT_EVENT_FLAG_CLEAR,
//    RT_WAITING_FOREVER, &e) == RT_EOK)
//   {
//      rt_kprintf("thread1: OR recv event 0x%x\r\n", e);
//   }
//      rt_kprintf("thread1: delay 1s to prepare the second event\r\n");
//      rt_thread_mdelay(1000);
///* �� �� �� �� �� �� ���� �� �� 3 �� �� �� 5 �� �� �� ʱ �� �� �� �� �� �� �� 1�� �� �� �� �� �� �� �� �� ��
//־ */
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
////�߳� thread2 ��ջ�ռ䡢�߳̿��ƿ��Լ��߳� thread1 ����ں�����
////���� 3 ���¼��������¼� 3����ʱ 200ms��
////�����¼� 5����ʱ 200ms�������¼� 3��������
//
///* �� �� 2 �� �� */
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
///* �� ʼ �� �� �� �� �� */
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
/* �� �� �� msh �� �� �� �� �� */
//MSH_CMD_EXPORT(event_sample, event sample);


/************************************************************����ʵ��**************************************************************/
//������һ�ּ򵥵��̼߳���Ϣ���ݷ�ʽ���ص��ǿ����Ƚϵͣ�Ч�ʽϸߡ��� RT-Thread ����ϵͳ��ʵ
//�����ܹ�һ�δ���һ�� 4 �ֽڴ�С���ʼ�����������߱�һ���Ĵ洢���ܣ��ܹ�����һ���������ʼ��� (��
//�����ɴ�������ʼ������ʱָ������������)��������һ���ʼ�����󳤶��� 4 �ֽڣ����������ܹ����ڲ�
//���� 4 �ֽڵ���Ϣ���ݡ�

////�̼߳�ͨ��������
////�߳�1��������ʼ����߳�2�������ʼ�
//
//#define THREAD_PRIORITY 10
//#define THREAD_TIMESLICE 5
///* �� �� �� �� �� */
//static struct rt_mailbox mb;
///* �� �� �� �� �� �� �� �� �� */
//static char mb_pool[128];
//static char mb_str1[] = "I'm a mail!";
//static char mb_str2[] = "this is another mail!";
//static char mb_str3[] = "over";
//
//ALIGN(RT_ALIGN_SIZE)
//static char thread1_stack[1024];
//static struct rt_thread thread1;
///* �� �� 1 �� �� */
//static void thread1_entry(void *parameter)
//{
//    char *str;
//    while (1)
//    {
//    rt_kprintf("thread1: try to recv a mail\n");
///* �� �� �� �� �� ȡ �� �� */
//    if (rt_mb_recv(&mb, (unsigned long int *)&str, RT_WAITING_FOREVER) == RT_EOK)
//    {
//        rt_kprintf("thread1: get a mail from mailbox, the content:%s\n", str);
//        if (str == mb_str3)
//            break;
//        /* �� ʱ 100ms */
//        rt_thread_mdelay(100);
//    }
//    }
///* ִ �� �� �� �� �� �� �� */
//    rt_mb_detach(&mb);
//}
//
//ALIGN(RT_ALIGN_SIZE)
//static char thread2_stack[1024];
//static struct rt_thread thread2;
///* �� �� 2 �� �� */
//static void thread2_entry(void *parameter)
//{
//    rt_uint8_t count;
//    count = 0;
//    while (count < 10)
//    {
//        count ++;
//        if (count & 0x1)
//        {
///* �� �� mb_str1 �� ַ �� �� �� �� */
//            rt_mb_send(&mb, (rt_uint32_t)&mb_str1);
//        }
//        else
//        {
///* �� �� mb_str2 �� ַ �� �� �� �� */
//            rt_mb_send(&mb, (rt_uint32_t)&mb_str2);
//        }
///* �� ʱ 200ms */
//        rt_thread_mdelay(200);
//    }
///* �� �� �� �� �� �� �� �� 1�� �� �� 2 �� �� �� �� �� �� */
//rt_mb_send(&mb, (rt_uint32_t)&mb_str3);
//}
//
//int mailbox_sample(void)
//{
//    rt_err_t result;
///* �� ʼ �� һ �� mailbox */
//    result = rt_mb_init(&mb,
//                        "mbt", /* �� �� �� mbt */
//                        &mb_pool[0], /* �� �� �� �� �� �� �� �� �� mb_pool */
//                        sizeof(mb_pool) / 4, /* �� �� �� �� �� �� �� Ŀ�� �� Ϊ һ �� ��
//                                                                �� ռ 4 �� �� */
//                        RT_IPC_FLAG_FIFO); /* �� �� FIFO �� ʽ �� �� �� �� �� �� */
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
///* �� �� �� msh �� �� �� �� �� */
//MSH_CMD_EXPORT(mailbox_sample, mailbox sample);


/*******************************************************************��Ϣ����**********************************************/

//#define THREAD_PRIORITY 25
//#define THREAD_TIMESLICE 5
///* �� Ϣ �� �� �� �� �� */
//static struct rt_messagequeue mq;
///* �� Ϣ �� �� �� �� �� �� �� �� �� Ϣ �� �� �� �� */
//static rt_uint8_t msg_pool[2048];
//
////�߳� thread1 ʹ�õ�ջ�ռ䡢�߳̿��ƿ飬�Լ��߳� thread1 ����ں�����
////ÿ 50ms ����Ϣ��������ȡ��Ϣ������ӡ��Ϣ���ݣ�20 �κ������

//ALIGN(RT_ALIGN_SIZE)
//static char thread1_stack[1024];
//static struct rt_thread thread1;
///* �� �� 1 �� �� �� �� */
//static void thread1_entry(void *parameter)
//{
//    char buf = 0;
//    rt_uint8_t cnt = 0;
//    while (1)
//    {
///* �� �� Ϣ �� �� �� �� �� �� Ϣ */
//        if (rt_mq_recv(&mq, &buf, sizeof(buf), RT_WAITING_FOREVER) == RT_EOK)
//        {
//            rt_kprintf("thread1: recv msg from msg queue, the content:%c\r\n", buf);
//            if (cnt == 19)
//            {
//                break;
//            }
//        }
///* �� ʱ 50ms */
//        cnt++;
//        rt_thread_mdelay(50);
//    }
//    rt_kprintf("thread1: detach mq \r\n");
//    rt_mq_detach(&mq);
//}
//
////�߳� thread2 ʹ�õ�ջ�ռ䡢�߳̿��ƿ飬�Լ��߳� thread2 ����ں�����
////ÿ 5ms ����Ϣ�����з�����Ϣ������ӡ��Ϣ���ݣ�20 �κ����
//ALIGN(RT_ALIGN_SIZE)
//static char thread2_stack[1024];
//static struct rt_thread thread2;
///* �� �� 2 �� �� */
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
//    /* �� �� �� �� �� Ϣ �� �� Ϣ �� �� �� */
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
//    else if (cnt>= 20)/* �� �� 20 �� �� Ϣ ֮ �� �� �� */
//    {
//    rt_kprintf("message queue stop send, thread2 quit\r\n");
//    break;
//    }
//    else
//    {
//    /* �� �� �� Ϣ �� �� Ϣ �� �� �� */
//    result = rt_mq_send(&mq, &buf, 1);
//    if (result != RT_EOK)
//    {
//    rt_kprintf("rt_mq_send ERR\r\n");
//    }
//    rt_kprintf("thread2: send message - %c\r\n", buf);
//    }
//    buf++;
//    cnt++;
//    /* �� ʱ 5ms */
//    rt_thread_mdelay(5);
//    }
//}
//
////��Ϣ���е�ʾ�����룬��ʼ����һ����Ϣ���У���ʼ���������� thread1 �� thread2.
//// ��������ʹ��MSH_CMD_EXPORT �������
//
///* �� Ϣ �� �� ʾ �� �� �� ʼ �� */
//int msgq_sample(void)
//{
//    rt_err_t result;
///* �� ʼ �� �� Ϣ �� �� */
//    result = rt_mq_init(&mq,
//        "mqt",
//        &msg_pool[0], /* �� �� �� ָ �� msg_pool */
//        1, /* ÿ �� �� Ϣ �� �� С �� 1 �� �� */
//        sizeof(msg_pool), /* �� �� �� �� �� С �� msg_pool �� ��
//        С */
//        RT_IPC_FLAG_FIFO); /* �� �� �� �� �� �� �� �� ���� �� �� �� ��
//        �� �� �� �� �� �� �� �� �� Ϣ */
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
///* �� �� �� msh �� �� �� �� �� */
//MSH_CMD_EXPORT(msgq_sample, msgq sample);

/*******************************************************�����̹߳��Ӻ���****************************************************/
//�����̹߳��Ӻ���������ϵͳ�ڿ��е�ʱ��ִ��һЩ�ǽ�������

//#define THREAD_PRIORITY      20
//#define THREAD_STACK_SIZE    1024
//#define THREAD_TIMESLICE     5
//
///* ָ���߳̿��ƿ��ָ�� */
//static rt_thread_t tid = RT_NULL;
//
///* ���к������Ӻ���ִ�д��� */
//volatile static int hook_times = 0;
//
///* �����̹߳��Ӻ��� */
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
///* �߳���� */
//static void thread_entry(void *parameter)
//{
//    int i = 5;
//    while (i--)
//    {
//        hook_times = 0;
//        /* ����500ms */
//        rt_kprintf("thread1 delay 50 OS Tick.\n", hook_times);
//        rt_thread_mdelay(500);
//    }
//    rt_kprintf("delete idle hook.\n");
//
//    /* ɾ�����й��Ӻ��� */
//    rt_thread_idle_delhook(idle_hook);
//    rt_kprintf("thread1 finish.\n");
//}
//
//int idle_hook_sample(void)
//{
//    /* ���ÿ����̹߳��� */
//    rt_thread_idle_sethook(idle_hook);
//
//    /* �����߳� */
//    tid = rt_thread_create("thread1",
//                           thread_entry, RT_NULL,
//                           THREAD_STACK_SIZE,
//                           THREAD_PRIORITY, THREAD_TIMESLICE);
//    if (tid != RT_NULL)
//        rt_thread_startup(tid);
//
//    return 0;
//}
///* ������ msh �����б��� */
//MSH_CMD_EXPORT(idle_hook_sample, idle hook sample);


/********************************************************ϵͳ���ȹ��Ӻ���*******************************************************/
//������ӷ��ˣ���ҪҲ��

//������Ӻ�����ϵͳ���������л�ʱ���У�ͨ��������Ӻ�����
//���ǿ����˽⵽ϵͳ�������ʱ��һЩ��Ϣ��
//ϵͳ���ȹ��Ӻ���ֻ�����ã�û��ɾ����
//���ҺͿ����̹߳��Ӻ�����ͬ���ǣ�ϵͳ���ȹ��Ӻ���ֻ������1��


///* ���ÿ���̵߳ļ����� */
//volatile rt_uint32_t count[2];
//
//
///* �߳�1��2����һ����ڣ�����ڲ�����ͬ */
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
//        /* �ر��ж� */
//        rt_kprintf("thread %d is running \n", value);
//        rt_thread_mdelay(1000); //��ʱһ��ʱ��
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
//    /* ���õ��������� */
//    rt_scheduler_sethook(hook_of_scheduler); /* ���뺯����������ָ�� */
//
//    /* �����߳�1 */
//    tid1 = rt_thread_create("thread1",
//                            thread_entry, (void *)1,
//                            THREAD_STACK_SIZE,
//                            THREAD_PRIORITY, THREAD_TIMESLICE);
//    if (tid1 != RT_NULL)
//        rt_thread_startup(tid1);
//
//    /* �����߳�2 */
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
///* ������ msh �����б��� */
//MSH_CMD_EXPORT(scheduler_hook, scheduler_hook sample);


////ע��Shell����
///* �������������� */
//void hellooo(int argc, char* argv[])
//{
//    rt_kprintf("hello RT-Thread!\n");
//}
////MSH_CMD_EXPORT(���ú��� , ˵��);
//MSH_CMD_EXPORT(hellooo , say hello to RT-Thread);
//
////�����в���������ʱ�����������Ϊ int argc �� char**argv��
////argc ��ʾ�����ĸ�����argv ��ʾ�����в����ַ���ָ������ָ��
//static void atcmd(int argc, char**argv)
//{
//
//}
//
//MSH_CMD_EXPORT(atcmd, atcmd sample: atcmd <server|client>);

//rt_kprintf("static thread 1 is running.\n");














