// I2Cdev �⼯�� - ��Ҫ I2C �豸��
// ��λ���ֽ� I2C R/W ��������Ϊһ���������
// 2013-06-05 ���� Jeff Rowberg <jeff@rowberg.net>
//
// ������־��
// 2013-05-06 - ��� Francesco Ferrara �� Fastwire v0.24 ʵ�֣��������޸�
// 2013-05-05 - �޸��˽�λֵд���ֵ����⣨Sasquatch/Farzanegan��
// 2012-06-09 - �޸�ʹ�� Arduino Wire һ�ζ�ȡ > 32 ���ֽڵ���Ҫ����
// - ��ʹ�ù�ʱ�� IDE �����޵� I2Cdev ʵ��ʱ��ӱ���������
// 2011-11-01 - �޸� write*Bits ������㣨��л sasquatch @ Arduino ��̳��
// 2011-10-03 - ������Զ� Arduino �汾����Է���ʹ��
// 2011-10-02 - ����� Gene Knight �� NBWire TwoWire ��ʵ�֣������޸�
// 2011-08-31 - ����˶� Arduino 1.0 Wire ���֧�֣������� 0.x ��ͬ��
// 2011-08-03 - Ϊ read* ��������˿�ѡ�ĳ�ʱ�������Ա����ɸ���Ĭ��ֵ
// 2011-08-02 - ����˶� 16 λ�Ĵ�����֧��
// - �޸���ĳЩ�����ϲ���ȷ�� Doxygen ע��
// - Ϊ��ȡ������ӳ�ʱֵ����л mem @ Arduino ��̳��
// 2011-07-30 - ���Ķ�/д�����ṹ�Է��سɹ����ֽ���
// - �����з�����Ϊ��̬�Խ�ʡ���豸�ڴ�
// 2011-07-28 - ��ʼ�汾

#include "I2Cdev.h"

/** Default constructor.
 */
I2Cdev::I2Cdev ()
{
}

/** �� 8 λ�豸�Ĵ����ж�ȡ����λ��
 * @param devAddr I2C ���豸��ַ
 * @param regAddr ע��Ҫ��ȡ�� regAddr
 * @param bitNum Ҫ��ȡ��λλ�� (0-7)
 * @param data ����λֵ������
 * @param timeout �Ժ���Ϊ��λ�Ŀ�ѡ��ȡ��ʱ��0 ��ʾ���ã�����ʹ�� I2Cdev::readTimeout �е�Ĭ����ֵ��
 * @return ������״̬��true = �ɹ���
 */
int8_t I2Cdev::readBit (uint8_t devAddr, uint8_t regAddr, uint8_t bitNum, uint8_t *data, uint16_t timeout)
{
    uint8_t b;
    uint8_t count = readByte(devAddr, regAddr, &b, timeout);
    *data = b & (1 << bitNum);
    return count;
}

/** �� 16 λ�豸�Ĵ����ж�ȡ����λ��
 * @param devAddr I2C ���豸��ַ
 * @param regAddr ע��Ҫ��ȡ�� regAddr
 * @param bitNum Ҫ��ȡ��λλ�� (0-15)
 * @param data ����λֵ������
 * @param timeout �Ժ���Ϊ��λ�Ŀ�ѡ��ȡ��ʱ��0 ��ʾ���ã�����ʹ�� I2Cdev::readTimeout �е�Ĭ����ֵ��
 * @return ������״̬��true = �ɹ���
 */
int8_t I2Cdev::readBitW (uint8_t devAddr, uint8_t regAddr, uint8_t bitNum, uint16_t *data, uint16_t timeout)
{
    uint16_t b;
    uint8_t count = readWord(devAddr, regAddr, &b, timeout);
    *data = b & (1 << bitNum);
    return count;
}

