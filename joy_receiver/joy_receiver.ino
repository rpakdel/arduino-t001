/*
* Motor B (right when looking from front)
* This is a very basic example of how to send data from one node to another
* Updated: Dec 2014 by TMRh20
*/

#include <Wire.h>
#include <SPI.h>
#include <RF24_config.h>
#include <printf.h>
#include <nRF24L01.h>

#include "RF24.h"

#include "joydata.h"
#include "motor.h"
#include "motorpins.h"

RF24 radio(9, 10);
byte addresses[][6] = { "1Node", "2Node" };

// Motor A is left when facing the front of the robot
MotorPins motorA = { 7, 8, 6 };
// Motor B is right when facing the front of the robot
MotorPins motorB = { 3, 4, 5 };

Motor motor(motorA, motorB);

void setupRadio()
{
	radio.begin();
	radio.setPALevel(RF24_PA_HIGH);
	radio.setAutoAck(1);
	radio.setRetries(0, 15);
	radio.openReadingPipe(0, addresses[1]);
	radio.startListening();
}

void setup()
{
	motor.setup();
	setupRadio();
    Serial.begin(115200);
    Serial.println(F("T001"));
}

void handleJoyData(JoyData& joyData)
{
	int16_t motorBSpeed = 0;
	int16_t motorASpeed = 0;

	if (joyData.Y > 0)
	{
		// forward
		motorBSpeed = joyData.Y;
		motorASpeed = joyData.Y;
		
		if (joyData.X < 0)
		{
			// forward right
			motorBSpeed = joyData.Y + joyData.X;
		}
		else if (joyData.X > 0)
		{
			// forward left
			motorASpeed = joyData.Y - joyData.X;
		}
	}
	else if (joyData.Y < 0)
	{
		// reverse
		motorBSpeed = joyData.Y;
		motorASpeed = joyData.Y;

		if (joyData.X < 0)
		{
			// reverse right
			motorBSpeed = joyData.Y - joyData.X;
		}
		else if (joyData.X > 0)
		{
			// reverse left
			motorASpeed = joyData.Y + joyData.X;
		}
	}
	else if (joyData.X != 0)
	{
		// not moving forward or reverse, turning on point
		// condition below is just for clarification
		if (joyData.X < 0)
		{
			// spin left
			// motor B backward
			motorBSpeed = joyData.X;
			// motor A forward
			motorASpeed = -joyData.X;
		}
		else if (joyData.X > 0)
		{
			// spin right
			// motor B forward
			motorBSpeed = joyData.X;
			// motor A backward
			motorASpeed = -joyData.X;
		}
	}
	else
	{
		// stop both motors
	}

	motor.move(motorASpeed, motorBSpeed);

	Serial.print("B: ");
	Serial.print(motorBSpeed);
	Serial.print(" A: ");
	Serial.println(motorASpeed);
}
long prevMillis = 0;
void loop()
{
	long m = millis();
    if (radio.available())
    {
		//Serial.println(F("Got joy data"));
		prevMillis = m;

		JoyData joyData;
		radio.read(&joyData, sizeof(joyData));

		// motor values are 0-255
		joyData.X = joyData.X / 2;
		joyData.Y = joyData.Y / 2;

		//Serial.println(F("----------------"));
		//printlnJoyData(joyData, Serial);
		
		handleJoyData(joyData);
    }
    else
    {		
		long diff = m - prevMillis;
		if (diff > 3000)
		{
			prevMillis = m;
			Serial.println(F("No joy data after 3 seconds"));
		}
    }
}
