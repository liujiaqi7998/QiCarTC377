/*
 1.  I2Cdev�⼯��-��I2C�豸��ͷ�ļ�
 2. ��λ���ֽ�I2C R / W��������Ϊһ���������
 3. 2013��6��5�գ��ܷ��޲���Jeff Rowberg��<jeff@rowberg.net>

 ������־��
 1. 2015��10��30��-simondlevy��ΪTeensy3.1֧��i2c_t3
 2. 2013��5��6��-���Francesco Ferrara��Fastwire v0.24ʵ�֣�������һЩС�޸�
 3. 2013��5��5��-����˽�λֵд���ֵ����⣨Sasquatch / Farzanegan��
 4. 2012-06-09-�������Ҫ���⣬ʹ��Arduino Wireһ�ζ�ȡ> 32���ֽ�
 ʹ�ù�ʱ�Ļ�IDE�����޵�I2Cdevʵ��ʱ��ӱ���������
 6. 2011-11-01-�޸�д��*λ������㣨��лsasquatch @ Arduino��̳��
 7. 2011-10-03-������Զ�Arduino�汾��⹦�ܣ�����ʹ��
 8. 2011��10��2��-�����Gene Knight��NBWire TwoWire��ʵ�֣��������������޸�
 9. 2011��8��31��-�����˶�Arduino 1.0 Wire���֧�֣�������0.x��ͬ��
 10. 2011��3��3��-Ϊ��ȡ*��������˿�ѡ�ĳ�ʱ�����������ɸ���Ĭ��ֵ
 11. 2011��8��2��-�����˶�16λ�Ĵ�����֧��
 -�޸���ĳЩ�����ϲ���ȷ��Doxygenע��
 -Ϊ��ȡ��������˳�ʱֵ����лmem @ Arduino��̳��
 12. 2011��7��30��-�����˶�/д�����ṹ�Է��سɹ����ֽ���
 -�����з�����Ϊ��̬�Խ�ʡ���豸�ڴ�
 13. 2011��7��28��-��ʼ�汾
 */

#ifndef _I2CDEV_H_
#define _I2CDEV_H_

#include "headfile.h"
#include "Ard_define.h"

// 1000ms default read timeout (modify with "I2Cdev::readTimeout = [ms];")
// 1000msĬ�϶�ȡ��ʱ���á� I2Cdev :: readTimeout = [ms];�޸ġ���
#define I2CDEV_DEFAULT_READ_TIMEOUT     1000

class I2Cdev
{
    public:
        I2Cdev ();

        static int8_t readBit (uint8_t devAddr, uint8_t regAddr, uint8_t bitNum, uint8_t *data, uint16_t timeout =
                I2Cdev::readTimeout);
        static int8_t readBitW (uint8_t devAddr, uint8_t regAddr, uint8_t bitNum, uint16_t *data, uint16_t timeout =
                I2Cdev::readTimeout);
        static int8_t readBits (uint8_t devAddr, uint8_t regAddr, uint8_t bitStart, uint8_t length, uint8_t *data,
                uint16_t timeout = I2Cdev::readTimeout);
        static int8_t readBitsW (uint8_t devAddr, uint8_t regAddr, uint8_t bitStart, uint8_t length, uint16_t *data,
                uint16_t timeout = I2Cdev::readTimeout);
        static int8_t readByte (uint8_t devAddr, uint8_t regAddr, uint8_t *data,
                uint16_t timeout = I2Cdev::readTimeout);
        static int8_t readWord (uint8_t devAddr, uint8_t regAddr, uint16_t *data,
                uint16_t timeout = I2Cdev::readTimeout);
        static int8_t readBytes (uint8_t devAddr, uint8_t regAddr, uint8_t length, uint8_t *data, uint16_t timeout =
                I2Cdev::readTimeout);
        static int8_t readWords (uint8_t devAddr, uint8_t regAddr, uint8_t length, uint16_t *data, uint16_t timeout =
                I2Cdev::readTimeout);

        static bool writeBit (uint8_t devAddr, uint8_t regAddr, uint8_t bitNum, uint8_t data);
        static bool writeBitW (uint8_t devAddr, uint8_t regAddr, uint8_t bitNum, uint16_t data);
        static bool writeBits (uint8_t devAddr, uint8_t regAddr, uint8_t bitStart, uint8_t length, uint8_t data);
        static bool writeBitsW (uint8_t devAddr, uint8_t regAddr, uint8_t bitStart, uint8_t length, uint16_t data);
        static bool writeByte (uint8_t devAddr, uint8_t regAddr, uint8_t data);
        static bool writeWord (uint8_t devAddr, uint8_t regAddr, uint16_t data);
        static bool writeBytes (uint8_t devAddr, uint8_t regAddr, uint8_t length, uint8_t *data);
        static bool writeWords (uint8_t devAddr, uint8_t regAddr, uint8_t length, uint16_t *data);

        static uint16_t readTimeout;
};

#endif /* _I2CDEV_H_ */
