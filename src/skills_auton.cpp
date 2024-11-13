#include "EZ-Template/util.hpp"
#include "skills_auton.hpp"
#include "display/lv_hal/lv_hal_indev.h"
#include "helper_functions.hpp"
#include "main.h"

const int DRIVE_SPEED = 110; // This is 110/127 (around 87% of max speed). When this is 87%, the robot is correcting by making one side
                            // faster and one side slower, giving better heading correction.
const int TURN_SPEED = 65;
const int SWING_SPEED = 90;
const int INTAKE_SPEED = 550;
char clamp_portAUTON = 'G';

// int TEAM = 0; //Someone remind me to fix this, idk where to declare team color, and this code kinda ugly ngl

// char sorter_portOP = 'A'; //fix
// int color_port = 1; //fix
#define ON 1
#define OFF 0

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

// int sorterSkill()
// {
//     pros::ADIDigitalOut sorter(sorter_portOP);
//     pros::Optical light(color_port);
//     pros::c::optical_rgb_s_t rgbVal;
//     enum{RED, BLUE};
//     double blueThreshold = 10;
//     double redThreshold = 10;
//     int color = TEAM;
//     bool sorterState = false;
//     bool found = 0;

//     sorter.set_value(OFF);


//     while (!found) {
//       rgbVal = light.get_rgb();
//       if(rgbVal.blue > blueThreshold) {color = BLUE;}
//       else if(rgbVal.red > redThreshold) {color = RED;}
      
//       master.print(0,2,"Color: %s", color);
//       wait(150);

//       if (color == BLUE) {
//         return 1;
//       }
//       else if (color == RED) {
//         return 0;
//       }
//       else {
//         continue;
//       }
//     }
// }




//---------------------------------------------------------------------- Autons YR 2024-2025
// Path 2 (Simple): Red, 2 donuts and touches
void bottom_red_simple() {
  // Set-up: piston to ADI port clamp_port
  bool clamp_state = false;
  pros::ADIDigitalOut clamp (clamp_portAUTON);
  clamp.set_value(clamp_state); // retracted (unclamped)

  // Begin movement
  move_drive_wait(-27, DRIVE_SPEED);
  clamp.set_value(!clamp_state); // clamps down on bottom left mogol
  wait(500);

  // turn intake towards stacked rings (target is bottom red)
  turn_drive_wait(-110, TURN_SPEED);
  move_drive_wait(10, DRIVE_SPEED);
  // intaker_wait(INTAKE_SPEED, 1800);
  intaker(400);
  move_drive_wait(15, DRIVE_SPEED);

  // intake and turn + move to touch Ladder
  turn_drive_wait(-252, TURN_SPEED);
  move_drive_wait(30, DRIVE_SPEED-40);
  intaker(0);

  clamp.set_value(clamp_state);

}

// bottom red hard
void bottom_red_hard() {
  // Set-up: piston to ADI port clamp_port
  bool clamp_state = false;
  pros::ADIDigitalOut clamp (clamp_portAUTON);
  clamp.set_value(clamp_state); // retracted (unclamped)

  // Begin movement
  move_drive_wait(-32, DRIVE_SPEED-20);
  clamp.set_value(!clamp_state); // clamps down on bottom left mogol
  wait(500);

  // turn intake towards stacked rings (target is bottom red)
  turn_drive_wait(-110, TURN_SPEED);
  move_drive_wait(10, DRIVE_SPEED);

  intaker(400);
  move_drive_wait(15, DRIVE_SPEED);
  wait(500);
  turn_drive_wait(-50, TURN_SPEED);
  wait(1500);
  intaker(0);
  move_drive_wait(30, DRIVE_SPEED-30);
  clamp.set_value(clamp_state);

}

void top_red_simple() {
// Set-up: piston to ADI port clamp_port
  bool clamp_state = false;
  pros::ADIDigitalOut clamp (clamp_portAUTON);
  clamp.set_value(clamp_state); // retracted (unclamped)
  wait(1000);
  // Begin movement
  move_drive_wait(-29, DRIVE_SPEED);
  clamp.set_value(!clamp_state); // clamps down on bottom left mogol

  // turn intake towards stacked rings (target is bottom blue)
  turn_drive_wait(110, TURN_SPEED);
  intaker(400);
  move_drive_wait(20, DRIVE_SPEED);

  // intake and turn + move to touch Ladder
  turn_drive_wait(120, TURN_SPEED);
  move_drive_wait(30, DRIVE_SPEED);
  wait(1000);
  move_drive_wait(-20, DRIVE_SPEED);
  clamp.set_value(clamp_state);
  intaker(0);

}


