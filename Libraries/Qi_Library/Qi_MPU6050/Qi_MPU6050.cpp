#include "Qi_MPU6050.h"
#include "Motor_run.h"

/**
 * @brief       MPU6050空构造函数
 * @param       None
 * @retval      None
 */
Qi_MPU6050::Qi_MPU6050 ()
{

}

/**
 * @brief       MPU6050带参构造函数
 * @param       dev_add         IIC写入时的地址字节数据，+1为读取
 * @retval      None
 */
Qi_MPU6050::Qi_MPU6050 (uint8 dev_add)
{
    mpu_address = dev_add;
}

/*
 * @brief       MPU6050初始化
 * @param       null
 * @return      void
 */
void Qi_MPU6050::Init ()
{
    simiic_init();
    systick_delay_ms(STM0, 100);                                   //上电延时

    self1_check();
    simiic_write_reg(MPU6050_DEV_ADDR, PWR_MGMT_1, 0x00);   //解除休眠状态
    simiic_write_reg(MPU6050_DEV_ADDR, SMPLRT_DIV, 0x07);   //125HZ采样率
    simiic_write_reg(MPU6050_DEV_ADDR, MPU6050_CONFIG, 0x04);       //
    simiic_write_reg(MPU6050_DEV_ADDR, GYRO_CONFIG, 0x18);  //2000
    simiic_write_reg(MPU6050_DEV_ADDR, ACCEL_CONFIG, 0x10); //8g
    simiic_write_reg(MPU6050_DEV_ADDR, User_Control, 0x00);
    simiic_write_reg(MPU6050_DEV_ADDR, INT_PIN_CFG, 0x02);
}

/*
 * @brief       MPU6050循环处理
 * @param       null
 * @return      void
 */
void Qi_MPU6050::loop ()
{
    get_accdata();
    get_gyro();
    if ((original_gyro_y / 100) > 10 && add_clock == 0)
    {
        add_clock = 1;
        add_speed(LEFT_Motor, 20);
        add_speed(RIGHT_Motor, 20);
        printf("is add \n");
    }

    if ((original_gyro_y / 100) < -10 && add_clock == 1)
    {
        add_clock = 0;
        reduce_speed(LEFT_Motor, 20);
        reduce_speed(RIGHT_Motor, 20);
        printf("is reduce \n");
    }

    //printf("%ld   %ld   %ld\n", (original_gyro_x / 100), (original_gyro_y / 100), (original_gyro_z / 100));
}

/*
 * @brief       MPU6050自检函数
 * @param       null
 * @return      void
 */
void Qi_MPU6050::self1_check ()
{
    simiic_write_reg(MPU6050_DEV_ADDR, PWR_MGMT_1, 0x00);   //解除休眠状态
    simiic_write_reg(MPU6050_DEV_ADDR, SMPLRT_DIV, 0x07);   //125HZ采样率
    while (0x07 != simiic_read_reg(MPU6050_DEV_ADDR, SMPLRT_DIV, SIMIIC))
    {
        //卡在这里原因有以下几点
        //1 MPU6050坏了，如果是新的这样的概率极低
        //2 接线错误或者没有接好
        //3 可能你需要外接上拉电阻，上拉到3.3V
        //4 可能没有调用模拟IIC的初始化函数
    }
}

/*
 *  @brief      获取MPU6050加速度计数据
 *  @param      NULL
 *  @return     void
 */
void Qi_MPU6050::get_accdata ()
{
    uint8 dat[6];

    simiic_read_regs(MPU6050_DEV_ADDR, ACCEL_XOUT_H, dat, 6, SIMIIC);
    original_acc_x = (int16) (((uint16) dat[0] << 8 | dat[1]));
    original_acc_y = (int16) (((uint16) dat[2] << 8 | dat[3]));
    original_acc_z = (int16) (((uint16) dat[4] << 8 | dat[5]));
}

/*
 *  @brief      获取MPU6050陀螺仪数据
 *  @param      NULL
 *  @return     void
 *  @since      v1.0
 *  Sample usage:               执行该函数后，直接查看对应的变量即可
 */
void Qi_MPU6050::get_gyro ()
{
    uint8 dat[6];

    simiic_read_regs(MPU6050_DEV_ADDR, GYRO_XOUT_H, dat, 6, SIMIIC);
    original_gyro_x = (int16) (((uint16) dat[0] << 8 | dat[1]));
    original_gyro_y = (int16) (((uint16) dat[2] << 8 | dat[3]));
    original_gyro_z = (int16) (((uint16) dat[4] << 8 | dat[5]));
}

