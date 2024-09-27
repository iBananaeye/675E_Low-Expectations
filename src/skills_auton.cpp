#include "EZ-Template/util.hpp"
#include "display/lv_hal/lv_hal_indev.h"
#include "helper_functions.hpp"
#include "main.h"

const int DRIVE_SPEED = 110; // This is 110/127 (around 87% of max speed). When this is 87%, the robot is correcting by making one side
                            // faster and one side slower, giving better heading correction.
const int TURN_SPEED = 75;
const int SWING_SPEED = 90;
const int INTAKE_SPEED = 500;
char clamp_port = 'A';

void default_constants() //TUNE PID BASED OFF COMMENTS MADE BELOW
{
  chassis.set_slew_min_power(80, 80);
  chassis.set_slew_distance(7, 7);
  chassis.set_pid_constants(&chassis.headingPID, 11, 0, 20, 0);//increase D and decrease I (later on for this one) to decrease settling time = robot moves faster between calls
  chassis.set_pid_constants(&chassis.forward_drivePID, 0.45, 0, 5, 0);//increase D and decrease I (later on for this one) to decrease settling time = robot moves faster between calls
  chassis.set_pid_constants(&chassis.backward_drivePID, 0.45, 0, 5, 0);
  chassis.set_pid_constants(&chassis.turnPID, 5, 0.003, 35, 15);//increase D and decrease I (later on for this one) to decrease settling time = robot moves faster between calls
  chassis.set_pid_constants(&chassis.swingPID, 7, 0, 45, 0);
}

/*
HOW TO TUNE PID

Step 1 - kP
You'll start with kP. Set it to some number, and run your example autonomous. The robot will either undershoot the target (kP too low), or the robot is oscillate around the target (kP too high).

To tune a PD controller, you want the kP to oscillate a little bit, usually a bounce or two.

Step 2 - kD
After finding a kP that oscillates a little, we can tune kD. Increase kD until the oscillation goes away. This movement should look more "snappy" then just a P loop.

Step 3 - Repeat
Repeat Steps 1 and Steps 2 until kD cannot remove oscillation from the movement. Then go back to the last values that worked.

Step 4 - kI
Sometimes you need a little extra power to get your robot all the way there. Integral can be a dangerous variable because it grows exponentially. The fourth parameter is what the error needs to be for I to start. For turns, we found around 15 degrees is good.

Increase kI until any minor disturbances are accounted for. You might need to adjust kD while tuning kI.
*/



void tune_PID(){
  //go straight
  //go back
  //turn right
  //turn left
  //swing turn right
  //swing turn left
  //go straight for a while to check heading
}                                                                                                                                                                                                                                                                                                                                                                                                               

// Path 2 (Simple): Red, 2 donuts and touches
void bottom_simple() {
  // Set-up: piston to ADI port clamp_port
  bool clamp_state = false;
  pros::ADIDigitalOut clamp (clamp_port);
  clamp.set_value(clamp_state); // retracted (unclamped)

  // Begin movement
  move_drive_wait(-20, DRIVE_SPEED);
  clamp.set_value(!clamp_state); // clamps down on bottom left mogol

  // turn intake towards stacked rings (target is bottom red)
  turn_drive_wait(-110, TURN_SPEED);
  move_drive_wait(20, DRIVE_SPEED);

  // intake and turn + move to touch Ladder
  intaker_wait(INTAKE_SPEED, 2200);
  turn_drive_wait(-195, TURN_SPEED);
  move_drive_wait(30, DRIVE_SPEED);

}

void bottom_ring_first() {
  // Set-up: piston to ADI port clamp_port
  bool clamp_state = false;
  pros::ADIDigitalOut clamp (clamp_port);
  clamp.set_value(clamp_state); // retracted (unclamped)

  // Begin movement
  move_drive_wait(-20, DRIVE_SPEED);
  clamp.set_value(!clamp_state); // clamps down on bottom left mogol

  // turn intake towards stacked rings (target is bottom red)
  turn_drive_wait(-110, TURN_SPEED);
  move_drive_wait(20, DRIVE_SPEED);

  // intake and turn + move to touch Ladder
  intaker_wait(INTAKE_SPEED, 2200);
  turn_drive_wait(-195, TURN_SPEED);
  move_drive_wait(30, DRIVE_SPEED);

}

//Red, 2 rings, midline goal, touches
void mid_line_rush(){
  bool clamp_state = false;
  pros::ADIDigitalOut clamp (clamp_port);
  clamp.set_value(clamp_state); // retracted (unclamped)

  //turn and get midline mogol 
  turn_drive_wait(20, TURN_SPEED); 
  move_drive_wait(40, DRIVE_SPEED);
  clamp.set_value(!clamp_state);

  //get bottom red ring
  turn_drive_wait(-45, TURN_SPEED);
  move_drive_wait(10, DRIVE_SPEED);
  intaker_wait(INTAKE_SPEED, 2200);

  //touch ladder
  turn_drive_wait(-45, TURN_SPEED);
  move_drive_wait(20, DRIVE_SPEED);
}



