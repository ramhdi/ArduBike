#include "motor.h"

Motor::Motor(int mot_plus, int mot_min, int mot_pwm) {
    _mot_plus = mot_plus;
    _mot_min = mot_min;
    _mot_pwm = mot_pwm;
}

void Motor::attach() {
    pinMode(_mot_plus, OUTPUT);
    pinMode(_mot_min, OUTPUT);
    pinMode(_mot_pwm, OUTPUT);

    digitalWrite(_mot_plus, HIGH);
    digitalWrite(_mot_min, LOW);
}

void Motor::write(int thr) {
    _mot_thr_norm = thr;
    _mot_thr = map(_mot_thr_norm, 0, 100, 0, 255);
    analogWrite(_mot_pwm, _mot_thr);
}