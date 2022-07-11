// I2Cdev 库集合 - 主要 I2C 设备类
// 将位和字节 I2C R/W 函数抽象为一个方便的类
// 2013-06-05 作者 Jeff Rowberg <jeff@rowberg.net>
//
// 更新日志：
// 2013-05-06 - 添加 Francesco Ferrara 的 Fastwire v0.24 实现，并稍作修改
// 2013-05-05 - 修复了将位值写入字的问题（Sasquatch/Farzanegan）
// 2012-06-09 - 修复使用 Arduino Wire 一次读取 > 32 个字节的主要问题
// - 在使用过时或 IDE 或有限的 I2Cdev 实现时添加编译器警告
// 2011-11-01 - 修复 write*Bits 掩码计算（感谢 sasquatch @ Arduino 论坛）
// 2011-10-03 - 添加了自动 Arduino 版本检测以方便使用
// 2011-10-02 - 添加了 Gene Knight 的 NBWire TwoWire 类实现，稍作修改
// 2011-08-31 - 添加了对 Arduino 1.0 Wire 库的支持（方法与 0.x 不同）
// 2011-08-03 - 为 read* 方法添加了可选的超时参数，以便轻松更改默认值
// 2011-08-02 - 添加了对 16 位寄存器的支持
// - 修复了某些方法上不正确的 Doxygen 注释
// - 为读取操作添加超时值（感谢 mem @ Arduino 论坛）
// 2011-07-30 - 更改读/写函数结构以返回成功或字节数
// - 将所有方法设为静态以节省多设备内存
// 2011-07-28 - 初始版本

#include "I2Cdev.h"

/** Default constructor.
 */
I2Cdev::I2Cdev ()
{
}

/** 从 8 位设备寄存器中读取单个位。
 * @param devAddr I2C 从设备地址
 * @param regAddr 注册要读取的 regAddr
 * @param bitNum 要读取的位位置 (0-7)
 * @param data 单个位值的容器
 * @param timeout 以毫秒为单位的可选读取超时（0 表示禁用，保留使用 I2Cdev::readTimeout 中的默认类值）
 * @return 读操作状态（true = 成功）
 */
int8_t I2Cdev::readBit (uint8_t devAddr, uint8_t regAddr, uint8_t bitNum, uint8_t *data, uint16_t timeout)
{
    uint8_t b;
    uint8_t count = readByte(devAddr, regAddr, &b, timeout);
    *data = b & (1 << bitNum);
    return count;
}

/** 从 16 位设备寄存器中读取单个位。
 * @param devAddr I2C 从设备地址
 * @param regAddr 注册要读取的 regAddr
 * @param bitNum 要读取的位位置 (0-15)
 * @param data 单个位值的容器
 * @param timeout 以毫秒为单位的可选读取超时（0 表示禁用，保留使用 I2Cdev::readTimeout 中的默认类值）
 * @return 读操作状态（true = 成功）
 */
int8_t I2Cdev::readBitW (uint8_t devAddr, uint8_t regAddr, uint8_t bitNum, uint16_t *data, uint16_t timeout)
{
    uint16_t b;
    uint8_t count = readWord(devAddr, regAddr, &b, timeout);
    *data = b & (1 << bitNum);
    return count;
}

/** 从 8 位设备寄存器中读取多个位。
 * @param devAddr I2C 从设备地址
 * @param regAddr 注册要读取的 regAddr
 * @param bitStart 要读取的第一位位置 (0-7)
 * @param length 要读取的位数（不超过 8）
 * @param data 右对齐值的容器（即从任何 bitStart 位置读取的 '101' 将等于 0x05）
 * @param timeout 以毫秒为单位的可选读取超时（0 表示禁用，保留使用 I2Cdev::readTimeout 中的默认类值）
 * @return 读操作状态（true = 成功）
 */
int8_t I2Cdev::readBits (uint8_t devAddr, uint8_t regAddr, uint8_t bitStart, uint8_t length, uint8_t *data,
        uint16_t timeout)
{
    // 01101001 read byte
    // 76543210 bit numbers
    //    xxx   args: bitStart=4, length=3
    //    010   masked
    //   -> 010 shifted
    uint8_t count, b;
    if ((count = readByte(devAddr, regAddr, &b, timeout)) != 0)
    {
        uint8_t mask = ((1 << length) - 1) << (bitStart - length + 1);
        b &= mask;
        b >>= (bitStart - length + 1);
        *data = b;
    }
    return count;
}

/** 从 16 位设备寄存器中读取多个位。
 * @param devAddr I2C 从设备地址
 * @param regAddr 注册要读取的 regAddr
 * @param bitStart 要读取的第一位位置 (0-15)
 * @param length 要读取的位数（不超过 16）
 * @param data 右对齐值的容器（即从任何 bitStart 位置读取的 '101' 将等于 0x05）
 * @param timeout 以毫秒为单位的可选读取超时（0 表示禁用，保留使用 I2Cdev::readTimeout 中的默认类值）
 * @return 读操作状态（1 = 成功，0 = 失败，-1 = 超时）
 */
