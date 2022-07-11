/*
 1.  I2Cdev库集合-主I2C设备类头文件
 2. 将位和字节I2C R / W函数抽象为一个方便的类
 3. 2013年6月5日，杰夫·罗伯格（Jeff Rowberg）<jeff@rowberg.net>

 更新日志：
 1. 2015年10月30日-simondlevy：为Teensy3.1支持i2c_t3
 2. 2013年5月6日-添加Francesco Ferrara的Fastwire v0.24实现，并做了一些小修改
 3. 2013年5月5日-解决了将位值写入字的问题（Sasquatch / Farzanegan）
 4. 2012-06-09-解决了主要问题，使用Arduino Wire一次读取> 32个字节
 使用过时的或IDE或有限的I2Cdev实现时添加编译器警告
 6. 2011-11-01-修复写入*位掩码计算（感谢sasquatch @ Arduino论坛）
 7. 2011-10-03-添加了自动Arduino版本检测功能，易于使用
 8. 2011年10月2日-添加了Gene Knight的NBWire TwoWire类实现，并进行了少量修改
 9. 2011年8月31日-增加了对Arduino 1.0 Wire库的支持（方法与0.x不同）
 10. 2011年3月3日-为读取*方法添加了可选的超时参数，可轻松更改默认值
 11. 2011年8月2日-增加了对16位寄存器的支持
 -修复了某些方法上不正确的Doxygen注释
 -为读取操作添加了超时值（感谢mem @ Arduino论坛）
 12. 2011年7月30日-更改了读/写函数结构以返回成功或字节数
 -将所有方法设为静态以节省多设备内存
 13. 2011年7月28日-初始版本
 */

#ifndef _I2CDEV_H_
#define _I2CDEV_H_

#include "headfile.h"
#include "Ard_define.h"

// 1000ms default read timeout (modify with "I2Cdev::readTimeout = [ms];")
// 1000ms默认读取超时（用“ I2Cdev :: readTimeout = [ms];修改”）
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
