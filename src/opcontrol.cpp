#include "EZ-Template/util.hpp"
#include "helper_functions.hpp"
#include "main.h"
#include "pros/adi.h"
#include "pros/misc.h"
#include "pros/motors.h"
//using namespace pros = no need to retype pros:: everytime

// local variable defined
const int vel = 600;
const int arm_vel = 400;

bool arm_down = true;
bool arm_set = false;
bool clamp_state = true;
char clamp_portOP = 'G';





void intakes() {
    while (true) {
        if (master.get_digital(pros::E_CONTROLLER_DIGITAL_R1)) {
            intaker(vel);
        }
        else if (master.get_digital(pros::E_CONTROLLER_DIGITAL_R2)) {
            intaker(-vel);
        }
        else {
            intaker(0);
        }
    }
    pros::delay(ez::util::DELAY_TIME); 
}


void clamps() {
    pros::ADIDigitalOut clamp (clamp_portOP); // port ^^^
    clamp.set_value(clamp_state); // retracted

    while (true) {
        if (master.get_digital(pros::E_CONTROLLER_DIGITAL_A)) {
            clamp.set_value(!clamp_state);
            clamp_state = !clamp_state;
            wait(460);
        }
    }
    pros::delay(ez::util::DELAY_TIME);
}

void wall_score() {
        arm.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
        arm.pros::Motor::set_zero_position(0);
    while (true) {
        if (master.get_digital(pros::E_CONTROLLER_DIGITAL_L1) && arm_down) {
            arm.move_velocity(-arm_vel);
            wait(700);
            arm.move_velocity(0);
            // arm.move_absolute(75, arm_vel);
            arm_down = !arm_down;
            arm_set = true;
        }
        // else if (master.get_digital(pros::E_CONTROLLER_DIGITAL_L1) && arm_set) {
        //     arm.move_absolute(25, arm_vel);
        //     arm_set = false;
        // }
        else if (master.get_digital(pros::E_CONTROLLER_DIGITAL_L2) && !arm_down){
            arm.move_velocity(arm_vel);
            wait(700);
            arm.move_velocity(0);
            arm_down = !arm_down;
            arm_set = false;
        }
    }
}