#include "core1_main.h"
#include "Qi_Library.h"

void core1_setup ()
{
   
    //��ȡ���������ͷ������Ϣ(cpu1,cpu2�ò���)
   // MT9V03x_Get_Conf();
    //ͨ��RT-Thread Finsh ��ʾ���������ͷ������Ϣ(cpu1,cpu2�ò���)
   // MT9V03x_Show_Info();

}
void core1_loop ()
{
    //ʹ��ͼ��ʱ��Ҫͬʱʹ��Buffer_image,mt9v03x_image,Result_image,temp_image
    if (MT9V03x_IMG_Complete())
    {
        MT9V03x_Clear_Flag();
        DIP::Global_Otsu_Better((uint8 *) mt9v03x_image, (uint8 *) Result_image, MT9V03X_H, MT9V03X_W);
    }
}
