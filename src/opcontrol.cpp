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

bool clamp_state = false;
char clamp_portOP = 'G';

bool arm_down = true;
bool arm_set = false;



void intakes() {
    while (true) {
        if (master.get_digital(pros::E_CONTROLLER_DIGITAL_R1)) {
            intaker(-vel);
        } else if (master.get_digital(pros::E_CONTROLLER_DIGITAL_R2)) {
            intaker(vel);
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
    const int DOWN_POSITION = -15; //Not 0 to make sure the motors don't fry themselves tryna go through metal
    const int LOAD_POSITION = -235;
    const int SCORE_POSITION = -2000; 
    arm.pros::Motor::tare_position();

    int armPosiion = DOWN_POSITION;
    while (true) 
    {
        if(master.get_digital(pros::E_CONTROLLER_DIGITAL_L1) )
        {
            if(armPosiion == DOWN_POSITION)
            {
                armPosiion = LOAD_POSITION;
                arm.move_absolute(LOAD_POSITION, arm_vel);
            }
            else
            {
                armPosiion = DOWN_POSITION;
                arm.move_absolute(DOWN_POSITION, arm_vel);
            }
            wait(250);
        }
        else if(master.get_digital(pros::E_CONTROLLER_DIGITAL_L2))
        {
            if(armPosiion == LOAD_POSITION)
            {
                armPosiion = SCORE_POSITION;
                arm.move_absolute(SCORE_POSITION, arm_vel);
            }
            else if(armPosiion == SCORE_POSITION)
            {
                armPosiion = LOAD_POSITION;
                arm.move_absolute(LOAD_POSITION, arm_vel);
            }
            wait(300);
        }
    }
    pros::delay(ez::util::DELAY_TIME);
}

void debugTurn() //help find turn angles
{
    master.clear();
    pros::Imu imu(7);
    while(true)
    {
        if(master.get_digital(pros::E_CONTROLLER_DIGITAL_LEFT))
        {
            imu.tare_yaw();
            wait(200);
        }
        master.print(0,0, "Angle: %.1lf    ", imu.get_yaw());
        wait(300);
    }
}

int inches = 0;
void debugDrive()
{
    chassis.set_drive_brake(pros::E_MOTOR_BRAKE_BRAKE);
    master.clear();
    while(true)
    {
        int target = 0;
        while(master.get_digital(pros::E_CONTROLLER_DIGITAL_RIGHT) == 0)
        {
            if(master.get_digital(pros::E_CONTROLLER_DIGITAL_LEFT) == 1 && master.get_digital(pros::E_CONTROLLER_DIGITAL_UP) == 1)
            {
                inches = 0;
            }
            else if(master.get_digital(pros::E_CONTROLLER_DIGITAL_UP))
            {
                target += 1;
            }
            else if(master.get_digital(pros::E_CONTROLLER_DIGITAL_DOWN))
            {
                target -= 1;
            }
            master.print(1,0, "Dist: %d, Go: %d     ", inches, target);
            wait(200);
        }
        for(pros::Motor m : chassis.left_motors)
        {
            if(target < 0)
            {
                m.move_velocity(-40);
            }
            else
            {
                m.move_velocity(40);
            }
            
        }
        for(pros::Motor m : chassis.right_motors)
        {
            if(target < 0)
            {
                m.move_velocity(-42);
            }
            else
            {
                m.move_velocity(42);
            }
        }
        chassis.left_motors[1].tare_position();
        double factor = 48;
        int offset = 20;
        if(target < 0)
        {
            factor = 51.5;
        }
        else if(target == 0)
        {
            offset = 0;
        }
        while((abs(chassis.left_motors[1].get_position()) < abs(target * factor - offset)))
        {
            wait(100);
        }
        for(pros::Motor m : chassis.left_motors)
        {
            m.move_velocity(0);
        }
        for(pros::Motor m : chassis.right_motors)
        {
            m.move_velocity(0);
        }
        inches += target;
        wait(200);
        master.clear_line(1);
    }
}