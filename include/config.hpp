#pragma once
#include "api.h"
#include "pros/adi.hpp"
#include "pros/motors.hpp"

//Left Drive: 11, 6, 10
//Right Drive: 19, 20, 9                             
//Conveyor: 8
//Intake: 2
//Wall Stake: 7
//Color Sensor: 15
//Radio: 1
//IMU: 18
//Clamp: B

//Available Ports: 3, 12, 13, 14, 15 16, 19
// 5 doesn't work?
enum Port
{
    INTAKE_PORT = -2,
    CONVEYOR_PORT = 9,
    WALL_STAKE_PORT = 8,

    COLOR_SENSOR_PORT = 4,
    IMU_PORT = 18,

    LEFT_FARBACK_WHEEL_PORT = -11,
    LEFT_MIDDLE_WHEEL_PORT = -6,
    LEFT_FRONT_WHEEL_PORT = -10,

    RIGHT_FARBACK_WHEEL_PORT = 17,
    RIGHT_MIDDLE_WHEEL_PORT = 20,
    RIGHT_FRONT_WHEEL_PORT = 7,

    CLAMP_PORT = 'G',
    SORTER_PORT = 'F', 
    DOINKER_PORT = 'C' //doesn't exist yet
};

extern pros::Motor intake;
extern pros::Motor conveyor;
extern pros::Motor arm;

extern pros::Imu imu;
extern pros::Optical light;