int8_t I2Cdev::readBitsW (uint8_t devAddr, uint8_t regAddr, uint8_t bitStart, uint8_t length, uint16_t *data,
        uint16_t timeout)
{
    // 1101011001101001 read byte
    // fedcba9876543210 bit numbers
    //    xxx           args: bitStart=12, length=3
    //    010           masked
    //           -> 010 shifted
    uint8_t count;
    uint16_t w;
    if ((count = readWord(devAddr, regAddr, &w, timeout)) != 0)
    {
        uint16_t mask = ((1 << length) - 1) << (bitStart - length + 1);
        w &= mask;
        w >>= (bitStart - length + 1);
        *data = w;
    }
    return count;
}

/** 从 8 位设备寄存器读取单个字节。
 * @param devAddr I2C 从设备地址
 * @param regAddr 注册要读取的 regAddr
 * @param data 从设备读取的字节值的容器
 * @param timeout 以毫秒为单位的可选读取超时（0 表示禁用，保留使用 I2Cdev::readTimeout 中的默认类值）
 * @return 读操作状态（true = 成功）
 */
int8_t I2Cdev::readByte (uint8_t devAddr, uint8_t regAddr, uint8_t *data, uint16_t timeout)
{
    return readBytes(devAddr, regAddr, 1, data, timeout);
}

/** 从 16 位设备寄存器中读取单个字。
 * @param devAddr I2C 从设备地址
 * @param regAddr 注册要读取的 regAddr
 * @param data 从设备读取的字值的容器
 * @param timeout 以毫秒为单位的可选读取超时（0 表示禁用，保留使用 I2Cdev::readTimeout 中的默认类值）
 * @return 读操作状态（true = 成功）
 */
int8_t I2Cdev::readWord (uint8_t devAddr, uint8_t regAddr, uint16_t *data, uint16_t timeout)
{
    return readWords(devAddr, regAddr, 1, data, timeout);
}

/** 从 8 位设备寄存器读取多个字节。
 * @param devAddr I2C 从设备地址
 * @param regAddr 首先注册要读取的 regAddr
 * @param length 要读取的字节数
 * @param data 用于存储读取数据的数据缓冲区
 * @param timeout 以毫秒为单位的可选读取超时（0 表示禁用，保留使用 I2Cdev::readTimeout 中的默认类值）
 * @return 读取的字节数（-1 表示失败）
 */
int8_t I2Cdev::readBytes (uint8_t devAddr, uint8_t regAddr, uint8_t length, uint8_t *data, uint16_t timeout)
{
    int8_t count = 0;
    uint32_t t1 = millis();
    simiic_read_regs(devAddr, regAddr, data, length, SIMIIC);
    // check for timeout
    if (timeout > 0 && millis() - t1 >= timeout && count < length)
        count = -1; // timeout
    return count;
}

/** 从 16 位设备寄存器中读取多个字。
 * @param devAddr I2C 从设备地址
 * @param regAddr 首先注册要读取的 regAddr
 * @param length 要读取的字数
 * @param data 用于存储读取数据的缓冲区
 * @param timeout 以毫秒为单位的可选读取超时（0 表示禁用，保留使用 I2Cdev::readTimeout 中的默认类值）
 * @return 读取的字数（-1 表示失败）
 */
int8_t I2Cdev::readWords (uint8_t devAddr, uint8_t regAddr, uint8_t length, uint16_t *data, uint16_t timeout)
{
    int8_t count = 0;
    uint32_t t1 = millis();

    bool msb = true; // starts with MSB, then LSB
    for (; count < length && (timeout == 0 || millis() - t1 < timeout);)
    {
        if (msb)
        {
            // first byte is bits 15-8 (MSb=15)
            data[count] = simiic_read_reg(devAddr, regAddr, SIMIIC) << 8;
        }
        else
        {
            // second byte is bits 7-0 (LSb=0)
            data[count] |= simiic_read_reg(devAddr, regAddr, SIMIIC);

            count++;
        }
        msb = !msb;
    }

    if (timeout > 0 && millis() - t1 >= timeout && count < length)
        count = -1; // timeout
    return count;
}

/** 在 8 位设备寄存器中写入一个位。
 * @param devAddr I2C 从设备地址
 * @param regAddr 注册要写入的 regAddr
 * @param bitNum 要写入的位位置 (0-7)
 * @param value 要写入的新位值
 * @return 操作状态（true = 成功）
 */

bool I2Cdev::writeBit (uint8_t devAddr, uint8_t regAddr, uint8_t bitNum, uint8_t data)
{
    uint8_t b;
    readByte(devAddr, regAddr, &b);
    b = (data != 0) ? (b | (1 << bitNum)) : (b & ~(1 << bitNum));
    return writeByte(devAddr, regAddr, b);
}

/** 在 16 位设备寄存器中写入单个位。
 * @param devAddr I2C 从设备地址
 * @param regAddr 注册要写入的 regAddr
 * @param bitNum 要写入的位位置 (0-15)
 * @param value 要写入的新位值
 * @return 操作状态（true = 成功）
 */
