#include "main.h"
#include "pros/adi.h"
#include "pros/misc.h"
#include "pros/motors.h"
//using namespace pros = no need to retype pros:: everytime

// local variable defined
const int vel = 600;
bool clamp_state = false;
char clamp_port = 'A';




void intakes() {
    while (true) {
        if (master.get_digital(pros::E_CONTROLLER_DIGITAL_L1)) {
            intake.move_velocity(vel); 
        }
        else if (master.get_digital(pros::E_CONTROLLER_DIGITAL_L2)) {
            intake.move_velocity(-vel);
        }
        else {
            intake.move_velocity(0);
        }
    }
    pros::delay(ez::util::DELAY_TIME); 
}

void clamps() {
    pros::ADIDigitalOut clamp (clamp_port); // port 'A'
    clamp.set_value(clamp_state); // retracted

    while (true) {
        if (master.get_digital(pros::E_CONTROLLER_DIGITAL_R1)) {
            clamp.set_value(!clamp_state);
            clamp_state = !clamp_state;
        }
    }
}