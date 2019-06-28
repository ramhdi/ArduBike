/*
ArduBike - Open Source Electric Bike Platform
Developed by Rama Rahardi - rama.orion@gmail.com
28/06/19
*/

#include "ArduBike.h"

// Sensor declaration
#define FREQ_SENSOR_READ 13 // Hz
unsigned long sensorPrevMillis = 0; // ms
const int pot_pin = PA1;
int pot_val;
Potentiometer pot;

// Motor declaration
#define FREQ_MOTOR_WRITE 23 // Hz
unsigned long motorPrevMillis = 0; // ms
const int mot_pin_plus = PB12;
const int mot_pin_min = PB13;
const int mot_pin_pwm = PA8;
int mot_thr;
Motor motor;

unsigned long currentMillis;

void setup() {
    Serial.begin(9600);
    pinMode(mot_pin_pwm, OUTPUT);
    pot.attach(pot_pin);
    motor.attach(mot_pin_plus, mot_pin_min, mot_pin_pwm);
}

void loop() {
    currentMillis = millis();
    //Serial.println(currentMillis);

    // Task: Sensor read
    if ((currentMillis - sensorPrevMillis) >= 1000 / FREQ_SENSOR_READ) {
        sensorPrevMillis = currentMillis;

        pot_val = pot.read();
        Serial.print("["); Serial.print(currentMillis); Serial.print("] ");
        Serial.print("pot_val = "); Serial.println(pot_val);
    }

    // Task: Motor write
    if ((currentMillis - motorPrevMillis) >= 1000 / FREQ_MOTOR_WRITE) {
        motorPrevMillis = currentMillis;

        mot_thr = pot_val;
        motor.write(mot_thr);
        Serial.print("["); Serial.print(currentMillis); Serial.print("] ");
        Serial.print("mot_thr = "); Serial.println(mot_thr);
    }
}