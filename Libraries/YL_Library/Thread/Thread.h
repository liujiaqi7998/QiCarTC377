///*
// * thread.h
// *
// *  Created on: 2021��10��31��
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
////tc264Ӧ��ֻ��֧��32�����ȼ�
//#define THREAD_STACK_SIZE 512
//#define THREAD_TIMESLICE 5
////ʱ��Ƭ��ֻ���߳����ȼ���ͬʱ��������
////ʱ��ƬԼ�������߳�����ʱ����
////��λ��1��OS tick
//#define MAXSEM 5
//
////������̬�߳�
////��д�̴߳���-�����߳�-�����߳�
//
////������̬�߳�
////�����̶߳�-�����߳̿��ƿ�
////��д�̴߳���-�����߳�-�����߳�
//
////��ϵͳ��ʹ������RAM�����߳�ʱ���߳̿��ƿ���߳�ջ������������
////��̬�߳��ٶȲ����ܵ�Ӱ��
//
////�����̵߳Ĺ��Ӻ���Ҫע���κ�ʱ�̶����ᱻ����
////�����߳̿�������4�����Ӻ���
//
//
////�ź���
//int producer_consumer(void);
//
////������
//int pri_inversion(void);
//
////�¼���
//int event_sample(void);
//
////����
//int mailbox_sample(void);
//
////��Ϣ����
//int msgq_sample(void);
//
////�����߳�
////���Ӻ�����ִ�е���ش�����뱣֤�����߳����κ�ʱ�̶����ᱻ����
//int idle_hook_sample(void);
//
////ϵͳ���ȹ��Ӻ���
//int scheduler_hook(void);
//
//
///*******************************************************************�̵߳���**********************************************/
////�����ر�ϵͳ����,�����ٽ���
///***********�ر��ж�**********///�ڹر��ж�ǰ��Ӧ�ȶ���������ж�״̬��¼������Ȼ�����ж�ʱ�ٽ��ж�״̬����
////�ж���
////    rt_base_t level;
////        /* �ر��ж� */
////        level = rt_hw_interrupt_disable();//ʹ���ж�����ͬ���ж�
////����������ڹر��жϲ����عر��ж�ǰ���ж�״̬��
////        /* �ָ��ж� */
////        rt_hw_interrupt_enable(level);
////�ָ��ر��ж�ǰ������״̬
//
//
///**********��ֹ����**********/
////����������
////void rt_enter_critical(void); /* �����ٽ���*/
////���ж�����ͬ��������ϵͳ��Ӧ�ⲿ�ж�
////����Ҫע���ٽ�������Դδ���޸�
//
////����������
////void rt_exit_critical(void); /* �˳��ٽ���*/
//
////�����ͽ���Ƕ��ʹ�ã�ÿ����һ�ξ�Ҫ��Ӧ�Ľ��������Ƕ��65535��
////ʹ�õ�������֮��ϵͳ�����پ������ȼ���ϵ
//
///***************************************************************�ź���****************************************************/
////�������Ա����ٽ���
//
////�ź������ƿ�
////struct rt_semaphore
////{
////   struct rt_ipc_object parent;/*�̳���ipc_object��*/
////   rt_uint16_t value;   /* �ź�����ֵ  */
////};
/////* rt_sem_t��ָ��semaphore�ṹ���ָ������ */
////typedef struct rt_semaphore* rt_sem_t;
//
////�����ź���
////rt_sem_t rt_sem_create (const char* name, rt_uint32_t value, rt_uint8_t flag);
//
////ɾ���ź���
////rt_err_t rt_sem_delete (rt_sem_t sem);
//
////��ʼ���ź�������
////rt_err_t rt_sem_init (rt_sem_t sem, const char* name, rt_uint32_t value, rt_uint8_t flag);
//
////�����ź���
////rt_err_t rt_sem_detach (rt_sem_t sem);
////ʹ�øú������ں��Ȼ������й��ڸ��ź����ȴ������ϵ��̣߳�Ȼ�󽫸��ź������ں˶����������ɾ����
//
////��ȡ�ź���
////rt_err_t rt_sem_take (rt_sem_t sem, rt_int32_t time);
//
////�޵ȴ���ȡ�ź���
////rt_err_t rt_sem_trytake(rt_sem_t sem);
//
////�ͷ��ź���
////rt_err_t rt_sem_release(rt_sem_t sem);
////���߳������Դ�ķ��ʺ�Ӧ�����ͷ������е��ź�����ʹ�������߳��ܻ�ø��ź���
//
//
///****************************************************************************������****************************************/
//
////����������
////rt_mutex_t rt_mutex_create (const char* name, rt_uint8_t flag);
//
////ɾ��������
////rt_err_t rt_mutex_delete (rt_mutex_t mutex);
//
////��ʼ��������
////rt_err_t rt_mutex_init (rt_mutex_t mutex, const char* name, rt_uint8_t flag);
////��̬������������ڴ�����ϵͳ����ʱ�ɱ���������ģ�һ��������ݶλ�ZI���С�
////��ʹ�����ྲ̬����������ǰ����Ҫ�Ƚ��г�ʼ����
//
//
////���뻥����
////rt_err_t rt_mutex_detach (rt_mutex_t mutex);
////ʹ�øú����ӿں��ں��Ȼ������й��ڸû������ϵ��̣߳�
////Ȼ��ϵͳ���û��������ں˶��������������ɾ����
//
//
////��ȡ������
////rt_err_t rt_mutex_take (rt_mutex_t mutex, rt_int32_t time);
////�߳�ͨ����������������ȡ������������Ȩ���̶߳Ի�����������Ȩ�Ƕ�ռ�ģ�
////ĳһ��ʱ��һ��������ֻ�ܱ�һ���̳߳��С�
//
////�ͷŻ�����
////rt_err_t rt_mutex_release(rt_mutex_t mutex);
////���߳���ɻ�����Դ�ķ��ʺ�Ӧ�����ͷ���ռ�ݵĻ�������ʹ�������߳��ܼ�ʱ��ȡ�û�������
//
///*************************************************************�¼�����****************************************************/
//
////�����¼�
////rt_event_t rt_event_create (const char* name, rt_uint8_t flag);
////���øú����ӿ�ʱ��ϵͳ��Ӷ�̬�ڴ���з����¼�����Ȼ����ж���ĳ�ʼ����IPC�����ʼ��������set���ó�0��
//
////ɾ���¼�
////rt_err_t rt_event_delete (rt_event_t event);
////ϵͳ����ʹ���¼�����ʱ��ͨ��ɾ���¼�������ƿ����ͷ�ϵͳ��Դ
//
////��ʼ���¼�
////rt_err_t rt_event_init(rt_event_t event, const char* name, rt_uint8_t flag);
////���øýӿ�ʱ����ָ����̬�¼�����ľ������ָ���¼����ƿ��ָ�룩��
////Ȼ��ϵͳ���ʼ���¼����󣬲����뵽ϵͳ���������н��й�����
//
////�����¼�
////rt_err_t rt_event_detach(rt_event_t event);
////�û������������ʱ��ϵͳ���Ȼ������й��ڸ��¼��ȴ������ϵ��̣߳�
////Ȼ�󽫸��¼����ں˶����������ɾ����
//
////�����¼�
////rt_err_t rt_event_recv(rt_event_t event, rt_uint32_t set, rt_uint8_t option,
////rt_int32_t timeout, rt_uint32_t* recved);
//
////�����¼�
////rt_err_t rt_event_send(rt_event_t event, rt_uint32_t set);
////ͨ������setָ�����¼���־���趨event������¼���־ֵ��Ȼ������ȴ���event�¼������ϵĵȴ��߳�������
////�ж��Ƿ����̵߳��¼�����Ҫ���뵱ǰevent�����¼���־ֵƥ�䣬����У����Ѹ��̡߳�
//
//
//
//
//
//
//
//#endif /* CODE_THREAD_THREAD_H_ */
//