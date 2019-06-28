/*
ArduBike - Open Source Electric Bike Platform
Developed by Rama Rahardi - rama.orion@gmail.com
28/06/19
*/

#include "ArduBike.h"

// Sensor declaration
#define FREQ_SENSOR_READ 10 // Hz
unsigned long sensorPrevMillis = 0; // ms
const int pot_pin = PA0;
int pot_val;
Sensor sensor(pot_pin);

// Motor declaration
#define FREQ_MOTOR_WRITE 10 // Hz
unsigned long motorPrevMillis = 0; // ms
const int mot_pin_plus = PB12;
const int mot_pin_min = PB13;
const int mot_pin_pwm = PB14;
int mot_thr;
Motor motor(mot_pin_plus, mot_pin_min, mot_pin_pwm);

unsigned long currentMillis;
void setup() {
    motor.attach();
}

void loop() {
    currentMillis = millis();

    // Task: Sensor read
    if ((currentMillis - sensorPrevMillis) >= 1000 / FREQ_SENSOR_READ) {
        sensorPrevMillis = currentMillis;

        pot_val = sensor.readPotentio();
    }

    // Task: Motor write
    if ((currentMillis - motorPrevMillis) >= 1000 / FREQ_MOTOR_WRITE) {
        motorPrevMillis = currentMillis;

        mot_thr = pot_val;
        motor.write(mot_thr);
    }
}