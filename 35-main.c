#pragma config(UserModel, "C:/Users/rstudent/code/robot-configs/35.c")
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#pragma platform(VEX)

//Competition Control and Duration Settings
#pragma competitionControl(Competition)
#pragma autonomousDuration(20)
#pragma userControlDuration(120)

#include "Vex_Competition_Includes.c"   //Main competition background code...do not modify!


#include <CKFlywheelSpeedController.h>



// A e^(B x)
// 1.2889 e 0.0925 x
#define A		1.2889
#define B		0.0925

#define Kq	0.05
#define Ki	0.03
#define Kd	0.0

const bool iWantSpeedDial = true;



#define trigger2power(BtnSet)		(vexRT[BtnSet ## U] ? 127 : (vexRT[BtnSet ## D] ? -127 : 0))




FlywheelSpeedController flywheelController;




void drive(int left, int right){
	motor[leftDrive] = left;
	motor[rightDrive] = right;
}



// only give something if not in the "dead" zone
int livingJoy(int ch){
	int value = vexRT[ch];
	return abs(value) > 12 ? value : 0;
}


float targetV = 0;


task FlywheelSpeedControl()
{
	while(true){
		setTargetSpeed( flywheelController, targetV );
		update( flywheelController );

		delay(40);
	}
}



//task LCDUpdate() {
//	string line0, line1;
//	float speedL, speedR;
//	float vExpander, vMain;

//	while(true){
//		delay(250);

//		speedL = getAverage( leftFlywheelController.maFlywheelSpeed );

//		vExpander = SensorValue[vPowerExpander] / 280.0;
//		vMain = MainBatteryVoltage();

//		sprintf(line0, "%.1f %.1f %.1f", targetV, speedL, speedR);
//		sprintf(line1, "%.2f %.2f", vExpander, vMain);
//		clearLCDLine(0);
//		displayLCDString(0, 0, line0);
//		clearLCDLine(1);
//		displayLCDString(1, 0, line1);
//	}
//}





float speedDialValue(){
	return 10*potentiometer(speedDial); //rad/sec of motor output shaft
}

task testWheelsWithDial(){
	while(true){
		targetV = speedDialValue();
		delay(200);
	}
}





/////////////////////////////////////////////////////////////////////////////////////////
//
//                          Pre-Autonomous Functions
//
// You may want to perform some actions before the competition starts. Do them in the
// following function.
//
/////////////////////////////////////////////////////////////////////////////////////////

void pre_auton()
{
  // Set bStopTasksBetweenModes to false if you want to keep user created tasks running between
  // Autonomous and Tele-Op modes. You will need to manage all user created tasks if set to false.
  bStopTasksBetweenModes = true;

	tMotor motorPorts[] = { mFly1, mFly2, mFly3, mFly4 };


	FlywheelSpeedControllerInit( flywheelController, Kq, Ki, Kd, A, B, motorPorts, 4 );

}

/////////////////////////////////////////////////////////////////////////////////////////
//
//                                 Autonomous Task
//
// This task is used to control your robot during the autonomous phase of a VEX Competition.
// You must modify the code to add your own robot specific commands here.
//
/////////////////////////////////////////////////////////////////////////////////////////

task autonomous()
{
	startTask(FlywheelSpeedControl);

}




/////////////////////////////////////////////////////////////////////////////////////////
//
//                                 User Control Task
//
// This task is used to control your robot during the user control phase of a VEX Competition.
// You must modify the code to add your own robot specific commands here.
//
/////////////////////////////////////////////////////////////////////////////////////////

task usercontrol()
{
	startTask(FlywheelSpeedControl);

	if( iWantSpeedDial ){
		startTask(testWithSpeedDial);
	}


	while(true){

		// A few preset flywheel speeds
		  //   if( vexRT[Btn8U] ) targetV = 10;
	//else if( vexRT[Btn8R] ) targetV = 8;
//	else if( vexRT[Btn8D] ) targetV = 7.2;
//targetV = 10;
		motor[intakeUp] = trigger2power(Btn6);//vexRT[Btn6U] ? 127 : (vexRT[Btn6D] ? -127 : 0);
		motor[intakeRoller] = trigger2power(Btn5);//vexRT[Btn5U] ? 127 : (vexRT[Btn5D] ? -127 : 0);
		// Tank drive with joystick deadzone eliminated
		drive( livingJoy(ChJoyLY), livingJoy(ChJoyRY) );

		delay(10);
	}

}
