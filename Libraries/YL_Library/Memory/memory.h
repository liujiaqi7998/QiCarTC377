/*
 * memory.h
 *
 *  Created on: 2021��11��28��
 *      Author: huyongliang
 */

#ifndef CODE_MEMORY_H_
#define CODE_MEMORY_H_

#include "rtthread.h"

//��̬�ڴ��
void thread1_entry(void *parameter);
int dynmem_sample(void);

//�ڴ��
int mempool_sample(void);


//void rt_system_heap_init(void *begin_addr,void *end_addr)
//���ڴ���������û�ָ����С���ڴ��

//void* rt_malloc(rt_size_t size)
//��ϵͳ�ѿռ����ҵ����ʴ�С���ڴ�飬Ȼ����ڴ�ĵ�ַ��Ϊrt_malloc�ķ���ֵ���ظ��û�

//void* rt_realloc(void *rmem,rt_size_t newsize)
//����ע�ͣ�Ϊ�û����·����ڴ��Ĵ�С��ԭ���ڴ��rmem���������ݱ��ֲ��䣬��С������£���������ݱ��Զ��ض϶���������ɹ���rmem��ǰ������ڴ��ϵͳ�Զ��ͷš�

//void* rt_calloc(rt_size_t count,re_size_t size)
//����ע�ͣ�Ϊ�û����������ڴ��ַ�Ķ���ڴ�飬�����Զ������ڴ�ĳ�ʼ��Ϊ0��
//count �ڴ������   size �ڴ������

//void re_free(void *ptr)
//����ע�ͣ�Ϊ�û���malloc��realloc��calloc������ڴ��ͷţ�ÿ�������ڴ�������������ͷţ����������ڴ�й©��
//С�᣺malloc��calloc�����𣬶�������������ڴ棬malloc������ڴ���ܲ��������ģ���������������ʽ�����ڶ��У�calloc������ڴ������������ڴ�飬calloc��malloc��ͬ����calloc�Ὣ����Ķ��ڴ��Զ���ʼ��Ϊ0����ʼ��Ч�ʱ�malloc�ֶ���ʼ���ٶȻ�죬����ͬ�ѿռ��£�calloc������ڴ��кܴ������û��malloc����Ķѿռ��

//void rt_malloc_sethook(void (*hook)(void *ptr,rt_size_t size));
//����ע�ͣ����ù��Ӻ��������ڴ������ɺ����뵽���ڴ��ʹ�С����hook������

//void hook(void *ptr,rt_size_t size);
//����ע�ͣ�rt_malloc_sethook���Ӻ����ĺ���ָ��
//ptr ���䵽���ڴ��ָ��   size ���䵽���ڴ��ָ���С

//void rt_free_sethook(void (*hook)(void *ptr));
//����ע�ͣ����ù��Ӻ��������ڴ��ͷ����ǰ���лص������û��ͷŵ��ڴ��ַ����Ϊ�������ݽ�ȥ(��ʱ�ڴ��û�б��ͷ�)

//void hook(void *ptr);
//����������rt_free_sethook���Ӻ����ĺ���ָ��
//ptr ���ͷŶ��ڴ��ָ��






#endif /* CODE_MEMORY_H_ */
