#include "main.h"
#include "pros/adi.hpp"
#include "pros/motors.h"
#include "pros/motors.hpp"


pros::Motor intake(INTAKE_PORT, pros::v5::MotorGears::blue, pros::v5::MotorEncoderUnits::degrees);
pros::Motor conveyor(CONVEYOR_PORT, pros::v5::MotorGears::blue, pros::v5::MotorEncoderUnits::degrees);
pros::Motor arm(WALL_STAKE_PORT, pros::v5::MotorGears::blue, pros::v5::MotorEncoderUnits::degrees);

pros::Optical light(COLOR_SENSOR_PORT);
pros::Imu imu(IMU_PORT);

pros::Controller master(pros::E_CONTROLLER_MASTER);