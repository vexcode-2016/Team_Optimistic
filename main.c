#pragma config(UART_Usage, UART1, uartUserControl, baudRate115200, IOPins, None, None)
#pragma config(Sensor, in1,    intakePot,      sensorPotentiometer)
#pragma config(Sensor, in2,    liftPot,        sensorPotentiometer)
#pragma config(Sensor, dgtl1,  rightQuad,      sensorQuadEncoder)
#pragma config(Sensor, dgtl6,  leftQuad,       sensorQuadEncoder)
#pragma config(Sensor, dgtl10, stopBtn,        sensorTouch)
#pragma config(Sensor, dgtl11, testBtn,        sensorTouch)
#pragma config(Sensor, dgtl12, liftStopButton, sensorTouch)
#pragma config(Motor,  port1,           liftRO,        tmotorVex393_HBridge, openLoop)
#pragma config(Motor,  port2,           intakeY,       tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port3,           liftLI,        tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port4,           driveRB,       tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port5,           driveRFY,      tmotorVex393_MC29, openLoop, encoderPort, dgtl1)
#pragma config(Motor,  port6,           driveLFY,      tmotorVex393_MC29, openLoop, encoderPort, dgtl6)
#pragma config(Motor,  port7,           driveLB,       tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port8,           liftRI,        tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port9,           liftLO,        tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port10,          lidar,         tmotorVex393_HBridge, openLoop, reversed, encoderPort, None)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#define BCI_SEM_DEBUG_FULL_OFF
#define BCI_USE_UART
#define BCI_USE_TIMER
#define BCI_USE_POS_PID
#define BCI_USE_VEL_PID
#include "BCI\BCI.h"

//#define UARTHANDLER_DEBUG
//#define UARTHANDLER_DEBUG_READ
//#define MOVETOPOINT_DEBUG
//#define POINTMATH_DEBUG
#define MPC_DEBUG

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

task main()
{
	startTask(monitorStop);

	clearDebugStream();

	motor[lidar] = 55; //64 at 7.51V, 48 at 8.27V

	initUART();
	initSensors();

	//startTask(testDrive);
	//startTask(testLift);
	intakeAndLiftTask_intakeState = INTAKE_REST;
  intakeAndLiftTask_liftState = LIFT_REST;
	startTask(intakeAndLiftTask);
	startTask(readBuffer);
	wait1Msec(250);

/*
	while(!vexRT[Btn8D]){wait1Msec(15);}

	driveStraight(-600);

	while (true)
	{
		moveToPoint(914 + TWO_TILE_MM, 304 + ONE_TILE_MM);
		wait1Msec(1000);
		moveToPoint(914, 304 + ONE_TILE_MM);
		wait1Msec(1000);

		if (vexRT[JOY_BTN_LD])
		{
			int vert, hor;
			while (true)
			{
				vert = vexRT[JOY_JOY_LV];
				hor = vexRT[JOY_JOY_LH];

				if (abs(vert) < JOY_THRESHOLD)
					vert = 0;
				if (abs(hor) < JOY_THRESHOLD)
					hor = 0;

				setLeftMotors(vert + hor);
				setRightMotors(vert - hor);

				if (vexRT[JOY_BTN_LD])
				{
					setLeftMotors(0);
					setRightMotors(0);
					break;
				}

				wait1Msec(15);
			}
		}
	}
*/

	while(!vexRT[Btn8D]){wait1Msec(15);}
	driveStraight(-550);
	intakeAndLiftTask_intakeState = INTAKE_OPEN;
	intakeAndLiftTask_liftState = LIFT_DOWN;
	startTask(commandRobot);

	while (true) { wait1Msec(15); }

	//int vert, hor;

	//while (true)
	//{
	//	vert = vexRT[JOY_JOY_LV];
	//	hor = vexRT[JOY_JOY_LH];

	//	if (abs(vert) < JOY_THRESHOLD)
	//		vert = 0;
	//	if (abs(hor) < JOY_THRESHOLD)
	//		hor = 0;

	//	setLeftMotors(vert + hor);
	//	setRightMotors(vert - hor);

	//	wait1Msec(15);
	//}
}
