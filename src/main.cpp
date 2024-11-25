#include "main.h"
#include "pros/motors.h"
#include "skills_auton.hpp"
#include "lemlib/api.hpp" // IWYU pragma: keep

pros::Motor lB(Port::LEFT_BACK_WHEEL_PORT, pros::v5::MotorGearset::blue);
pros::Motor lM(Port::LEFT_MIDDLE_WHEEL_PORT, pros::v5::MotorGearset::blue);
pros::Motor lF(Port::LEFT_FRONT_WHEEL_PORT, pros::v5::MotorGearset::blue);
pros::Motor rB(Port::RIGHT_BACK_WHEEL_PORT, pros::v5::MotorGearset::blue);
pros::Motor rM(Port::RIGHT_MIDDLE_WHEEL_PORT, pros::v5::MotorGearset::blue);
pros::Motor rF(Port::RIGHT_FRONT_WHEEL_PORT, pros::v5::MotorGearset::blue);

pros::MotorGroup leftMotors({
  Port::LEFT_BACK_WHEEL_PORT,
  Port::LEFT_MIDDLE_WHEEL_PORT,
  Port::LEFT_FRONT_WHEEL_PORT
}
);

pros::MotorGroup rightMotors({
  Port::RIGHT_BACK_WHEEL_PORT,
  Port::RIGHT_MIDDLE_WHEEL_PORT,
  Port::RIGHT_FRONT_WHEEL_PORT
});

lemlib::Drivetrain drivetrain{
  &leftMotors, //List of left motors
  &rightMotors, //List of right motors
  12.6, //Distance between left and right side of the drive
  lemlib::Omniwheel::NEW_325, //Wheel diameter (May be different than what the name suggests)
  360, //RPM of drive
  2 //Horizontal drift
};

lemlib::OdomSensors odomSensors{ //Odometry method, use nullptr if you dont have
  nullptr, //First vertical tracking wheel
  nullptr, //Second vertical tracking wheel
  nullptr, //First horizontal tracking wheel
  nullptr, //Second horizontal tracking wheel
  &imu //imu, declared in config.cpp
};

lemlib::ControllerSettings lateralController{ //PID for translational/lateral movements
  10, //kP - Proportional Gain
  0, //kI - Proportional Integral Gain
  3, //kD - Proportional Derivative Gain
  3, //AntiWindup
  1, //SmallError - Smaller Error range in inches
  100, //SmallErrorTimeout
  3, //LargeError - Larger Error range in inches
  500, //LargeError Timeout
  20, //Slew - Maximum acceleration
};
lemlib::ControllerSettings angularController{ //PID for rotational/angular/turning movements
  2, //kP - Proportional Gain
  0, //kI - Proportional Integral Gain
  10, //kD - Proportional Derivative Gain
  3, //AntiWindup
  1, //SmallError - Smaller Error range in degrees
  100, //SmallErrorTimeout
  3, //LargeError - Larger Error range in degrees
  500, //LargeError Timeout
  0, //Slew - Maximum acceleration
};


lemlib::Chassis chassis(
  drivetrain,
  lateralController,
  angularController,
  odomSensors
);

/* To tune PID do the following
Angular:
Mock auton to turn to a certain point, if it ossicilates about the target, increase kD
If it doesn't oscilate increase kP
Repeat until no amount of kD stops the ossicilation
kI rarely needs tuning as it is steady state error


*/


/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 */
void initialize() {
	pros::delay(500); // Stop the user from doing anything while legacy ports configure.

  //Lemlib intialization
  pros::lcd::initialize();
  chassis.calibrate();
  
}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {chassis.setBrakeMode(pros::E_MOTOR_BRAKE_COAST);
}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {}

/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 */

void autonomous() {
  chassis.curvature(0, 0);

  pros::Task screen_task([&]() {
        chassis.setPose(0,0,0);
        while (true) {
            // print robot location to the brain screen
            master.print(0,0, "X: %.1lf Y: %.1lf R: %.1lf",chassis.getPose().x, chassis.getPose().y, chassis.getPose().theta); // y
            // delay to save resources
            pros::delay(300);
        }
    });
  pros::Task autAutoClamp(autoClamp);
  chassis.turnToHeading(0,1000);
  chassis.waitUntilDone();
  doink();
  chassis.turnToHeading(60,1000);
  chassis.turnToHeading(15,1000);
  chassis.waitUntilDone();
  undoink();
  chassis.moveToPoint(-60,-60, 3000);
  chassis.waitUntilDone();

  autAutoClamp.remove();
  // tune_PID();
  // sorterAut.remove();
}

/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 */

void opcontrol() { 
    //chassis.set_drive_brake(pros::E_MOTOR_BRAKE_COAST);
    enum{RED,BLUE};
    setTeam(); //no input takes color sensor input for team
    
    pros::Task intakeT(intakes);
    // pros::Task intakeConSortT(intakesConSorter);
    //pros::Task clampT(clamps);
    pros::Task autoclampT(autoClamp);
    pros::Task wall_scoreT(wall_score);
    // pros::Task sorterT(sorter);
    pros::Task doinkerT(doinker);

    
    
    // pros::Task debugTurnT(debugTurn);
    // pros::Task debugDriveT(debugDrive);

    //Lemlib arcade drive
    while(true)
    {
      //Get joystick values
      int leftY = master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
      int rightX = master.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X);

      chassis.arcade(leftY, rightX);
      pros::delay(10);
    }
  	pros::delay(10); 
        // Used for timer calculations
    
}

//12 red, 11 blue

