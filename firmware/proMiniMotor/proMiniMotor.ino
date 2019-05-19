/*
This program is writed by LOK CHON MOU in 2019-03

Pins used:
Digital inputs 5 & 6 are connected to A4950 motor drive as PWM input
Digital inputs 9 & 10 are connected to A4950 motor drive as PWM input
Digital inputs 11 & 3 are connected to A4950 motor drive as PWM input
For PS2 remote : D2, D4, D7 and D8 is connected

*/
//#############################
#include <PS2X_lib.h>
//#############################
#define M1_E1 5
#define M1_E2 6

#define M2_E1 9
#define M2_E2 10

#define M3_E1 11
#define M3_E2 3
// for PS2 remote, the pin order
//is data, command, Vmotor, GND, 3V3, Attention, Clock, NC, Acknowledge
#define DATA 2
#define COMMAND 4
#define ATTENTION 7
#define CLOCK 8

#define ledPin 13       //state led in D13, use for debug

#define maximum_speed_boosters   255.0    //turbo_maximum speed
#define maximum_speed   170.0      //common_maximum speed


//#############################
PS2X ps2x;
int error = 99;
byte type = 99;
byte vibrate = 0;

float joystick_X, joystick_Y;
float speed_change;

boolean inverse_motor_direction[]={false, false, false};

unsigned long timer; // timer 0 is adjusted, 64 = 1ms

void setup() {

    pinMode(M1_E1, OUTPUT);
    pinMode(M1_E2, OUTPUT);

    pinMode(M2_E1, OUTPUT);
    pinMode(M2_E2, OUTPUT);

    pinMode(M3_E1, OUTPUT);
    pinMode(M3_E2, OUTPUT);

    pinMode(ledPin, OUTPUT);
    // set the pin 5, 6 pwm frequency, ATMEGA 328 is timer 0
    // set the pin 9, 10 pwm frequency, ATMEGA 328 is timer 1
    // set the pin 11, 3 pwm frequency, ATMEGA 328 is timer 2
    //TCCR0B = TCCR0B & 0b11111000 | 1;
    //TCCR1B = TCCR1B & 0b11111000 | 1;
    //TCCR2B = TCCR2B & 0b11111000 | 1;

    Serial.begin(115200);

    delay(300);
    error = ps2x.config_gamepad(CLOCK, COMMAND, ATTENTION, DATA, true, true);
    type = ps2x.readType();
    speed_change = maximum_speed;
}

void loop() {

    if (millis() - timer >= 50) {
        timer = millis();

        if (error != 0 || type != 1) {
            digitalWrite(ledPin, LOW);
            error = ps2x.config_gamepad(CLOCK, COMMAND, ATTENTION, DATA, true, true);
            type = ps2x.readType();

        }
        else {
            if (type == 1)  {
                digitalWrite(ledPin, HIGH);
                ps2x.read_gamepad(false, vibrate);

                //turbo_mode-----------------------------
                if(ps2x.Button(PSB_R2)) speed_change = maximum_speed_boosters;
                else speed_change = maximum_speed;

                //---------------------------------------
                joystick_X = map(ps2x.Analog(PSS_LX),0.0,255.0,speed_change,-speed_change);
                joystick_Y = map(ps2x.Analog(PSS_LY),0.0,255.0,speed_change,-speed_change);

                if(ps2x.Button(PSB_PAD_UP)) {joystick_Y=speed_change; Serial.println("UP");}
                if(ps2x.Button(PSB_PAD_DOWN)) {joystick_Y=-speed_change; Serial.println("DOWN");}
                if(ps2x.Button(PSB_PAD_LEFT)) {joystick_X= speed_change; Serial.println("LEFT");}
                if(ps2x.Button(PSB_PAD_RIGHT)) {joystick_X=-speed_change; Serial.println("RIGHT");}

                if (abs(joystick_X)<=5.0) joystick_X=0.0;
                if (abs(joystick_Y)<=5.0) joystick_Y=0.0;
                float v=(speed_change-abs(joystick_X))*(joystick_Y/speed_change)+joystick_Y;
                float w=(speed_change-abs(joystick_Y))*(joystick_X/speed_change)+joystick_X;

                float output_R=(v+w)/2.0;
                float output_L=(v-w)/2.0;
                Serial.print(output_L);
                Serial.print("    ");
                Serial.println(output_R);

                if(ps2x.Button(PSB_CIRCLE) || ps2x.Button(PSB_SQUARE)) {kick(100);}
                else {kick(0);}

                // control motor---------------------------
                walkControl(output_L, output_R);
            }
        }
    }
}
