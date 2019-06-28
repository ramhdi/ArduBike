#include "sensor.h"

void Potentiometer::attach(int pot_pin) {
    _pot_pin = pot_pin;
}

int Potentiometer::read() {
    _pot_val = analogRead(_pot_pin);
    _pot_val_norm = map(_pot_val, 0, 4095, 0, 100); // normalises potentiometer reading to 0-100 scale
    return _pot_val_norm;
}