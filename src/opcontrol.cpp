#include "EZ-Template/util.hpp"
#include "helper_functions.hpp"
#include "main.h"
#include "pros/adi.h"
#include "pros/misc.h"
#include "pros/motors.h"
//using namespace pros = no need to retype pros:: everytime
#define ON 1
#define OFF 0
// local variable defined
const int vel = 600;
const int arm_vel = 400;

bool clamp_state = false;
char clamp_portOP = 'G';

int imu_port = 18;

// char sorter_portOP = 'A'; //fix
// int color_port = 4; //fix

char doinker_portOP = 'B'; //fix

void intakes() {
    while (true) {
        if (master.get_digital(pros::E_CONTROLLER_DIGITAL_R1)) {
            intaker(vel); // Intakes
        } else if (master.get_digital(pros::E_CONTROLLER_DIGITAL_R2)) {
            intaker(-vel); // Outakes
        } else {
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
    //Negative is up, 1200 is about a 90 degree motion of the arm
    const int DOWN_POSITION = -15; //Not 0 to make sure the motors don't fry themselves going through metal
    const int LOAD_POSITION = -235;
    const int SCORE_POSITION = -2000; 
    arm.pros::Motor::tare_position();

    int armPosition = DOWN_POSITION;
    while (true) 
    {
        if(master.get_digital(pros::E_CONTROLLER_DIGITAL_L1) )
        {
            if(armPosition == DOWN_POSITION)
            {
                armPosition = LOAD_POSITION;
                arm.move_absolute(LOAD_POSITION, arm_vel);
            }
            else
            {
                armPosition = DOWN_POSITION;
                arm.move_absolute(DOWN_POSITION, arm_vel);
            }
            wait(250);
        }
        else if(master.get_digital(pros::E_CONTROLLER_DIGITAL_L2))
        {
            if(armPosition == LOAD_POSITION)
            {
                armPosition = SCORE_POSITION;
                arm.move_absolute(SCORE_POSITION, arm_vel);
            }
            else if(armPosition == SCORE_POSITION)
            {
                armPosition = LOAD_POSITION;
                arm.move_absolute(LOAD_POSITION, arm_vel);
            }
            wait(300);
        }
    }
    pros::delay(ez::util::DELAY_TIME);
}

int TEAM = 0; //Someone remind me to fix this, idk where to declare team color, and this code kinda ugly ngl

// void sorterOP()
// {
//     pros::ADIDigitalOut sorter(sorter_portOP);
//     pros::Optical light(color_port);
//     pros::c::optical_rgb_s_t rgbVal;
//     enum{RED, BLUE};
//     double blueThreshold = 10;
//     double redThreshold = 10;
//     int color = TEAM;
//     bool manual = false;
//     bool sorterState = false;

//     sorter.set_value(OFF);
//     while(true)
//     {
//         rgbVal = light.get_rgb();
//         if(rgbVal.blue > blueThreshold) {color = BLUE;}
//         else if(rgbVal.red > redThreshold) {color = RED;}

//         if(master.get_digital(pros::E_CONTROLLER_DIGITAL_B))
//         {
//             manual = !manual;
//         }
//         if(!manual)
//         {
//             switch (color == TEAM)
//             {
//                 case true:
//                     sorter.set_value(OFF);
//                     sorterState = OFF;
//                     break;
//                 case false:
//                     sorter.set_value(ON);
//                     sorterState = ON;
//                     break;
//             }
//         }
//         else
//         {
//             if (master.get_digital(pros::E_CONTROLLER_DIGITAL_X))
//             {
//                 sorter.set_value(!sorterState);
//             }
//         }
//         master.print(2,0,"Sorter: %s", manual ? "Manual" : "Automatic");
//         wait(200);
//     }
// }

// void doinker()
// {
//     pros::ADIDigitalOut doinker(doinker_portOP);
//     bool doinkerState = OFF;
//     doinker.set_value(OFF);
//     while(true)
//     {
//         if(master.get_digital(pros::E_CONTROLLER_DIGITAL_Y))
//         {
//             doinker.set_value(!doinkerState);
//             wait(300);
//         }
//         wait(200);
//     }
// }

// void debugTurn() //help find turn angles
// {
//     master.clear();
//     pros::Imu imu(imu_port);
//     while(true)
//     {
//         if(master.get_digital(pros::E_CONTROLLER_DIGITAL_LEFT))
//         {
//             imu.tare_yaw();
//             wait(200);
//         }
//         master.print(0,0, "Angle: %.1lf    ", imu.get_yaw());
//         wait(300);
//     }
// }

// int inches = 0;
// void debugDrive()
// {
//     chassis.set_drive_brake(pros::E_MOTOR_BRAKE_BRAKE);
//     master.clear();
//     while(true)
//     {
//         int target = 0;
//         while(master.get_digital(pros::E_CONTROLLER_DIGITAL_RIGHT) == 0)
//         {
//             if(master.get_digital(pros::E_CONTROLLER_DIGITAL_LEFT) == 1 && master.get_digital(pros::E_CONTROLLER_DIGITAL_UP) == 1)
//             {
//                 inches = 0;
//             }
//             else if(master.get_digital(pros::E_CONTROLLER_DIGITAL_UP))
//             {
//                 target += 1;
//             }
//             else if(master.get_digital(pros::E_CONTROLLER_DIGITAL_DOWN))
//             {
//                 target -= 1;
//             }
//             master.print(1,0, "Dist: %d, Go: %d     ", inches, target);
//             wait(200);
//         }
//         for(pros::Motor m : chassis.left_motors)
//         {
//             if(target < 0)
//             {
//                 m.move_velocity(-40);
//             }
//             else
//             {
//                 m.move_velocity(40);
//             }

//         }
//         for(pros::Motor m : chassis.right_motors)
//         {
//             if(target < 0)
//             {
//                 m.move_velocity(-42);
//             }
//             else
//             {
//                 m.move_velocity(42);
//             }
//         }
//         chassis.left_motors[1].tare_position();
//         double factor = 48;
//         int offset = 20;
//         if(target < 0)
//         {
//             factor = 51.5;
//         }
//         else if(target == 0)
//         {
//             offset = 0;
//         }
//         while((abs(chassis.left_motors[1].get_position()) < abs(target * factor - offset)))
//         {
//             wait(100);
//         }
//         for(pros::Motor m : chassis.left_motors)
//         {
//             m.move_velocity(0);
//         }
//         for(pros::Motor m : chassis.right_motors)
//         {
//             m.move_velocity(0);
//         }
//         inches += target;
//         wait(200);
//         master.clear_line(1);
//     }
// }