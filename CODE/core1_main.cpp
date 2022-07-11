#include "core1_main.h"
#include "Qi_Library.h"

void core1_setup ()
{
   
    //获取总钻风摄像头配置信息(cpu1,cpu2用不了)
   // MT9V03x_Get_Conf();
    //通过RT-Thread Finsh 显示总钻风摄像头所有信息(cpu1,cpu2用不了)
   // MT9V03x_Show_Info();

}
void core1_loop ()
{
    //使用图像时不要同时使用Buffer_image,mt9v03x_image,Result_image,temp_image
    if (MT9V03x_IMG_Complete())
    {
        MT9V03x_Clear_Flag();
        DIP::Global_Otsu_Better((uint8 *) mt9v03x_image, (uint8 *) Result_image, MT9V03X_H, MT9V03X_W);
    }
}
