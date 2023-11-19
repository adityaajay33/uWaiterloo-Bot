void configureAllSensors(){

    SensorType[S1] = SensorEV3_Touch;
    Wait1MSec(50);
    SensorType[S2] = SensorEV3_Ultrasonic;
    Wait1MSec(50);
    SensorType[S3] = SensorEV3_Color;
    Wait1MSec(50);
    SensorType[S3] = modeEV3Color_Color;
    Wait1MSec(50);
    SensorType[S4] = SensorEV3_Gyro;
    Wait1MSec(50);
    SensorType[S4] = modeEV3Gyro_Calibration;
    Wait1MSec(50);
    SensorType[S4] = modeEV3Gyro_RateAndAngle;
    Wait1MSec(50);
}

void drive_forward(bool driveDir, int mot_pow, float distance){

    //if driveDir is 1: its going forward. if its 0, its going backwards
    //motorA and motorC are the wheels

    float encoder_distance = distance / (2*M_PI*2.75) * 360;
    float initial_pos = nMotorEncoder[motorA];
    if (driveDir){

        motor[motorA]= motor[motorC] = mot_pow;
        while((encoder_distance+initial_pos)> nMotorEncoder[motorA]){}
    }
    else{

        motor[motorA] = motor[motorC] = -1*mot_pow;
        while((initial_pos-encoder_distance)< nMotorEncoder[motorA]){}
    }

    motor[motorA] = motor[motorC] = 0;
}

void turn(bool clockwiseDir, int degrees){

    int currentGyro = SensorValue[S4];
    if(clockwiseDir){
        motor[motorA] = 40;
        motor[motorC] = -40
        while(SensorValue[S4]<(currentGyro+degrees)){}
    }
    else{
        motor[motorA] = -40;
        motor[motorC] = 40
        while(SensorValue[S4]>(currentGyro-degrees)){}
    }
    motor[motorA] = motor[motorC] = 0;
}

void leaveHome(float dist){
    drive_forward(1, 40, dist);
    turn(1, 90);
}

void returnHome(float dist, int coord[2]){
    turn(0, 180);
    drive_forward(1, 40, dist*(coord[1]));
    turn(1, 90);
    drive_forward(1, 40, dist*(coord[0]));
    turn(0, 90);
    drive_forward(1, 40, dist);
    turn(0, 180);

}

void deliverFood(int del_arr[][2], float dist){
    for(int i = 0; i<sizeof(del_arr); i++){
        int sideways = del_arr[i][0];
        int vertical = del_arr[i][1];

        leaveHome(dist);
        drive_forward(1, 40, dist*sideways);
        turn(0, 90);
        drive_forward(1, 40, dist*vertical);

        returnHome(dist, {sideways, vertical});
    }
    
}

int main(){

    configureAllSensors();

    while(!getButtonPress(buttonEnter)){}
    while(getButtonPress(buttonEnter)){}

    leaveHome()

    //test case
    deliverFood({{2, 2}, {1, 2}, {0, 1}});
}