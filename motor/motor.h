/*
ArduBike motor library
Motors currently supported:
- Brushed DC motor with single PWM control (H-Bridge motor driver)
*/

#ifndef __ardubike_motor__
#define __ardubike_motor__

class Motor {
    int _mot_plus, _mot_min, _mot_pwm;
    int _mot_thr, _mot_thr_norm;
    public:
    Motor();
    Motor(int mot_plus, int mot_min, int mot_pwm);
    ~Motor();
    void attach();
    void write(int thr); // Writes motor throttle in scale of 0-100
};

#endif