#include <Arduino.h>
#include <IRremote.hpp>

#define IR_RECEIVE_PIN 12

int leds[] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
int count = 10;
String number;
int i = 0;

unsigned long codes[] = {
    3910598400,
    4077715200,
    3877175040,
    2707357440,
    4144561920,
    3810328320,
    2774204160,
    3175284480,
    2907897600,
    3041591040};

void setup()
{
  for (int i = 0; i < count; i++)
  {
    pinMode(leds[i], OUTPUT);
  }

  IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK);
  Serial.begin(9600);
}

void off_leds()
{
  for (int i = 0; i < count; i++)
  {
    digitalWrite(leds[i], LOW);
  }
}

int position(unsigned long code)
{
  for (int i = 0; i < count; i++)
  {
    if (code == codes[i])
    {
      return i;
    }
  }
}

void loop()
{
  unsigned long enter = 3158572800;
  if (IrReceiver.decode())
  {
    unsigned long recieved_code = IrReceiver.decodedIRData.decodedRawData;
    if (recieved_code == enter)
    {
      off_leds();
      number = "";
    }
    else if (recieved_code != enter && number.toInt() < 1024)
    {
      for (int i = 0; i < count; i++)
      {
        if (recieved_code == codes[i])
        {
          number += position(recieved_code);
        }
      }
      Serial.println(number);
      for (unsigned int i = 0; i < count; i++)
      {
        digitalWrite(leds[i], bitRead(number.toInt(), 9 - i));
        // Serial.print(bitRead(number.toInt(), 9 - i));
      }
    }
    else{
      Serial.println("Number is too big, press enter!");
    }
    if (recieved_code != 0)
    {
      Serial.println(recieved_code);
    }

    IrReceiver.resume();
  }
}