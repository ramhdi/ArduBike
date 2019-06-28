#include "sensor.h"

Sensor::Sensor(int pot_pin) {
    _pot_pin = pot_pin;
}

int Sensor::readPotentio() {
    _pot_val = analogRead(_pot_pin);
    _pot_val_norm = map(_pot_val, 0, 1023, 0, 100); // normalises potentiometer reading to 0-100 scale
    return analogRead(_pot_val_norm);
}