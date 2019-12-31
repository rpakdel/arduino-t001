#include "motor.h"


Motor::Motor(MotorPins motorA, MotorPins motorB) :
	mA(motorA), mB(motorB)
{
	
}

void Motor::setup()
{
	pinMode(mA.dir1Pin, OUTPUT);
	pinMode(mA.dir2Pin, OUTPUT);
	pinMode(mA.speedPin, OUTPUT);
	pinMode(mB.dir1Pin, OUTPUT);
	pinMode(mB.dir2Pin, OUTPUT);
	pinMode(mB.speedPin, OUTPUT);
}

void Motor::move(int16_t motorASpeed, int16_t motorBSpeed)
{
	motorMove(mA, motorASpeed);
	motorMove(mB, motorBSpeed);
}

void Motor::motorMove(MotorPins& motor, int16_t motorSpeed)
{
	motorSpeed = clampMotorSpeed(motorSpeed);

	if (motorSpeed > 0)
	{
		// forward
		digitalWrite(motor.dir1Pin, LOW);
		digitalWrite(motor.dir2Pin, HIGH);

		analogWrite(motor.speedPin, motorSpeed);
	}
	else if (motorSpeed < 0)
	{
		// reverse
		digitalWrite(motor.dir1Pin, HIGH);
		digitalWrite(motor.dir2Pin, LOW);

		analogWrite(motor.speedPin, -motorSpeed);
	}
	else
	{
		// stop
		digitalWrite(motor.dir1Pin, LOW);
		digitalWrite(motor.dir2Pin, LOW);

		analogWrite(motor.speedPin, 0);
	}
}

int16_t Motor::clampMotorSpeed(int16_t motorSpeed)
{
	if (motorSpeed > 255)
	{
		motorSpeed = 255;
	}

	if (motorSpeed < -255)
	{
		motorSpeed = -255;
	}

	return motorSpeed;
}
