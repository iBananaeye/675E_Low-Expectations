#pragma once
#include "api.h"

void intaker(double v);
void intaker_wait(double v, int time);

void wait(int time);

void move_drive_wait(double target, int speed);
void turn_drive_wait(double target, int speed);

void wall_staker(int pos, int arm_vel);

void doink();
void undoink();

int getRingColor();
void setTeam();
void setTeam(int t);
int getTeam();

