 /*
* Getting Started example sketch for nRF24L01+ radios
* This is a very basic example of how to send data from one node to another
* Updated: Dec 2014 by TMRh20
*/

#include <Wire.h>

#include <RF24_config.h>
#include <printf.h>
#include <nRF24L01.h>
#include <SPI.h>
#include <SoftwareSerial.h>
#include "RF24.h"
#include "joydata.h"

#define XAXIS_ANALOG_PIN A1
#define YAXIS_ANALOG_PIN A0
#define SW_ANALOG_PIN A2

int16_t XAXIS_RANGE[3] = { 23, 519, 1011 }; // min, mid, max
int16_t YAXIS_RANGE[3] = { 22, 522, 1011 }; // min, mid, max

RF24 radio(9, 10);
byte addresses[][6] = { "1Node", "2Node" };

void setupRadio()
{
	radio.begin();
	radio.setRetries(0, 15);
	radio.setPALevel(RF24_PA_HIGH);

	radio.openWritingPipe(addresses[1]);
	radio.stopListening();
}

void setup() 
{
    Serial.begin(115200);
	setupRadio();

    Serial.println(F("SENDER"));    
}

void loop()
{
    JoyData joyData;
	// this configuration is for the joystick attached to the breadboard
    joyData.X = - analogRead(XAXIS_ANALOG_PIN) + XAXIS_RANGE[1];
    joyData.Y = - analogRead(YAXIS_ANALOG_PIN) + YAXIS_RANGE[1];
    joyData.Button = 0;
    joyData.Id = 0;

	clampJoyDataXY(joyData, -512, 512);
	zeroDeadZoneJoyData(joyData, 10, 1);


    printJoyData(joyData, Serial);
    Serial.print(F("..."));

    if (!radio.write(&joyData, sizeof(joyData)))
    {
        Serial.println(F("NOK"));
    }
    else
    {
        Serial.println(F("OK"));
    }
}
