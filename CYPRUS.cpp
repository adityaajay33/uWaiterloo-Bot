/******************************************************************
CYPRUS.c
Main Program for the WaiterlooBot

Written by: Aditya Ajay, Andrew Chai, Brian Zhang, and Sayan Saha
******************************************************************/

//******************CONSTANTS******************//

const int RESTAURANT_HEIGHT = 2;
const int RESTAURANT_WIDTH = 3;
const int STANDARD_M_POWER = 50;
const int NUM_X_POS = 50;
const int NUM_Y_POS = 50;
const int table_locations[RESTAURANT_HEIGHT][RESTAURANT_WIDTH] = {{1,2,3},
																																	{4,5,6}};
const float TAX = 1.13;

//******************ARRAYS******************//

float bill[RESTAURANT_HEIGHT*RESTAURANT_WIDTH] = {0,0,0,0,0,0};

//******************STRUCTURES******************//

typedef struct
{
	int row, col;

}Location;


//******************FUNCTIONS******************//

/*
 * configureAllSensors()
 *
 * Configures all sensors for the robot.
 */
void configureAllSensors()
{
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
	SensorMode[S4] = modeEV3Gyro_Calibration;
	wait1Msec(100);
	SensorMode[S4] = modeEV3Gyro_RateAndAngle;
	wait1Msec(50);
}

/*
 * setMotors()
 *
 * Sets the speeds for motor A and motor C. Powers are reversed due
 * to the motors being physically backwards on robot.
 *
 * @param motor_power_A The power set for motor A
 * @param motor_power_C The power set for motor C
 */
void setMotors(int motor_power_A, int motor_power_C)
{
	motor[motorA] = -motor_power_A;
	motor[motorC] = -motor_power_C;
}

void followPath()
{
	setMotors(0,0);
	const int SLOW = 10;

	bool found_line = false;
	const int TIME = 1000;

	time1[T1] = 0;
	motor[motorA] = SLOW;
	motor[motorC] = -SLOW;
	while(time1[T1] < TIME && !found_line){
		if ((SensorValue[S2] == (int)colorBlack) || (SensorValue[S2] == (int)colorRed)) {
			found_line = true;
		}
	}

	while (SensorValue[S2] != (int)colorBlack && !found_line) {
		time1[T1] = 0;
		motor[motorA] = -SLOW;
		motor[motorC] = SLOW;
		while(time1[T1] < 2*TIME && !found_line){
			if ((SensorValue[S2] == (int)colorBlack) || (SensorValue[S2] == (int)colorRed)) {
				found_line = true;
			}
		}
		motor[motorA] = SLOW;
		motor[motorC] = -SLOW;
		time1[T1] = 0;

		while(time1[T1] < 2*TIME && !found_line){
			if ((SensorValue[S2] == (int)colorBlack) || (SensorValue[S2] == (int)colorRed)) {
				found_line = true;
			}
		}
	}
	setMotors(0,0);
}

