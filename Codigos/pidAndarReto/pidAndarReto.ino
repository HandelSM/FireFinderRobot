#include <PID_v1.h>

int motorR[2] = { 8, 9 };
int motorL[2] = { 10, 11 };

//Define Variables we'll be connecting to
double SetpointLeft, InputLeft, OutputLeft;
double SetpointRight, InputRight, OutputRight;

//Specify the links and initial tuning parameters
PID leftPID(&InputLeft, &OutputLeft, &SetpointLeft,2,0,1, DIRECT);
PID rightPID(&InputRight, &OutputRight, &SetpointRight,2,0,1, DIRECT);

int encoderRPin = 2;
int encoderLPin = 3;
int countR = 0;
int countL = 0;
int currentEncoderRight = LOW;
int lastEncoderRight = LOW;
int currentEncoderLeft = LOW;
int lastEncoderLeft = LOW;

void setup ()
{
  //turn the PID on
  leftPID.SetMode(AUTOMATIC);
  pinMode( encoderRPin, INPUT );
  pinMode( encoderLPin, INPUT );
  Serial.begin(9600);
}

void loop()
{
  currentEncoderRight = digitalRead( encoderRPin );
  currentEncoderLeft = digitalRead( encoderLPin );
  
  if ( currentEncoderRight != lastEncoderRight )
  {
    countR++;
    Serial.print("Count 1: ");
    Serial.println( countR );
  }
  
  if ( currentEncoderLeft != lastEncoderLeft )
  {
    countL++;
    Serial.print("Count 2: ");
    Serial.println( countL );
  }
  lastEncoderRight = currentEncoderRight;
  lastEncoderLeft = currentEncoderLeft;
  
  int error = countR - countL;
  SetpointLeft = error;
  SetpointRight = error;
  InputLeft = countL;
  InputRight = countR;
  leftPID.Compute();
  analogWrite( motorR[0], OutputRight );
  analogWrite( motorR[1], 0 );
  analogWrite( motorL[0], OutputLeft );
  analogWrite( motorR[1], 0 );
}