/** �� 8 λ�豸�Ĵ����ж�ȡ���λ��
 * @param devAddr I2C ���豸��ַ
 * @param regAddr ע��Ҫ��ȡ�� regAddr
 * @param bitStart Ҫ��ȡ�ĵ�һλλ�� (0-7)
 * @param length Ҫ��ȡ��λ���������� 8��
 * @param data �Ҷ���ֵ�������������κ� bitStart λ�ö�ȡ�� '101' ������ 0x05��
 * @param timeout �Ժ���Ϊ��λ�Ŀ�ѡ��ȡ��ʱ��0 ��ʾ���ã�����ʹ�� I2Cdev::readTimeout �е�Ĭ����ֵ��
 * @return ������״̬��true = �ɹ���
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

/** �� 16 λ�豸�Ĵ����ж�ȡ���λ��
 * @param devAddr I2C ���豸��ַ
 * @param regAddr ע��Ҫ��ȡ�� regAddr
 * @param bitStart Ҫ��ȡ�ĵ�һλλ�� (0-15)
 * @param length Ҫ��ȡ��λ���������� 16��
 * @param data �Ҷ���ֵ�������������κ� bitStart λ�ö�ȡ�� '101' ������ 0x05��
 * @param timeout �Ժ���Ϊ��λ�Ŀ�ѡ��ȡ��ʱ��0 ��ʾ���ã�����ʹ�� I2Cdev::readTimeout �е�Ĭ����ֵ��
 * @return ������״̬��1 = �ɹ���0 = ʧ�ܣ�-1 = ��ʱ��
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

/** �� 8 λ�豸�Ĵ�����ȡ�����ֽڡ�
 * @param devAddr I2C ���豸��ַ
 * @param regAddr ע��Ҫ��ȡ�� regAddr
 * @param data ���豸��ȡ���ֽ�ֵ������
 * @param timeout �Ժ���Ϊ��λ�Ŀ�ѡ��ȡ��ʱ��0 ��ʾ���ã�����ʹ�� I2Cdev::readTimeout �е�Ĭ����ֵ��
 * @return ������״̬��true = �ɹ���
 */
int8_t I2Cdev::readByte (uint8_t devAddr, uint8_t regAddr, uint8_t *data, uint16_t timeout)
{
    return readBytes(devAddr, regAddr, 1, data, timeout);
}

/** �� 16 λ�豸�Ĵ����ж�ȡ�����֡�
 * @param devAddr I2C ���豸��ַ
 * @param regAddr ע��Ҫ��ȡ�� regAddr
 * @param data ���豸��ȡ����ֵ������
 * @param timeout �Ժ���Ϊ��λ�Ŀ�ѡ��ȡ��ʱ��0 ��ʾ���ã�����ʹ�� I2Cdev::readTimeout �е�Ĭ����ֵ��
 * @return ������״̬��true = �ɹ���
 */
int8_t I2Cdev::readWord (uint8_t devAddr, uint8_t regAddr, uint16_t *data, uint16_t timeout)
{
    return readWords(devAddr, regAddr, 1, data, timeout);
}

/** �� 8 λ�豸�Ĵ�����ȡ����ֽڡ�
 * @param devAddr I2C ���豸��ַ
 * @param regAddr ����ע��Ҫ��ȡ�� regAddr
 * @param length Ҫ��ȡ���ֽ���
 * @param data ���ڴ洢��ȡ���ݵ����ݻ�����
 * @param timeout �Ժ���Ϊ��λ�Ŀ�ѡ��ȡ��ʱ��0 ��ʾ���ã�����ʹ�� I2Cdev::readTimeout �е�Ĭ����ֵ��
 * @return ��ȡ���ֽ�����-1 ��ʾʧ�ܣ�
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

/** �� 16 λ�豸�Ĵ����ж�ȡ����֡�
 * @param devAddr I2C ���豸��ַ
 * @param regAddr ����ע��Ҫ��ȡ�� regAddr
 * @param length Ҫ��ȡ������
 * @param data ���ڴ洢��ȡ���ݵĻ�����
 * @param timeout �Ժ���Ϊ��λ�Ŀ�ѡ��ȡ��ʱ��0 ��ʾ���ã�����ʹ�� I2Cdev::readTimeout �е�Ĭ����ֵ��
 * @return ��ȡ��������-1 ��ʾʧ�ܣ�
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

/** �� 8 λ�豸�Ĵ�����д��һ��λ��
 * @param devAddr I2C ���豸��ַ
 * @param regAddr ע��Ҫд��� regAddr
 * @param bitNum Ҫд���λλ�� (0-7)
 * @param value Ҫд�����λֵ
 * @return ����״̬��true = �ɹ���
 */

