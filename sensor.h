/*
ArduBike sensor library
Sensors currently supported:
- Potentiometer, as throttle sensor
- More sensors soon
*/

#ifndef __ardubike_sensor__
#define __ardubike_sensor__

#include <Arduino.h>

class Potentiometer {
    int _pot_pin;
    int _pot_val, _pot_val_norm;
    
    public:
    void attach(int pot_pin);
    int read(); // Reads potentiometer analog value (0-1023)
};

#endif