bool driveForward(int motor_power)
{
	setMotors(motor_power, motor_power);
	wait1Msec(300);

	while(SensorValue[S2] != (int)colorRed)
	{
		if ((SensorValue[S2] != (int)colorBlack) && (SensorValue[S2] != (int)colorRed)) {
			followPath();
			setMotors(motor_power, motor_power);
			wait1Msec(100);
		}
		if (SensorValue[S3] < 10) {
			setMotors(0,0);
			return false;
		}
	}
	setMotors(0,0);
	return true;
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
	resetGyro(S4);

	if (angle>0){
		setMotors(25,-25);
	}else{
		setMotors(-25,25);
	}

	while (abs(SensorValue[S4])<abs(angle)) {}

 	setMotors(0,0);
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
 * driveToTable()
 *
 * Drives the robot forwards and turns towards to nearest table so that it
 * is ready to drop off the plate
 *
 * @param motor_power The motor power that the robot moves at.
 */
void driveToTable (int motor_power)
{
	setMotors(motor_power, motor_power);

	driveForward(STANDARD_M_POWER);
	wait1Msec(200);
	setMotors(0,0);

	turn(-90);

	setMotors(motor_power, motor_power);

	wait1Msec(400);

	setMotors(0,0);
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
 * driveToLocation()
 *
 * Drives the robot to the actual given location of the table
 * location with various helper functions.
 *
 * @param target The x and y coordinate of the table in the restaurant grid
 */
void driveToLocation (Location &target)
{
	//leaveKitchen();

	for (int count = 0; count < (target.row); count++) {
		driveForward(STANDARD_M_POWER);
	}
	setMotors(STANDARD_M_POWER,STANDARD_M_POWER);
	wait1Msec(300);
	turn(90);
	for (int count = 0; count < (target.col); count++) {
		driveForward(STANDARD_M_POWER);
	}

	driveToTable(STANDARD_M_POWER);
}

/*
 * driveHome()
 *
 * Returns the robot home back to the kitchen after it has completed
 * delivering the food or bill. Assume no obstacles on way home.
 *
 * @param target The x and y coordinate of the table in the restaurant grid
 */
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

/*
 * waitButton()
 *
 * Waits for the specified button to be pressed
 *
 * @param button The type of button that is waited for
 */
void waitButton(TEV3Buttons button)
{
	while(!getButtonPress(button)) {}
	while(getButtonPress(button)) {}
}

//comment later
void clearScreen() {
	for (int row = 1; row < 15; row++) {
		displayString(row, "                                        ");
	}
}

/*
 * tableChoice()
 *
 * Prompts the user for a table choice (which table to serve to)
 *
 * @return an integer value which represents the table number chosen.
 */
int tableChoice()
{
	clearScreen();
	displayBigStringAt(NUM_X_POS-20, NUM_Y_POS+30, "TABLE NUMBER:" );
	int table_num = 1;
	while(!getButtonPress(buttonEnter)) {
		if (getButtonPress(buttonUp) && table_num < 6) {
			table_num++;
			wait1Msec(300);
		}
		else if (getButtonPress(buttonDown) && table_num > 0) {
			table_num--;
			wait1Msec(300);
		}
		displayBigStringAt(NUM_X_POS, NUM_Y_POS, "%d", table_num);
	}
	clearScreen();
	return table_num;
}

void moveClaw(bool open) //true for open, false for close
{
	const int ENC_LIMIT = 620;
	nMotorEncoder[motorB] = 0;

	if (open) {
		motor[motorB] = 25;
	} else {
		motor[motorB] = -25;
	}
	while (abs(nMotorEncoder[motorB]) < abs(ENC_LIMIT))
	{}
	motor[motorB] = 0;
}

bool foodOrBill()
{
	clearScreen();
	displayBigStringAt(0, NUM_Y_POS+30, "<FOOD OR BILL?> " );

	while(true) {
		if (getButtonPress(buttonLeft)) {
			clearScreen();
			displayBigStringAt(0, NUM_Y_POS+30, "DELIVERING FOOD!" );
			wait1Msec(2000);
			clearScreen();
			return true;
		}
		else if (getButtonPress(buttonRight)) {
			clearScreen();
			displayBigStringAt(0, NUM_Y_POS+30, "SENDING BILL" );
			wait1Msec(2000);
			clearScreen();
			return false;
		}
	}
	return false;
}

void getPayment(int table_num) {
	displayBigStringAt(0, 75, "PLEASE PAY BILL");

	displayBigStringAt(50, 50, "$%1.2f", bill[table_num-1]*TAX);

	waitButton(buttonEnter);

	bill[table_num-1] = 0;
}

//comment later
void scanPlate (int table_num) {
	if (SensorValue[S1] == (int)colorBlue) {
		bill[table_num-1] += 8;
		displayString(10, "%d", bill[table_num-1]);
	} else if (SensorValue[S1] == (int)colorYellow) {
		bill[table_num-1] += 12;
	} else if (SensorValue[S1] == (int)colorRed) {
		bill[table_num-1] += 16;
	}
}

//implement later (checks if bills are all paid)
bool billsEmpty() {
	for (int table = 0; table < RESTAURANT_HEIGHT*RESTAURANT_WIDTH; table++) {
		if (bill[table] != 0) {
			return false;
		}
	}
	return true;
}

void collectBills() {

}




//*****************MAIN PROGRAM******************//
//TODO:
//Integrate bill collection system
//Be able to return home from any point.

task main(){
	configureAllSensors();

	bool in_use = true;
	//main loop
	displayBigStringAt(0, 100, "Welcome!");
	displayString(10, "The uWaiterlooBot is at your service!");

	wait1Msec(2000);
	clearScreen();

	while(in_use) {
		clearScreen();
		displayBigStringAt(0, 50, "Press enter:");

		//wait for input
		waitButton(buttonEnter);
		int table_num = 0;

		//get table input from user
		table_num	= tableChoice();
		wait1Msec(1000);

		if (table_num == 0) {
			in_use = false;
		}	else {
			//ask whether to deliver food or bill
			bool deliver_food;
			deliver_food = foodOrBill();

			if (deliver_food) {
				moveClaw(false);
				scanPlate(table_num);
			}

			//get to table
			Location target;
			getDriveLocation(table_num, target);
			driveToLocation(target);


			//check for obstacles along the way

			//deliver food or bill
			if (deliver_food) {
				wait1Msec(2000);
				moveClaw(true);
			} else {
				getPayment(table_num);
			}

			//driveHome(target);
		}
	}

	//shut down procedure
	if (!billsEmpty()) {
		displayString(3, "PLEEEEEAAAAASE PAY YOUR BILLS"); //insert bill collection system here
		wait1Msec(2000);
		//activate bill payment procedure
	} else {
		displayBigStringAt(0, 100, "THANK YOU FOR");
		displayBigStringAt(0, 75, "USING");
		displayBigStringAt(0, 50, "THE UWAITERLOO BOT");
		wait1Msec(2000);
	}
}
