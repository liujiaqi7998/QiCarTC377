/*
 * memory.h
 *
 *  Created on: 2021年11月28日
 *      Author: huyongliang
 */

#ifndef CODE_MEMORY_H_
#define CODE_MEMORY_H_

#include "rtthread.h"

//动态内存堆
void thread1_entry(void *parameter);
int dynmem_sample(void);

//内存池
int mempool_sample(void);


//void rt_system_heap_init(void *begin_addr,void *end_addr)
//从内存堆中申请用户指定大小的内存块

//void* rt_malloc(rt_size_t size)
//从系统堆空间中找到合适大小的内存块，然后把内存的地址作为rt_malloc的返回值返回给用户

//void* rt_realloc(void *rmem,rt_size_t newsize)
//函数注释：为用户重新分配内存块的大小，原来内存块rmem的数据内容保持不变，缩小的情况下，后面的数据被自动截断丢弃。申请成功后，rmem先前申请的内存块系统自动释放。

//void* rt_calloc(rt_size_t count,re_size_t size)
//函数注释：为用户申请连续内存地址的多个内存块，并且自动进行内存的初始化为0。
//count 内存块数量   size 内存块容量

//void re_free(void *ptr)
//函数注释：为用户将malloc、realloc、calloc申请的内存释放，每次申请内存用完后必须进行释放，否则会造成内存泄漏。
//小结：malloc和calloc的区别，都是向堆中申请内存，malloc申请的内存可能不是连续的，可能是以链表形式存在于堆中，calloc申请的内存必须的连续的内存块，calloc较malloc不同的是calloc会将申请的堆内存自动初始化为0，初始化效率比malloc手动初始化速度会快，在相同堆空间下，calloc申请的内存有很大可能是没有malloc申请的堆空间大。

//void rt_malloc_sethook(void (*hook)(void *ptr,rt_size_t size));
//函数注释：设置钩子函数会在内存分配完成后将申请到的内存块和大小传进hook函数中

//void hook(void *ptr,rt_size_t size);
//函数注释：rt_malloc_sethook钩子函数的函数指针
//ptr 分配到的内存块指针   size 分配到的内存块指针大小

//void rt_free_sethook(void (*hook)(void *ptr));
//函数注释：设置钩子函数会在内存释放完成前进行回调，将用户释放的内存地址块作为参数传递进去(此时内存块没有被释放)

//void hook(void *ptr);
//函数描述：rt_free_sethook钩子函数的函数指针
//ptr 待释放堆内存块指针






#endif /* CODE_MEMORY_H_ */
