#include "main.h"
#include "pros/motors.h"
#include "skills_auton.hpp"
#include "lemlib/api.hpp" // IWYU pragma: keep

pros::MotorGroup leftMotors({
  Port::LEFT_FARBACK_WHEEL_PORT,
  Port::LEFT_MIDDLE_WHEEL_PORT,
  Port::LEFT_FRONT_WHEEL_PORT
});
pros::MotorGroup rightMotors({
  Port::RIGHT_FARBACK_WHEEL_PORT,
  Port::RIGHT_MIDDLE_WHEEL_PORT,
  Port::RIGHT_FRONT_WHEEL_PORT
});
lemlib::Drivetrain drivetrain{
  &leftMotors, //List of left motors
  &rightMotors, //List of right motors
  16, //Distance between left and right side of the drive
  2.75, //Wheel diameter (May be different than what the name suggests)
  480, //RPM of drive
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








/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 */
void initialize() {
	pros::delay(500); // Stop the user from doing anything while legacy ports configure.

  

  // Initialize chassis and auton selector

  //Lemlib intialization
  
  chassis.calibrate();
  pros::Task screen_task([&]() {
        while (true) {
            // print robot location to the brain screen
            pros::lcd::print(1, "X: %.1f Y: %.1f R: %.1f",chassis.getPose().x, chassis.getPose().y, chassis.getPose().theta); // y
            // delay to save resources
            pros::delay(20);
        }
    });
}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {//drive coast
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
  //chassis.reset_pid_targets(); // Resets PID targets to 0
  //chassis.reset_gyro(); // Reset gyro position to 0
    //chassis.reset_drive_sensor(); // Reset drive sensors to 0
  	// chassis.set_drive_brake(pros::E_MOTOR_BRAKE_HOLD); // Set motors to hold.  This helps autonomous consistency.
  //chassis.set_drive_brake(pros::E_MOTOR_BRAKE_HOLD);
  	//ez::as::auton_selector.call_selected_auton(); // Calls selected auton from autonomous selector.

  // pros::Task sorterAut(sorter);
  skills();
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
    pros::Task clampT(clamps);
    pros::Task wall_scoreT(wall_score);
    pros::Task sorterT(sorter);
    pros::Task doinkerT(doinker);
    
    // pros::Task debugTurnT(debugTurn);
    // pros::Task debugDriveT(debugDrive);

    //Easytemplate arcade drive
    // while (true) {
    //     chassis.arcade_standard(ez::SPLIT);
    // }

    //Lemlib arcade drive
    while(true)
    {
      //Get joystick values
      int leftY = master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
      int rightX = master.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X);

      chassis.arcade(leftY, rightX);
      pros::delay(25);
    }
  	pros::delay(20); 
        // Used for timer calculations
    
}

