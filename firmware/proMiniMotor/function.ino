

void driveMotor1(int out) {
    if (out > 0) {
        out = map(out, 0, 255, 255, 0);
        analogWrite(inverse_motor_direction[0] ? M1_E1 : M1_E2, out);
        analogWrite(inverse_motor_direction[0] ? M1_E2 : M1_E1, 255);
    }
    else {
        out = map(out, 0, -255, -255, 0);
        analogWrite(inverse_motor_direction[0] ? M1_E1 : M1_E2, 255);
        analogWrite(inverse_motor_direction[0] ? M1_E2 : M1_E1, abs(out));
    }
}

void driveMotor2(int out) {
    if (out > 0) {
        out = map(out, 0, 255, 255, 0);
        analogWrite(inverse_motor_direction[1] ? M2_E1 : M2_E2, out);
        analogWrite(inverse_motor_direction[1] ? M2_E2 : M2_E1, 255);
    }
    else {
        out = map(out, 0, -255, -255, 0);
        analogWrite(inverse_motor_direction[1] ? M2_E1 : M2_E2, 255);
        analogWrite(inverse_motor_direction[1] ? M2_E2 : M2_E1, abs(out));
    }
}

void driveMotor3(int out) {
    if (out > 0) {
        out = map(out, 0, 255, 255, 0);
        analogWrite(inverse_motor_direction[2] ? M3_E1 : M3_E2, out);
        analogWrite(inverse_motor_direction[2] ? M3_E2 : M3_E1, 255);
    }
    else {
        out = map(out, 0, -255, -255, 0);
        analogWrite(inverse_motor_direction[2] ? M3_E1 : M3_E2, 255);
        analogWrite(inverse_motor_direction[2] ? M3_E2 : M3_E1, abs(out));
    }
}

void stop(){
    walkControl(0,0);
}

void kick(int a){
    driveMotor3(a);
}

void walkControl(int left, int right){
    driveMotor1(left);
    driveMotor2(right);
}
