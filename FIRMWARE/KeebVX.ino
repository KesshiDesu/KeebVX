#include <Keyboard.h>
#include <Mouse.h>
#include <Encoder.h>

#define DELAY 1  //rest time between loops in ms (1ms = 1000hz polling rate)
enum PinAssignments
{
encoderPinA = 0,
encoderPinB = 1,
encoderPinC = 3,
encoderPinD = 2,
};

typedef struct { //like a dictionary array: Type  variableName
  Encoder enc;
  char axis;
} encoder_t; //Structure name

#define ENCODER_COUNT 2
#define ENCODER_SENSITIVITY 9

encoder_t encoders[ENCODER_COUNT] = {
  {Encoder(encoderPinA, encoderPinB), 'x'},
  {Encoder(encoderPinC, encoderPinD), 'y'}
};

typedef struct {
  byte switchPin;
  char key;
} switch_t;

#define SWITCH_COUNT 6

switch_t switches[SWITCH_COUNT] = {
  {A0, 'd'},
  {A1, 'f'},
  {A2, 'j'},
  {A3, 'k'},
  {A4, 'c'},
  {A5, 'm'},
  //{4, ''},
  //{5, ''},
  //{6, ''},
};


void setup() {
  // put your setup code here, to run once:
  for (int i=0; i < SWITCH_COUNT; i++) {
    pinMode(switches[i].switchPin, INPUT_PULLUP);
  }
  //Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  for(int i=0; i < ENCODER_COUNT;i++) {
    long encVal = encoders[i].enc.read();
    if (encVal != 0) {
      if(encoders[i].axis == 'x')
        Mouse.move(ENCODER_SENSITIVITY*encVal,0,0);
      else
        Mouse.move(0,ENCODER_SENSITIVITY*encVal,0);
      encoders[i].enc.write(0);
    }
  }
  Keyboard.begin();
  for (int i=0; i < SWITCH_COUNT; i++) {
    if (digitalRead(switches[i].switchPin) == 0)
    {
      Keyboard.press(switches[i].key);
    } else {
      Keyboard.release(switches[i].key);
    }
  }
  Keyboard.end();
  delay(DELAY);
}
