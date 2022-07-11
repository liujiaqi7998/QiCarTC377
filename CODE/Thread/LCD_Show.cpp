/*
 * LCD_Show.cpp
 *
 *  Created on: 2022年7月10日
 *      Author: liujiaqi
 */

#include "LCD_Show.h"

// SPI参数列表
SPI_Param SPI_param = {
    SPI_2,           //定义使用的SPI号
    SPI2_CS0_P15_2,  //定义SPI_CS引脚
    SPI2_SCLK_P15_3, //定义SPI_SCK引脚
    SPI2_MOSI_P15_5, //定义SPI_MOSI引脚
    SPI2_MISO_P15_4, //定义SPI_MISO引脚
    SPI_Mode_0,      // SPI模式(共4种)
    48 * 1000 * 1000 // SPI波特率
};

// IPS_114参数列表
IPS_114_Param IPS_114_param = {
    P15_4,      //液晶背光引脚定义 由于实际通讯未使用P15_4因此 这里复用为GPIO
    P15_1,      //液晶复位引脚定义
    P15_0,      //液晶命令位引脚定义
    IPS_RED,    //定义写字笔的颜色
    IPS_WHITE,  //定义背景颜色
    IPS114_W,   //定义IPS114最大宽度
    IPS114_H,   //定义IPS114最大高度
    Direction3, //定义IPS114显示方向
};

IPS_114 Screen;

//------------------------------------------------------------
// @brief       LCD线程入口
// @param       parameter   参数
// @return      void
// Sample usage:
//------------------------------------------------------------
void LCD_Show_thread_entry(void *paremeter)
{
    rt_kprintf("\n LCD_Show thread begain! \n");
    while (1)
    {
        MT9V03x_Sendimg_To_IPS114(Screen, (uint8*) Result_image, MT9V03X_W, MT9V03X_H);
        rt_thread_delay(1);
    }
    rt_kprintf("\n  LCD_Show thread exit! \n");
}

//------------------------------------------------------------
// @brief       LCD初始化
// @param       parameter   参数
// @return      void
// Sample usage:
//------------------------------------------------------------
void LCD_Show_int()
{
    //初始化参数
    Screen.Set_Param(IPS_114_param, SPI_param);
    //屏幕初始化
    Screen.Init();

    // 线程控制块指针
    rt_thread_t tid1;
    // 创建动态线程
    tid1 = rt_thread_create("LCD_Show_thread",                      // 线程名称
        LCD_Show_thread_entry,                                      // 线程入口函数
        RT_NULL,                                            // 线程参数
        512,                                                // 512 个字节的栈空间
        20,                                                  // 线程优先级为5，数值越小，优先级越高，0为最高优先级。
                                                            // 可以通过修改rt_config.h中的RT_THREAD_PRIORITY_MAX宏定义(默认值为8)来修改最大支持的优先级
        5);                                                 // 时间片为5

    rt_kprintf("create LCD_Show_thread thread.\n");
    if(tid1 != RT_NULL)                                     // 线程创建成功
    {
        rt_kprintf("thread1 LCD_Show_thread thread create OK.\n");
        rt_thread_startup(tid1);                            // 运行该线程
    }
    else                                                    // 线程创建失败
    {
        rt_kprintf("thread1 LCD_Show_thread thread create ERROR.\n");
    }

}
