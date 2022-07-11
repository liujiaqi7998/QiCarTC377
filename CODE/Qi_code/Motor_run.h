#ifndef __MOTOR_RUN_H
#define __MOTOR_RUN_H

#include "OJ_Library.h"
#include "Qi_Library.h"

#define LEFT_Motor 1
#define RIGHT_Motor 0

void Motor_run_Init ();
void Motor_run_loop ();
void left_changeAutoTune ();
void left_AutoTuneHelper (boolean start);
void right_changeAutoTune ();
void right_AutoTuneHelper (boolean start);
void set_speed (int direction, double sp);
void add_speed (int direction, double sp);
void reduce_speed (int direction, double sp);
#endif /*__MOTOR_RUN_H*/
