#ifndef __QI_MPU6050_H
#define __QI_MPU6050_H
#include "headfile.h"
#include "math.h"

#define SMPLRT_DIV              0x19    //�����ǲ����ʣ�����ֵ��0x07(125Hz)
#define MPU6050_CONFIG          0x1A    //��ͨ�˲�Ƶ�ʣ�����ֵ��0x06(5Hz)
#define GYRO_CONFIG             0x1B    //�������Լ켰������Χ������ֵ��0x18(���Լ죬2000deg/s)
#define ACCEL_CONFIG            0x1C    //���ټ��Լ졢������Χ����ͨ�˲�Ƶ�ʣ�����ֵ��0x01(���Լ죬2G��5Hz)
#define INT_PIN_CFG             0x37    //����6050����I2CΪֱͨģʽ�Ĵ���
#define ACCEL_XOUT_H            0x3B
#define ACCEL_XOUT_L            0x3C
#define ACCEL_YOUT_H            0x3D
#define ACCEL_YOUT_L            0x3E
#define ACCEL_ZOUT_H            0x3F
#define ACCEL_ZOUT_L            0x40
#define GYRO_XOUT_H             0x43
#define GYRO_XOUT_L             0x44
#define GYRO_YOUT_H             0x45
#define GYRO_YOUT_L             0x46
#define GYRO_ZOUT_H             0x47
#define GYRO_ZOUT_L             0x48
#define User_Control            0x6A    //�ر�6050�Ը���I2C�豸�Ŀ���
#define PWR_MGMT_1              0x6B    //��Դ��������ֵ��0x00(��������)
#define WHO_AM_I                0x75    //IIC��ַ�Ĵ���(Ĭ����ֵ0x68��ֻ��)

class Qi_MPU6050
{
    private:
        uint8 mpu_address = 0xD0>>1 ; //IICд��ʱ�ĵ�ַ�ֽ����ݣ�+1Ϊ��ȡ
        int16 original_acc_x,original_acc_y,original_acc_z;
        int16 original_gyro_x,original_gyro_y,original_gyro_z;
        int add_clock = 0;
    public:
        Qi_MPU6050 ();
        Qi_MPU6050 (uint8 dev_add);
        void Init ();
        void loop();
        void self1_check();
        void get_accdata();
        void get_gyro();
};

#endif /*__QI_MPU6050_H*/
