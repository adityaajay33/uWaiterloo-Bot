/*
DriveToLocation
*/

//CONSTANTS
const int RESTAURANT_HEIGHT = 3;
const int RESTAURANT_WIDTH = 3;
const int STANDARD_M_POWER = 50;
const int table_locations[RESTAURANT_HEIGHT][RESTAURANT_WIDTH] = {{1,2,3},
																																	{4,5,6},
																																	{7,8,9}};

//STRUCTURES
typedef struct
{
	int row, col;

}Location;

//FUNCTIONS

/*
 * configureAllSensors()
 * Configures all sensors for the robot.
 */
void configureAllSensors(){
    SensorType[S1] = sensorEV3_Color;
 		wait1Msec(50);
  	SensorMode[S1] = modeEV3Color_Color;
    wait1Msec(50);
    SensorType[S2] = sensorEV3_Color;
 		wait1Msec(50);
  	SensorMode[S2] = modeEV3Color_Color;
    wait1Msec(50);
    SensorType[S3] = sensorEV3_Ultrasonic;
    wait1Msec(50);
    SensorType[S4] = sensorEV3_Gyro;
    wait1Msec(50);
    SensorType[S4] = modeEV3Gyro_Calibration;
    wait1Msec(50);
    SensorType[S4] = modeEV3Gyro_RateAndAngle;
    wait1Msec(50);
}


/*
 * getDriveLocation()
 *
 * Function is passed a target table number and a location object by reference.
 * The location object is modified to contain the row and column of the table.
 *
 * @param table_num The number of the table which needs to be located.
 * @param &target Location object to be modified by reference.
 */
void getDriveLocation(int table_num, Location &target)
{
	for (int row=0; row < RESTAURANT_HEIGHT; row++) {
		for (int col=0; col < RESTAURANT_WIDTH; col++) {
			if (table_locations[row][col] == table_num){
				target.row = row;
				target.col = col;
				return;
			}
		}
	}
	return;
}

/*
 * driveForward()
 *
 * Drives the robot forward until the next turning point which is
 * indicated by a blue tile on the floor
 */
void driveForward(int motor_power)
{
	motor[motorA] = motor[motorC] = -motor_power;

	while(SensorValue[S2] != (int)colorBlue) {}

	motor[motorA] = motor[motorC] = 0;
}


/*
 * turn()
 *
 * Rotates the robot by an angle and stops
 *
 * @param angle The angle whic hthe robot turns
 */
void turn(int angle)
{
	SensorValue[S4] = 0;

	if (angle>0){
		motor[motorA] = -25;
		motor[motorC] = 25;
	}else{
		motor[motorA] = 25;
		motor[motorC] = -25;
	}

	while (abs(SensorValue[S4])<abs(angle)) {}

 	motor[motorA] = motor[motorC] = 0;
}

/*
 * leaveKitchen()
 *
 * Moves the robot from the kitchen area to the start of the restaurant
 */
void leaveKitchen()
{
	turn(180);
	driveForward(STANDARD_M_POWER);
	turn(90);
	driveForward(STANDARD_M_POWER);

}

/*
 * driveForward()
 *
 * Drives the robot forwards and turns towards to nearest table so that it
 * is ready to drop off the plate
 *
 * @param motor_power The motor power that the robot moves at.
 */
void driveToTable (int motor_power)
{
	motor[motorA] = motor[motorC] = motor_power;

	while (SensorValue[S3] != (int)(colorYellow)) {}

	motor[motorA] = motor[motorC] = 0;

	turn(90);
	
	motor[motorA] = motor[motorC] = motor_power;
	
	while (SensorValue[S3] > 6) {}
	
	motor[motorA] = motor[motorC] = 0;
}

/*
 * driveToLocation()
 *
 * Drives the robot to the actual given location of the table
 * location with various helper functions.
 *
 * @param target The x and y coordinate of the table in the restaurant grid
 */
//drives to the actual table location from start(UNTESTED)
void driveToLocation (Location &target)
{
	leaveKitchen();

	for (int count = 0; count < (target.row) - 1; count++) {
		driveForward(STANDARD_M_POWER);
	}
	turn(90);
	for (int count = 0; count < (target.col) - 1; count++) {
		driveForward(STANDARD_M_POWER);
	}

	driveToTable(STANDARD_M_POWER);
}

//returns back to home base (UNFINISHED)
void driveHome(Location &target)
{
	for (int count = 0; count < target.col - 1; count++) {
		driveForward(STANDARD_M_POWER);
	}
	turn(-90);
	for (int count = 0; count < target.row - 1; count++) {
		driveForward(STANDARD_M_POWER);
	}
	
}


//MAIN PROGRAM

task main()
{
	configureAllSensors();
	
	Location target;
	int table_num = 4;
	getDriveLocation(table_num, target);
	
	//testing area
	driveToLocation(target);
	//sayan's deliver food function
	driveHome(target);
}
