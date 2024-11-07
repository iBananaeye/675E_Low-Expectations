#include "main.h"
#include "pros/adi.hpp"
#include "pros/motors.h"
#include "pros/motors.hpp"


pros::Motor intake(INTAKE_PORT, pros::E_MOTOR_GEARSET_06, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor conveyor(CONVEYOR_PORT, pros::E_MOTOR_GEARSET_06, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor arm(WALL_STAKE_PORT, pros::E_MOTOR_GEARSET_06, pros::E_MOTOR_ENCODER_DEGREES);

pros::Optical light(COLOR_SENSOR_PORT);
pros::Imu imu(IMU_PORT);

