/*
 * LCD_Show.cpp
 *
 *  Created on: 2022��7��10��
 *      Author: liujiaqi
 */

#include "LCD_Show.h"

// SPI�����б�
SPI_Param SPI_param = {
    SPI_2,           //����ʹ�õ�SPI��
    SPI2_CS0_P15_2,  //����SPI_CS����
    SPI2_SCLK_P15_3, //����SPI_SCK����
    SPI2_MOSI_P15_5, //����SPI_MOSI����
    SPI2_MISO_P15_4, //����SPI_MISO����
    SPI_Mode_0,      // SPIģʽ(��4��)
    48 * 1000 * 1000 // SPI������
};

// IPS_114�����б�
IPS_114_Param IPS_114_param = {
    P15_4,      //Һ���������Ŷ��� ����ʵ��ͨѶδʹ��P15_4��� ���︴��ΪGPIO
    P15_1,      //Һ����λ���Ŷ���
    P15_0,      //Һ������λ���Ŷ���
    IPS_RED,    //����д�ֱʵ���ɫ
    IPS_WHITE,  //���屳����ɫ
    IPS114_W,   //����IPS114�����
    IPS114_H,   //����IPS114���߶�
    Direction3, //����IPS114��ʾ����
};

IPS_114 Screen;

//------------------------------------------------------------
// @brief       LCD�߳����
// @param       parameter   ����
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
// @brief       LCD��ʼ��
// @param       parameter   ����
// @return      void
// Sample usage:
//------------------------------------------------------------
void LCD_Show_int()
{
    //��ʼ������
    Screen.Set_Param(IPS_114_param, SPI_param);
    //��Ļ��ʼ��
    Screen.Init();

    // �߳̿��ƿ�ָ��
    rt_thread_t tid1;
    // ������̬�߳�
    tid1 = rt_thread_create("LCD_Show_thread",                      // �߳�����
        LCD_Show_thread_entry,                                      // �߳���ں���
        RT_NULL,                                            // �̲߳���
        512,                                                // 512 ���ֽڵ�ջ�ռ�
        20,                                                  // �߳����ȼ�Ϊ5����ֵԽС�����ȼ�Խ�ߣ�0Ϊ������ȼ���
                                                            // ����ͨ���޸�rt_config.h�е�RT_THREAD_PRIORITY_MAX�궨��(Ĭ��ֵΪ8)���޸����֧�ֵ����ȼ�
        5);                                                 // ʱ��ƬΪ5

    rt_kprintf("create LCD_Show_thread thread.\n");
    if(tid1 != RT_NULL)                                     // �̴߳����ɹ�
    {
        rt_kprintf("thread1 LCD_Show_thread thread create OK.\n");
        rt_thread_startup(tid1);                            // ���и��߳�
    }
    else                                                    // �̴߳���ʧ��
    {
        rt_kprintf("thread1 LCD_Show_thread thread create ERROR.\n");
    }

}