// Work In Progress ----------------------------
// void bottom_blue_possibly() {
//   // Set-up: piston to ADI port clamp_port
//   bool clamp_state = false;
//   pros::ADIDigitalOut clamp (clamp_portAUTON);
//   clamp.set_value(clamp_state); // retracted (unclamped)

//   // Begin movement
//   move_drive_wait(-27, DRIVE_SPEED);
//   clamp.set_value(!clamp_state); // clamps down on bottom left mogol
//   wait(500);

//   // turn intake towards stacked rings (target is bottom red)
//   turn_drive_wait(-110, -TURN_SPEED);
//   move_drive_wait(10, DRIVE_SPEED);
//   // intaker_wait(INTAKE_SPEED, 1800);
//   intaker(400);
//   move_drive_wait(15, DRIVE_SPEED);

//   // intake and turn + move to touch Ladder
//   turn_drive_wait(-252, -TURN_SPEED);
//   move_drive_wait(30, DRIVE_SPEED-40);
//   intaker(0);

//   clamp.set_value(clamp_state);
// }
//----------------------------------------



// Path 3: Red, 2 donuts and touches Ladder.
void mid_red_line_rush(){
  bool clamp_state = false;
  pros::ADIDigitalOut clamp (clamp_portAUTON);
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

// Path 4: bottom blue simple
void bottom_blue_simple() {
  // Set-up: piston to ADI port clamp_port
  bool clamp_state = false;
  pros::ADIDigitalOut clamp (clamp_portAUTON);
  clamp.set_value(clamp_state); // retracted (unclamped)
  wait(1000);
  // Begin movement
  move_drive_wait(-29, DRIVE_SPEED);
  clamp.set_value(!clamp_state); // clamps down on bottom left mogol

  // turn intake towards stacked rings (target is bottom blue)
  turn_drive_wait(110, TURN_SPEED);
  intaker(400);
  move_drive_wait(20, DRIVE_SPEED);

  // intake and turn + move to touch Ladder
  turn_drive_wait(250, TURN_SPEED);
  move_drive_wait(27, DRIVE_SPEED);
  wait(1000);
  clamp.set_value(clamp_state);
  intaker(0);
  
}

void top_blue_simple() {
// Set-up: piston to ADI port clamp_port
  bool clamp_state = false;
  pros::ADIDigitalOut clamp (clamp_portAUTON);
  clamp.set_value(clamp_state); // retracted (unclamped)

  // Begin movement
  move_drive_wait(-32, DRIVE_SPEED-20);
  clamp.set_value(!clamp_state); // clamps down on bottom left mogol
  wait(500);

  // turn intake towards stacked rings (target is bottom red)
  turn_drive_wait(-110, TURN_SPEED);
  move_drive_wait(10, DRIVE_SPEED);

  intaker(400);
  move_drive_wait(15, DRIVE_SPEED);
  wait(500);
  turn_drive_wait(-180, TURN_SPEED);
  wait(1500);
  move_drive_wait(10, DRIVE_SPEED-30);
  wait(4000);
  intaker(0);

  clamp.set_value(clamp_state);


}



//------------------ SKILLS ---------------------
// Path 2
void skills(){
  bool clamp_state = false;
  pros::ADIDigitalOut clamp (clamp_portAUTON);
  clamp.set_value(clamp_state); // retracted (unclamped)

  //red
  move_drive_wait(-10, DRIVE_SPEED);
  clamp.set_value(!clamp_state); //get mogol
  move_drive_wait(-4, DRIVE_SPEED);

  turn_drive_wait(-90, TURN_SPEED);
  intaker(INTAKE_SPEED);
  move_drive_wait(16, DRIVE_SPEED);
  intaker_wait(INTAKE_SPEED, 400);
  intaker(INTAKE_SPEED);
  turn_drive_wait(-30, TURN_SPEED);
  move_drive_wait(14, DRIVE_SPEED);
  intaker_wait(INTAKE_SPEED, 400);
  intaker(400);
  move_drive_wait(-13, DRIVE_SPEED);
  turn_drive_wait(-88, TURN_SPEED);
  move_drive_wait(16, DRIVE_SPEED);
  move_drive_wait(-16, DRIVE_SPEED);
  turn_drive_wait(152, TURN_SPEED);
  move_drive_wait(24, DRIVE_SPEED);

  turn_drive_wait(135, TURN_SPEED);
  move_drive_wait(23, DRIVE_SPEED);
  intaker_wait(INTAKE_SPEED, 300);
  intaker(INTAKE_SPEED);

  move_drive_wait(-25, DRIVE_SPEED);
  turn_drive_wait(-45, TURN_SPEED);
  move_drive_wait(-35, DRIVE_SPEED);
  clamp.set_value(clamp_state);
  move_drive_wait(35, DRIVE_SPEED);




  turn_drive_wait(-135, TURN_SPEED);
  move_drive_wait(30, DRIVE_SPEED);
  intaker_wait(INTAKE_SPEED, 300);
  intaker(INTAKE_SPEED);


  // intaker(INTAKE_SPEED);
  // move_drive_wait(20, DRIVE_SPEED);
  // intaker(0); //scores 1 ring
  // turn_drive_wait(-90, TURN_SPEED);
  // intaker(INTAKE_SPEED);
  // move_drive_wait(5, DRIVE_SPEED);
  // intaker(0); // 1 ring
  // turn_drive_wait(-180, TURN_SPEED);
  // intaker(INTAKE_SPEED);
  // move_drive_wait(5, DRIVE_SPEED);
  // intaker(0); // 1 ring
  // turn_drive_wait(-90, TURN_SPEED);
  // intaker(INTAKE_SPEED);
  // move_drive_wait(13, DRIVE_SPEED);
  // intaker(0); // 1 ring
  // turn_drive_wait(-210, TURN_SPEED);
  // intaker(INTAKE_SPEED);
  // move_drive_wait(25, DRIVE_SPEED);
  // intaker(0); // 1 ring
  // turn_drive_wait(-150,TURN_SPEED);
  // move_drive_wait(26, DRIVE_SPEED);

  // clamp.set_value(clamp_state);



  //orange
  // turn_drive_wait(-135, TURN_SPEED);
  // move_drive_wait(50, DRIVE_SPEED);
  // intaker_wait(INTAKE_SPEED, 1000);
  // turn_drive_wait(-315, TURN_SPEED);
  // move_drive_wait(50, DRIVE_SPEED);
  // intaker_wait(INTAKE_SPEED, 1000); //2 rings on the robot
  // turn_drive_wait(-180, TURN_SPEED);
  // move_drive_wait(50, DRIVE_SPEED);
  // clamp.set_value(!clamp_state); //get mogol
  // intaker_wait(INTAKE_SPEED, 1200); //score the 2 rings on the robot
  // turn_drive_wait(-270, TURN_SPEED);
  // move_drive_wait(20, DRIVE_SPEED); 
  // intaker_wait(INTAKE_SPEED, 2200); //3 rings scored
  // turn_drive_wait(0, TURN_SPEED);
  // move_drive_wait(5, DRIVE_SPEED);
  // intaker_wait(INTAKE_SPEED, 2200); //4 rings scored
  // turn_drive_wait(-225, TURN_SPEED);
  // move_drive_wait(10, DRIVE_SPEED); 
  // intaker_wait(INTAKE_SPEED, 2200); //5 rings scored
  // turn_drive_wait(-135, TURN_SPEED);
  // move_drive_wait(50, DRIVE_SPEED);
  // intaker_wait(INTAKE_SPEED, 2200); //6 rings scored
  // clamp.set_value(clamp_state); 

  //yellow
  // turn_drive_wait(-225, TURN_SPEED);
  // move_drive_wait(30, DRIVE_SPEED);
  // intaker_wait(INTAKE_SPEED, 1000); 
  // turn_drive_wait(-135, TURN_SPEED); 
  // move_drive_wait(30, DRIVE_SPEED);
  // intaker_wait(INTAKE_SPEED, 1000); //2 rings stored
  // turn_drive_wait(80, TURN_SPEED);
  // move_drive_wait(60, DRIVE_SPEED);
  // clamp.set_value(!clamp_state); //get mogol
  // intaker_wait(INTAKE_SPEED, 1200); //score 2 rings
  // turn_drive_wait(70, TURN_SPEED);
  // move_drive_wait(20, DRIVE_SPEED);
  // intaker_wait(INTAKE_SPEED, 2200); //3 rings scored
  // turn_drive_wait(-100, TURN_SPEED);
  // move_drive_wait(40, DRIVE_SPEED);
  // turn_drive_wait(-45, TURN_SPEED);
  // move_drive_wait(40, DRIVE_SPEED);
  // intaker_wait(INTAKE_SPEED, 2200); //4 rings scored
  // turn_drive_wait(-90, TURN_SPEED);
  // move_drive_wait(30, DRIVE_SPEED);
  // intaker_wait(INTAKE_SPEED, 2200); //5 rings scored
  // turn_drive_wait(-45, TURN_SPEED);
  // move_drive_wait(40, DRIVE_SPEED);
  // intaker_wait(INTAKE_SPEED, 2200); //6 rings scored
  // clamp.set_value(clamp_state); 
}