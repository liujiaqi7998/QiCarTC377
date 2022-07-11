/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2020,��ɿƼ�
 * All rights reserved.
 * ��������QQȺ����Ⱥ��824575535
 *
 * �����������ݰ�Ȩ������ɿƼ����У�δ����������������ҵ��;��
 * ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
 *
 * @file            stm_systick
 * @company         �ɶ���ɿƼ����޹�˾
 * @author          ��ɿƼ�(QQ3184284598)
 * @version         �鿴doc��version�ļ� �汾˵��
 * @Software        ADS v1.2.2
 * @Target core     TC377TP
 * @Taobao          https://seekfree.taobao.com/
 * @date            2020-11-23
 ********************************************************************************************************************/

#include "IfxStm.h"
#include "IFXSTM_CFG.h"
#include "zf_stm_systick.h"

static uint32 systick_count[3];

//-------------------------------------------------------------------------------------------------------------------
//  @brief      systick��ʱ����
//  @param      stmn            ѡ��ʹ�õ�ģ��
//  @param      time            ��ʱһ�ֵ�ʱ�䣨��λΪ���룬�����÷�Χ0-20000000��
//  @param      num             ��ʱ������
//  @return     void
//  Sample usage:               �����û����ã��û���ʹ��h�ļ��еĺ궨��
//-------------------------------------------------------------------------------------------------------------------
void systick_delay(STMN_enum stmn, uint32 time, uint32 num)
{
    if(stmn == STM0)                                                            // RTT ��ռ�� STM0 ��Ϊϵͳ��ʱ��
    {
        if(IfxCpu_getCoreId() == IfxCpu_Id_0)                                   // ������� CPU0 ���� STM0 ��ʱ ��ô�л��� RTT ����ʱ
        {
            rt_thread_mdelay(time / 1000000);                                   // ��֧�ֺ��뼶��ʱ ���뼶������ʱ����Ч
        }
        else                                                                    // ������� CPU1/2 ���� STM0 ��ʱ ��ô�л��� STM1 ����ʱ
        {
            uint32 stm_clk;
            uint32 delay_time = (uint32)(stm_clk/1000000*time/1000);
            stm_clk = IfxStm_getFrequency(IfxStm_getAddress((IfxStm_Index)STM1));
            while(num--)
            {
                IfxStm_waitTicks(IfxStm_getAddress((IfxStm_Index)STM1), delay_time);
            }
        }
    }
    else                                                                        // STM1/2 ����Ӱ����������
    {
        uint32 stm_clk;
        uint32 delay_time = (uint32)(stm_clk/1000000*time/1000);
        stm_clk = IfxStm_getFrequency(IfxStm_getAddress((IfxStm_Index)stmn));
        while(num--)
        {
            IfxStm_waitTicks(IfxStm_getAddress((IfxStm_Index)stmn), delay_time);
        }
    }
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      systick��ʱ������
//  @param      stmn            ѡ��ʹ�õ�ģ��
//  @return     void
//  Sample usage:               systick_start(STM0);//��¼�µ�ǰ��ʱ��
//-------------------------------------------------------------------------------------------------------------------
void systick_start(STMN_enum stmn)
{

    systick_count[stmn] = IfxStm_getLower(IfxStm_getAddress((IfxStm_Index)stmn));
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief     ��õ�ǰSystem tick timer��ֵ
//  @param     stmn             ѡ��ʹ�õ�ģ��
//  @return    uint32           ���شӿ�ʼ�����ڵ�ʱ��(��λ10ns)
//  Sample usage:               uint32 tim = systick_getval(STM0);
//-------------------------------------------------------------------------------------------------------------------
uint32 systick_getval(STMN_enum stmn)
{
    uint32 time;
    uint32 stm_clk;

    stm_clk = IfxStm_getFrequency(IfxStm_getAddress((IfxStm_Index)stmn));

    time = IfxStm_getLower(IfxStm_getAddress((IfxStm_Index)stmn)) - systick_count[stmn];
    time = (uint32)((uint64)time * 100000000 / stm_clk);
    return time;
}