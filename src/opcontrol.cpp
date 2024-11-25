
#include "helper_functions.hpp"
#include "main.h"
#include "pros/adi.h"
#include "pros/misc.h"
#include "pros/motors.h"
//using namespace pros = no need to retype pros:: everytime

const int ON = 1;
const int OFF = 0;

// local variable defined
int vel = 600;
const int arm_vel = 500; 

bool clamp_state = false;

void intakes() {
    while (true) {
        if (master.get_digital(pros::E_CONTROLLER_DIGITAL_R1)) {
            intaker(vel); //Intakes
        } else if (master.get_digital(pros::E_CONTROLLER_DIGITAL_R2)) {
            intaker(-vel); //Outakes
        } else {
            intaker(0);
        }
    }
    pros::delay(10); 
}

void intakesConSorter() {
    int team = getTeam(); // defaults to red
    int color = team;
    bool sorterState = false;
    bool manual = false; // defaults to automatic

    light.set_integration_time(20);
    while (true) {
        color = getRingColor();
        if(master.get_digital(pros::E_CONTROLLER_DIGITAL_R1) && !master.get_digital(pros::E_CONTROLLER_DIGITAL_R2)&& color != 2 && light.get_proximity() > 215 &&color !=team)
        {
            intaker(-vel);
            wait(1000);
            intaker(0);
        }
        if (master.get_digital(pros::E_CONTROLLER_DIGITAL_R1)) {
            intaker(vel); //Intakes
        } else if (master.get_digital(pros::E_CONTROLLER_DIGITAL_R2)) {
            intaker(-vel); //Outakes
        } else {
            intaker(0);
        }
    }
    pros::delay(10); 
}

void clamps() {
    pros::adi::DigitalOut clamp (Port::CLAMP_PORT); // port in config.hpp
    clamp.set_value(clamp_state); // retracted

    while (true) {
        if (master.get_digital(pros::E_CONTROLLER_DIGITAL_A)) {
            clamp.set_value(!clamp_state);
            clamp_state = !clamp_state;
            wait(460);
        }
    }
    pros::delay(10);
}

void wall_score() {
    arm.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
    //Negative is up
    //-15, -310, -1670
    const int DOWN_POSITION = -15; //Not 0 to make sure the motors don't fry themselves going through metal
    const int LOAD_POSITION = -120;
    const int SCORE_POSITION = -555; //-55
    const int INSERT_POSITION = -630;
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
                armPosition = INSERT_POSITION;
                arm.move_absolute(INSERT_POSITION, arm_vel);
            }
            else if(armPosition == INSERT_POSITION)
            {
                armPosition = LOAD_POSITION;
                arm.move_absolute(LOAD_POSITION, arm_vel);
            }
            else
            {
                arm.move_absolute(SCORE_POSITION, arm_vel);
            }
            wait(300);
        }
        // Outputs encoder to screen to find wall stake positions
        // master.print(0,0, "Pos: %.1lf", arm.get_position());
        // wait(250);
    }
    pros::delay(10);
}

void sorter()
{
    pros::adi::DigitalOut sorter(Port::SORTER_PORT); //port in config.hpp
    enum{RED, BLUE, OTHER};
    const int waitTime = 275; //time before piston is retracted

    int team = getTeam(); // defaults to red
    int color = team;
    bool sorterState = false;
    bool manual = false; // defaults to automatic

    light.set_led_pwm(50);
    light.set_integration_time(20); //20 Millisecond refresh rate
    //The sensor can go down to 3, but brain only accepts every 20ms. Anything faster than this leds to nonsense being sensed
    
    master.print(0,0,"T%s-%s %s", 
            team == RED ? "R" : "B" ,
            manual ? "Man" : "Aut", 
            color == RED ? "R" : color==OTHER ? "U" : "B"
        ); //Prints 'T[Alliance Color Initial]-[Whether in auto or manual mode] [Last seen color]'

    sorter.set_value(OFF); //defaults to off
    while(true)
    {
        color = getRingColor();
        if(master.get_digital(pros::E_CONTROLLER_DIGITAL_Y)) // Alternates from auto to manual mode
        {
            manual = !manual;
            wait(300);
            sorter.set_value(OFF);
            master.print(0,0,"T%s-%s %s", team == RED ? "R" : "B" , manual ? "Man" : "Aut", color == RED ? "R" : color==OTHER ?"U" : "B");
        }
        if(!manual && color != OTHER && light.get_proximity() > 215)
        {
            switch (color == team)
            {
                case true:
                    sorter.set_value(OFF);
                    sorterState = OFF;
                    master.print(0,0,"T%s-%s %s", team == RED ? "R" : "B" , manual ? "Man" : "Aut", color == RED ? "R" : color==OTHER ?"U" : "B");
                    break;
                case false:
                    sorter.set_value(ON);
                    master.print(0,0,"T%s-%s %s", team == RED ? "R" : "B" , manual ? "Man" : "Aut", color == RED ? "R" : color==OTHER ?"U" : "B");
                    while(true) //prevents wasting air if opposing rings is held over sensor
                    {
                        wait(waitTime);
                        if((getRingColor() == team || getRingColor() == OTHER))
                        {
                            break;
                        }
                    }
                    sorter.set_value(OFF);
                    break;
            }
        }
        if(manual)
        {
            if(master.get_digital(pros::E_CONTROLLER_DIGITAL_X)) //manually control like clamp
            {
                sorter.set_value(!sorterState);
                sorterState = !sorterState;
                wait(300);
            }
        }
        wait(10);
    }
}

void doinker()
{
    pros::adi::DigitalOut doinker(Port::DOINKER_PORT);
    bool doinkerState = OFF;
    doinker.set_value(OFF);
    while(true)
    {
        if(master.get_digital(pros::E_CONTROLLER_DIGITAL_B))
        {
            doinker.set_value(!doinkerState);
            doinkerState = !doinkerState;
            wait(200);
        }
    }
}

void autoClamp()
{  
    pros::adi::DigitalIn autoClamp(Port::AUTOCLAMP_PORT);
  pros::adi::DigitalOut clamp(Port::CLAMP_PORT);
  clamp.set_value(false);
  bool state = false;
  bool isClamped = false;
  bool active = true;
  wait(300);
  while(true)
  {
    if(active&& state == false && autoClamp.get_value() == 1)
    {
      clamp.set_value(true);
      wait(250);
      state = true;
      isClamped = true;
    }
    if(master.get_digital(pros::E_CONTROLLER_DIGITAL_A))
    {
      clamp.set_value(!isClamped);
      isClamped = !isClamped;
      wait(500);
    }
    if(autoClamp.get_value() == 0)
    {
      state = false;
    }
    if(master.get_digital(pros::E_CONTROLLER_DIGITAL_LEFT))
    {
        active = !active;
        wait(300);
    }
    wait(20);
  }
}



//ignore everything below here, i would comment it out but
//I would need to comment stuff out across 3 files and that is no fun

void debugTurn() //help find turn angles
{
    master.clear();
    while(true)
    {
        if(master.get_digital(pros::E_CONTROLLER_DIGITAL_LEFT))
        {
            imu.tare_yaw();
            wait(200);
        }
        master.print(1,0, "Angle: %.1lf  ", imu.get_yaw());
        wait(300);
    }
}

// int inches = 0;
// void debugDrive() //Lets you move a certain amount from controller input, not used anywhere 
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
//             master.print(2,0, "Dist: %d, Go: %d     ", inches, target);
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