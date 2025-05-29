#ifndef TOUCHCONTROLLER_H
#define TOUCHCONTROLLER_H

class TouchController {
private:
    int touchPin0, touchPin1, touchPin2;

public:
    TouchController(int pin0, int pin1, int pin2);
    void begin();
    int checkTouch();
};

#endif
