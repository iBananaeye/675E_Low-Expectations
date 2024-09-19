#include "helper_functions.hpp"
#include "main.h"
#include "pros/adi.h"
#include "pros/misc.h"
#include "pros/motors.h"
//using namespace pros = no need to retype pros:: everytime

// local variable defined
const int vel = 600;
bool clamp_state = false;
char clamp_portOP = 'E';




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
        if (master.get_digital(pros::E_CONTROLLER_DIGITAL_L1)) {
            clamp.set_value(!clamp_state);
            clamp_state = !clamp_state;
        }
    }
    pros::delay(ez::util::DELAY_TIME);
}