bool I2Cdev::writeBit (uint8_t devAddr, uint8_t regAddr, uint8_t bitNum, uint8_t data)
{
    uint8_t b;
    readByte(devAddr, regAddr, &b);
    b = (data != 0) ? (b | (1 << bitNum)) : (b & ~(1 << bitNum));
    return writeByte(devAddr, regAddr, b);
}

/** �� 16 λ�豸�Ĵ�����д�뵥��λ��
 * @param devAddr I2C ���豸��ַ
 * @param regAddr ע��Ҫд��� regAddr
 * @param bitNum Ҫд���λλ�� (0-15)
 * @param value Ҫд�����λֵ
 * @return ����״̬��true = �ɹ���
 */
bool I2Cdev::writeBitW (uint8_t devAddr, uint8_t regAddr, uint8_t bitNum, uint16_t data)
{
    uint16_t w;
    readWord(devAddr, regAddr, &w);
    w = (data != 0) ? (w | (1 << bitNum)) : (w & ~(1 << bitNum));
    return writeWord(devAddr, regAddr, w);
}

/** �� 8 λ�豸�Ĵ�����д����λ��
 * @param devAddr I2C ���豸��ַ
 * @param regAddr ע��Ҫд��� regAddr
 * @param bitStart Ҫд��ĵ�һλλ�� (0-7)
 * @param length Ҫд���λ���������� 8��
 * @param data Ҫд����Ҷ���ֵ
 * @return ����״̬��true = �ɹ���
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

/** �� 16 λ�豸�Ĵ�����д����λ��
 * @param devAddr I2C ���豸��ַ
 * @param regAddr ע��Ҫд��� regAddr
 * @param bitStart Ҫд��ĵ�һλλ�� (0-15)
 * @param length Ҫд���λ���������� 16��
 * @param data Ҫд����Ҷ���ֵ
 * @return ����״̬��true = �ɹ���
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

/** �������ֽ�д�� 8 λ�豸�Ĵ�����
 * @param devAddr I2C ���豸��ַ
 * @param regAddr ע���ַд��
 * @param data Ҫд������ֽ�ֵ
 * @return ����״̬��true = �ɹ���
 */
bool I2Cdev::writeByte (uint8_t devAddr, uint8_t regAddr, uint8_t data)
{
    return writeBytes(devAddr, regAddr, 1, &data);
}

/** ��������д�� 16 λ�豸�Ĵ�����
 * @param devAddr I2C ���豸��ַ
 * @param regAddr ע���ַд��
 * @param data Ҫд�������ֵ
 * @return ����״̬��true = �ɹ���
 */
bool I2Cdev::writeWord (uint8_t devAddr, uint8_t regAddr, uint16_t data)
{
    return writeWords(devAddr, regAddr, 1, &data);
}

/** ������ֽ�д�� 8 λ�豸�Ĵ�����
 * @param devAddr I2C ���豸��ַ
 * @param regAddr Ҫд��ĵ�һ���Ĵ�����ַ
 * @param length Ҫд����ֽ���
 * @param data ����������������
 * @return ����״̬��true = �ɹ���
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

/** �������д�� 16 λ�豸�Ĵ�����
 * @param devAddr I2C ���豸��ַ
 * @param regAddr Ҫд��ĵ�һ���Ĵ�����ַ
 * @param length Ҫд������
 * @param data ����������������
 * @return ����״̬��true = �ɹ���
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

