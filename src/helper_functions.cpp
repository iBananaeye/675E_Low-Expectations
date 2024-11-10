#include "main.h"
#include "pros/adi.h"
#include "pros/misc.h"
#include "pros/motors.h"

void intaker(double v) {
    intake.move_velocity(-v);
    conveyor.move_velocity(v);
}

void intaker_wait(double v,int time) {
    intake.move_velocity(-v);
    conveyor.move_velocity(v);

    pros::delay(time);
    
    intake.move_velocity(0);
    conveyor.move_velocity(0);
}

//---------

void wait(int time){
  pros::delay(time);
}

//---------

void move_drive_wait(double target, int speed){
    chassis.set_drive_pid(target, speed);
    chassis.wait_drive();
}

void turn_drive_wait(double target, int speed) {
    chassis.set_turn_pid(target, speed);
    chassis.wait_drive();
}

//---------
enum{RED,BLUE, OTHER};
int team = RED; //default

int getRingColor()
{
    //Color sensors returns a hue in degrees on a color wheel
    //Red is at the top of the wheel blue is around 6 to 9 o clock

    //Red is 330-30
    //Blue is 150-250 depending on shade

    double blueThreshold = 90;
    double lowRedThreshold = 30; 
    double highRedThreshold = 270;

    double hue = light.get_hue();

    if(hue < lowRedThreshold || hue > highRedThreshold) 
    {
        return RED;
    }
    else if(hue > blueThreshold) 
    {
        return BLUE;
    }
    return OTHER;
}

void setTeam() //sets the team to whatever ring is over the color sensor
{
    int col = getRingColor(); //defaults to red
    if(col != OTHER)
    {
        team = col;
    }
}

int getTeam()
{
    return team;
}
