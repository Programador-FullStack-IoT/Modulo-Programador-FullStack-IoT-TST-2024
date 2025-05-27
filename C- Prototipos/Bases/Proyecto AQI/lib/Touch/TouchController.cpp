#include "TouchController.h"
#include <Arduino.h>

TouchController::TouchController(int pin0, int pin1, int pin2)
    : touchPin0(pin0), touchPin1(pin1), touchPin2(pin2) {}

void TouchController::begin() {
    // No se necesita inicialización para TOUCH
}

int TouchController::checkTouch() {
    if (touchRead(touchPin0) < 30) return 1; // Next
    if (touchRead(touchPin1) < 30) return 2; // Previous
    return 0; // No touch detected
}
