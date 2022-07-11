#include "Qi_MPU6050.h"
#include "Motor_run.h"

/**
 * @brief       MPU6050�չ��캯��
 * @param       None
 * @retval      None
 */
Qi_MPU6050::Qi_MPU6050 ()
{

}

/**
 * @brief       MPU6050���ι��캯��
 * @param       dev_add         IICд��ʱ�ĵ�ַ�ֽ����ݣ�+1Ϊ��ȡ
 * @retval      None
 */
Qi_MPU6050::Qi_MPU6050 (uint8 dev_add)
{
    mpu_address = dev_add;
}

/*
 * @brief       MPU6050��ʼ��
 * @param       null
 * @return      void
 */
void Qi_MPU6050::Init ()
{
    simiic_init();
    systick_delay_ms(STM0, 100);                                   //�ϵ���ʱ

    self1_check();
    simiic_write_reg(MPU6050_DEV_ADDR, PWR_MGMT_1, 0x00);   //�������״̬
    simiic_write_reg(MPU6050_DEV_ADDR, SMPLRT_DIV, 0x07);   //125HZ������
    simiic_write_reg(MPU6050_DEV_ADDR, MPU6050_CONFIG, 0x04);       //
    simiic_write_reg(MPU6050_DEV_ADDR, GYRO_CONFIG, 0x18);  //2000
    simiic_write_reg(MPU6050_DEV_ADDR, ACCEL_CONFIG, 0x10); //8g
    simiic_write_reg(MPU6050_DEV_ADDR, User_Control, 0x00);
    simiic_write_reg(MPU6050_DEV_ADDR, INT_PIN_CFG, 0x02);
}

/*
 * @brief       MPU6050ѭ������
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
 * @brief       MPU6050�Լ캯��
 * @param       null
 * @return      void
 */
void Qi_MPU6050::self1_check ()
{
    simiic_write_reg(MPU6050_DEV_ADDR, PWR_MGMT_1, 0x00);   //�������״̬
    simiic_write_reg(MPU6050_DEV_ADDR, SMPLRT_DIV, 0x07);   //125HZ������
    while (0x07 != simiic_read_reg(MPU6050_DEV_ADDR, SMPLRT_DIV, SIMIIC))
    {
        //��������ԭ�������¼���
        //1 MPU6050���ˣ�������µ������ĸ��ʼ���
        //2 ���ߴ������û�нӺ�
        //3 ��������Ҫ����������裬������3.3V
        //4 ����û�е���ģ��IIC�ĳ�ʼ������
    }
}

/*
 *  @brief      ��ȡMPU6050���ٶȼ�����
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
 *  @brief      ��ȡMPU6050����������
 *  @param      NULL
 *  @return     void
 *  @since      v1.0
 *  Sample usage:               ִ�иú�����ֱ�Ӳ鿴��Ӧ�ı�������
 */
void Qi_MPU6050::get_gyro ()
{
    uint8 dat[6];

    simiic_read_regs(MPU6050_DEV_ADDR, GYRO_XOUT_H, dat, 6, SIMIIC);
    original_gyro_x = (int16) (((uint16) dat[0] << 8 | dat[1]));
    original_gyro_y = (int16) (((uint16) dat[2] << 8 | dat[3]));
    original_gyro_z = (int16) (((uint16) dat[4] << 8 | dat[5]));
}

