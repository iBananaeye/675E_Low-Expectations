#include "main.h"
#include "pros/adi.h"
#include "pros/misc.h"
#include "pros/motors.h"

void intaker_wait(double v,int time) {
    intake.move_velocity(v);
    pros::delay(time);
    intake.move_velocity(0);
}

void intaker(double v) {
    intake.move_velocity(v);
}

void outtaker(double v) {
    intake.move_velocity(-v);
}

void wait(int time){
  pros::delay(time);
}


void move_drive_wait(double target, int speed){
    chassis.set_drive_pid(target, speed);
    chassis.wait_drive();
}

void turn_drive_wait(double target, int speed) {
    chassis.set_turn_pid(target, speed);
    chassis.wait_drive();
}

void outtaker_wait(double v, int time){
    intake.move_velocity(-v);
    pros::delay(time);
    intake.move_velocity(0);
}

