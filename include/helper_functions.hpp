#pragma once
#include "api.h"

void intaker(double v);
void intaker_wait(double v, int time);

void wait(int time);

void move_drive_wait(double target, int speed);
void turn_drive_wait(double target, int speed);




