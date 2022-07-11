#include "core0_main.h"
#include "LCD_Show.h"
void core0_setup()
{
    //…„œÒÕ∑≥ı ºªØ
    MT9V03x_Init();
    MT9V03x_Show_Info();
    LCD_Show_int();
}
void core0_loop()
{
}
