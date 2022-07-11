#ifndef __QI_MPU6050_H
#define __QI_MPU6050_H
#include "headfile.h"
#include "math.h"

#define SMPLRT_DIV              0x19    //陀螺仪采样率，典型值：0x07(125Hz)
#define MPU6050_CONFIG          0x1A    //低通滤波频率，典型值：0x06(5Hz)
#define GYRO_CONFIG             0x1B    //陀螺仪自检及测量范围，典型值：0x18(不自检，2000deg/s)
#define ACCEL_CONFIG            0x1C    //加速计自检、测量范围及高通滤波频率，典型值：0x01(不自检，2G，5Hz)
#define INT_PIN_CFG             0x37    //设置6050辅助I2C为直通模式寄存器
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
#define User_Control            0x6A    //关闭6050对辅助I2C设备的控制
#define PWR_MGMT_1              0x6B    //电源管理，典型值：0x00(正常启用)
#define WHO_AM_I                0x75    //IIC地址寄存器(默认数值0x68，只读)

class Qi_MPU6050
{
    private:
        uint8 mpu_address = 0xD0>>1 ; //IIC写入时的地址字节数据，+1为读取
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
