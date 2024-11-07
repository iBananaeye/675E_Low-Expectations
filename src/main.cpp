#include "main.h"
#include "EZ-Template/util.hpp"
#include "pros/motors.h"
#include "skills_auton.hpp"
Drive chassis (
  // Left Chassis Ports (negative port will reverse it)
  {
    Port::LEFT_FARBACK_WHEEL_PORT,
    Port::LEFT_MIDDLE_WHEEL_PORT,
    Port::LEFT_FRONT_WHEEL_PORT
  }

  // Right Chassis Ports
  ,{
    Port::RIGHT_FARBACK_WHEEL_PORT,
    Port::RIGHT_MIDDLE_WHEEL_PORT,
    Port::RIGHT_FRONT_WHEEL_PORT
  }

  // IMU Port
  ,Port::IMU_PORT

  // Wheel Diameter (Remember, 4" wheels are actually 4.125!)
  //    (or tracking wheel diameter)

  ,3.25

  // Cartridge RPM
  //   (or tick per rotation if using tracking wheels)
  ,600

  // External Gear Ratio (MUST BE DECIMAL)
  //    (or gear ratio of tracking wheel)
  // eg. if your drive is 84:36 where the 36t is powered, your RATIO would be 2.333.
  // eg. if your drive is 36:60 where the 60t is powered, your RATIO would be 0.6.
  ,1.66667 // 60:36 TEST 0.6 VALUE AS WELL TO MAKE SURE DRIVE IS RUNNING AT FULL POWER.

);

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 */
void initialize() {
	pros::delay(500); // Stop the user from doing anything while legacy ports configure.

  default_constants();
  chassis.toggle_modify_curve_with_controller(false); // Enables modifying the controller curve with buttons on the joysticks
  chassis.set_active_brake(0.0); // Sets the active brake kP. We recommend 0.1.
  chassis.set_curve_default(0, 0); // Defaults for curve. If using tank, only the first parameter is used. 
	ez::as::auton_selector.add_autons({

  });

    // Initialize chassis and auton selector
    chassis.initialize();
  	ez::as::initialize();
    master.clear();
}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {chassis.set_drive_brake(pros::E_MOTOR_BRAKE_COAST);}

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
  chassis.reset_pid_targets(); // Resets PID targets to 0
  chassis.reset_gyro(); // Reset gyro position to 0
    //chassis.reset_drive_sensor(); // Reset drive sensors to 0
  	// chassis.set_drive_brake(pros::E_MOTOR_BRAKE_HOLD); // Set motors to hold.  This helps autonomous consistency.
  chassis.set_drive_brake(pros::E_MOTOR_BRAKE_HOLD);
  	//ez::as::auton_selector.call_selected_auton(); // Calls selected auton from autonomous selector.

  top_blue_simple();
}

/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 */

void opcontrol() {
    chassis.set_drive_brake(pros::E_MOTOR_BRAKE_COAST);
    pros::Task intakeT(intakes);
    pros::Task clampT(clamps);
    pros::Task wall_scoreT(wall_score);
    pros::Task sorterT(sorter);
    pros::Task doinkerT(doinker);
    
    //pros::Task debugTurnT(debugTurn);
    //pros::Task debugDriveT(debugDrive);

    while (true) {
        chassis.arcade_standard(ez::SPLIT);
    }
  	pros::delay(ez::util::DELAY_TIME); 
        // Used for timer calculations
    
}