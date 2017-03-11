#pragma config(UART_Usage, UART1, uartUserControl, baudRate115200, IOPins, None, None)
#pragma config(I2C_Usage, I2C1, i2cSensors)
#pragma config(Sensor, in1,    intakePot,      sensorPotentiometer)
#pragma config(Sensor, dgtl1,  rightQuad,      sensorQuadEncoder)
#pragma config(Sensor, dgtl6,  leftQuad,       sensorQuadEncoder)
#pragma config(Sensor, dgtl10, stopBtn,        sensorTouch)
#pragma config(Sensor, dgtl11, skillsBtn,      sensorTouch)
#pragma config(Sensor, dgtl12, liftStopButton, sensorTouch)
#pragma config(Sensor, I2C_1,  liftIME,        sensorQuadEncoderOnI2CPort,    , AutoAssign )
#pragma config(Motor,  port1,           lidar,         tmotorVex393_HBridge, openLoop, reversed)
#pragma config(Motor,  port2,           liftRO,        tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port3,           liftLI,        tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port4,           driveLFY,      tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port5,           driveRB,       tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port6,           driveLB,       tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port7,           driveRFY,      tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port8,           intakeY,       tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port9,           liftLO,        tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port10,          liftRI,        tmotorVex393_HBridge, openLoop, encoderPort, I2C_1)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#pragma platform(VEX2)
#pragma competitionControl(Competition)
#include "Vex_Competition_Includes.c"


#define BCI_SEM_DEBUG_FULL_OFF
#define BCI_USE_UART
#define BCI_USE_TIMER
#define BCI_USE_POS_PID
#define BCI_USE_VEL_PID
#include "BCI\BCI.h"

//#define UARTHANDLER_DEBUG
//#define UARTHANDLER_DEBUG_READ
//#define MOVETOPOINT_DEBUG
#define POINTMATH_DEBUG

#include "uartHandler.c"
#include "pointMath.c"
#include "basicMotorControl.c"
#include "drivingFunctions.c"
#include "turningFunctions.c"
#include "intakeAndLiftHandler.c"
#include "collisionHandler.c"
#include "motorControl.c"
#include "decisionHandler.c"
#include "skills.c"
#include "noWarning.c"

task monitorStop()
{
	while (true)
	{
		if (SensorValue[stopBtn])
		{
			stopAllTasks();
			for (int i = 0; i < 10; i++)
			{
				motor[i] = 0;
			}
		}

		EndTimeSlice();
	}
}

void pre_auton()
{
  clearDebugStream();
  initUART();
  initSensors();
  motor[lidar] = 80;
}

task autonomous()
{
	startTask(monitorStop);

	intakeAndLiftTask_intakeState = INTAKE_WAIT;
	intakeAndLiftTask_liftState = LIFT_WAIT;
	startTask(intakeAndLiftTask);

	startTask(readBuffer);
	wait1Msec(250);

	//Fence stars
	intakeAndLiftTask_intakeState = INTAKE_OPEN;
	intakeAndLiftTask_liftState = LIFT_DOWN;
	driveStraight(-ONE_TILE_MM*1.5);
	dumpIntakeBasic();
	waitForLift(LIFT_DOWN);

	//Cube in center
	driveStraight(200);
	moveToPoint(609 + ONE_TILE_MM - 200, 304 + ONE_TILE_MM);
	turnToAbsAngle(0);
	intakeAndLiftTask_intakeState = INTAKE_CLOSED;
	wait1Msec(500);
	intakeAndLiftTask_liftCustomVal = 700;
	intakeAndLiftTask_liftState = LIFT_CUSTOM;
	turnToAbsAngle(-90);
	dumpIntakeBasic();
	waitForLift(LIFT_DOWN);

	//Wall stars
	moveToPoint(609 + ONE_TILE_MM*1.5, 304 + ONE_TILE_MM);
	turnToAbsAngle(-90);
	driveStraight(550);
	intakeAndLiftTask_intakeState = INTAKE_CLOSED;
	wait1Msec(100);
	driveStraight(-700);
	intakeAndLiftTask_intakeState = INTAKE_OPEN;
	driveStraight(200);
	intakeAndLiftTask_intakeState = INTAKE_CLOSED;
	wait1Msec(500);
	driveStraight(-200);
	intakeAndLiftTask_liftCustomVal = 700;
	intakeAndLiftTask_liftState = LIFT_CUSTOM;
	driveStraight(-200);
	dumpIntakeBasic();
}

task usercontrol()
{
	while (!vexRT[skillsBtn]) { wait1Msec(5); }

	startTask(autonomous);
	wait1Msec(60000);

	int fwdVal, turnVal;

  while (true)
  {
		fwdVal = vexRT[JOY_JOY_LV];
		turnVal = vexRT[JOY_JOY_LH];
		if (abs(fwdVal) < JOY_THRESHOLD)
			fwdVal = 0;
		if (abs(turnVal) < JOY_THRESHOLD)
			turnVal = 0;

		setLeftMotors(fwdVal + turnVal);
		setRightMotors(fwdVal - turnVal);

		if (vexRT[JOY_TRIG_LU])
			setIntakeMotors(64 * (vexRT[JOY_BTN_RL] - vexRT[JOY_BTN_RD]));
		else
			setIntakeMotors(127 * (vexRT[JOY_BTN_RL] - vexRT[JOY_BTN_RD]));

		if (vexRT[JOY_BTN_RR])
		{
			dumpIntakeBasic();
			intakeAndLiftTask_intakeState = INTAKE_WAIT;
			intakeAndLiftTask_liftState = LIFT_WAIT;
		}
		else
			setLiftMotors(127 * vexRT[JOY_TRIG_RU] - 127 * vexRT[JOY_TRIG_RD]);

		if (vexRT[JOY_BTN_RU])
		{
			motor[lidar] += 1;
			waitForZero(vexRT[JOY_BTN_RU]);
		}
		else if (vexRT[JOY_BTN_RD])
		{
			motor[lidar] -= 1;
			waitForZero(vexRT[JOY_BTN_RD]);
		}

		wait1Msec(15);
  }
}