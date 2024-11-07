#include "main.h"
#include "pros/adi.hpp"
#include "pros/motors.h"
#include "pros/motors.hpp"

pros::Motor intake(2, pros::E_MOTOR_GEARSET_06, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor conveyor(8, pros::E_MOTOR_GEARSET_06, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor arm(9, pros::E_MOTOR_GEARSET_06, pros::E_MOTOR_ENCODER_DEGREES);
