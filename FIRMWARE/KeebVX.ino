#include <Keyboard.h>
#include <Mouse.h>
#define ENCODER_OPTIMIZE_INTERRUPTS
#include <Encoder.h>

#define DELAY 0  //rest time between loops in ms (1ms = 1000hz polling rate)
enum PinAssignments
{
encoderPinA = 0,
encoderPinB = 1,
encoderPinC = 2,
encoderPinD = 3,
};

typedef struct { //like a dictionary array: Type  variableName
  Encoder enc;
  char axis;
} encoder_t; //Structure name

#define ENCODER_COUNT 2
#define ENCODER_SENSITIVITY 3

encoder_t encoders[ENCODER_COUNT] = {
  {Encoder(encoderPinA, encoderPinB), 'x'},
  {Encoder(encoderPinC, encoderPinD), 'y'}
};

typedef struct {
  byte switchPin;
  char key;
} switch_t;

#define SWITCH_COUNT 9

switch_t switches[SWITCH_COUNT] = {
  {A0, 'd'},
  {A1, 'f'},
  {A2, 'j'},
  {A3, 'k'},
  {5, 'c'},
  {6, 'm'},
  {4, '1'},
  {14, 's'},
  {15, 'l'},
  //{9, 'KEY_ESC'},
  //{10, 'KEY_ENTER'},
};


void setup() {
  // put your setup code here, to run once:
  for (int i=0; i < SWITCH_COUNT; i++) {
    pinMode(switches[i].switchPin, INPUT_PULLUP);
  }
 Serial.begin(230400);
}

int newEncDirection = 0;
int oldEncDirection = 0;

void loop() {
  long oldEncVal = 0;
  int newEncDirection = 0;
  int oldEncDirection = 0;
  for(int rest=0; rest < 100; rest++) { //for loop to reset encVal after a pause
    for(int i=0; i < ENCODER_COUNT;i++) {
      volatile float newEncVal = encoders[i].enc.read();
      if (newEncVal != 0){
        if (newEncVal < 0){newEncDirection = -1;}else {newEncDirection = 1;};
        if (oldEncVal < 0){oldEncDirection = -1;}else {oldEncDirection = 1;};
        if((abs(newEncVal) >= 10) or (((abs(newEncVal - oldEncVal)) >= 4) and (newEncDirection != oldEncDirection)) or ((abs(newEncVal - oldEncVal)) >= 6)) { //assume error
          //newEncVal = 0;
          //newEncVal = oldEncDirection*0.1;
          newEncVal = newEncDirection*0.1;
        }
        else {oldEncVal = newEncVal;};
        if(encoders[i].axis == 'x')
          Mouse.move(ENCODER_SENSITIVITY*newEncVal,0,0);
        if(encoders[i].axis == 'y')
          Mouse.move(0,-ENCODER_SENSITIVITY*newEncVal,0);
        encoders[i].enc.write(0);
        delay(DELAY);
        }else {rest++;}
      encoders[i].enc.write(0);
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
}
