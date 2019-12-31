#pragma once

#include <Arduino.h>

struct MotorPins
{
	uint8_t dir1Pin;
	uint8_t dir2Pin;
	// Needs to be a PWM pin to be able to control motor speed
	int16_t speedPin;
};