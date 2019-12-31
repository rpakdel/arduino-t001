#pragma once

#include "motorpins.h"

class Motor
{
public:
	Motor(MotorPins motorA, MotorPins motorB);
	void setup();
	void move(int16_t motorASpeed, int16_t motorBSpeed);
private:
	MotorPins mA;
	MotorPins mB;
	void motorMove(MotorPins& motor, int16_t motorSpeed);
	int16_t clampMotorSpeed(int16_t motorSpeed);
};