bool I2Cdev::writeBitW (uint8_t devAddr, uint8_t regAddr, uint8_t bitNum, uint16_t data)
{
    uint16_t w;
    readWord(devAddr, regAddr, &w);
    w = (data != 0) ? (w | (1 << bitNum)) : (w & ~(1 << bitNum));
    return writeWord(devAddr, regAddr, w);
}

/** 在 8 位设备寄存器中写入多个位。
 * @param devAddr I2C 从设备地址
 * @param regAddr 注册要写入的 regAddr
 * @param bitStart 要写入的第一位位置 (0-7)
 * @param length 要写入的位数（不超过 8）
 * @param data 要写入的右对齐值
 * @return 操作状态（true = 成功）
 */
bool I2Cdev::writeBits (uint8_t devAddr, uint8_t regAddr, uint8_t bitStart, uint8_t length, uint8_t data)
{
    //      010 value to write
    // 76543210 bit numbers
    //    xxx   args: bitStart=4, length=3
    // 00011100 mask byte
    // 10101111 original value (sample)
    // 10100011 original & ~mask
    // 10101011 masked | value
    uint8_t b;
    if (readByte(devAddr, regAddr, &b) != 0)
    {
        uint8_t mask = ((1 << length) - 1) << (bitStart - length + 1);
        data <<= (bitStart - length + 1); // shift data into correct position
        data &= mask; // zero all non-important bits in data
        b &= ~(mask); // zero all important bits in existing byte
        b |= data; // combine data with existing byte
        return writeByte(devAddr, regAddr, b);
    }
    else
    {
        return false;
    }
}

/** 在 16 位设备寄存器中写入多个位。
 * @param devAddr I2C 从设备地址
 * @param regAddr 注册要写入的 regAddr
 * @param bitStart 要写入的第一位位置 (0-15)
 * @param length 要写入的位数（不超过 16）
 * @param data 要写入的右对齐值
 * @return 操作状态（true = 成功）
 */
bool I2Cdev::writeBitsW (uint8_t devAddr, uint8_t regAddr, uint8_t bitStart, uint8_t length, uint16_t data)
{
    //              010 value to write
    // fedcba9876543210 bit numbers
    //    xxx           args: bitStart=12, length=3
    // 0001110000000000 mask word
    // 1010111110010110 original value (sample)
    // 1010001110010110 original & ~mask
    // 1010101110010110 masked | value
    uint16_t w;
    if (readWord(devAddr, regAddr, &w) != 0)
    {
        uint16_t mask = ((1 << length) - 1) << (bitStart - length + 1);
        data <<= (bitStart - length + 1); // shift data into correct position
        data &= mask; // zero all non-important bits in data
        w &= ~(mask); // zero all important bits in existing word
        w |= data; // combine data with existing word
        return writeWord(devAddr, regAddr, w);
    }
    else
    {
        return false;
    }
}

/** 将单个字节写入 8 位设备寄存器。
 * @param devAddr I2C 从设备地址
 * @param regAddr 注册地址写入
 * @param data 要写入的新字节值
 * @return 操作状态（true = 成功）
 */
bool I2Cdev::writeByte (uint8_t devAddr, uint8_t regAddr, uint8_t data)
{
    return writeBytes(devAddr, regAddr, 1, &data);
}

/** 将单个字写入 16 位设备寄存器。
 * @param devAddr I2C 从设备地址
 * @param regAddr 注册地址写入
 * @param data 要写入的新字值
 * @return 操作状态（true = 成功）
 */
bool I2Cdev::writeWord (uint8_t devAddr, uint8_t regAddr, uint16_t data)
{
    return writeWords(devAddr, regAddr, 1, &data);
}

/** 将多个字节写入 8 位设备寄存器。
 * @param devAddr I2C 从设备地址
 * @param regAddr 要写入的第一个寄存器地址
 * @param length 要写入的字节数
 * @param data 缓冲区复制新数据
 * @return 操作状态（true = 成功）
 */
bool I2Cdev::writeBytes (uint8_t devAddr, uint8_t regAddr, uint8_t length, uint8_t *data)
{
    uint8_t status = 0;

    for (uint8_t i = 0; i < length; i++)
    {
        simiic_write_reg(devAddr, regAddr, data[i]);
    }

    return status == 0;
}

/** 将多个字写入 16 位设备寄存器。
 * @param devAddr I2C 从设备地址
 * @param regAddr 要写入的第一个寄存器地址
 * @param length 要写的字数
 * @param data 缓冲区复制新数据
 * @return 操作状态（true = 成功）
 */
bool I2Cdev::writeWords (uint8_t devAddr, uint8_t regAddr, uint8_t length, uint16_t *data)
{
    uint8_t status = 0;
    for (uint8_t i = 0; i < length; i++)
    {
        simiic_write_reg(devAddr, regAddr, (uint8_t) (data[i] >> 8));
        simiic_write_reg(devAddr, regAddr, (uint8_t) data[i++]);
    }

    return status == 0;
}

/** Default timeout value for read operations.
 * Set this to 0 to disable timeout detection.
 */
uint16_t I2Cdev::readTimeout = I2CDEV_DEFAULT_READ_TIMEOUT;

