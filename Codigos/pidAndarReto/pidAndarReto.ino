#include <PID_v1.h>

int motorR[2] = { 5, 6 };
int motorL[2] = { 9, 10 };

double SetpointRight, InputRight, OutputRight;
double SetpointLeft, InputLeft, OutputLeft;

PID rightPID(&InputRight, &OutputRight, &SetpointRight, 2, 5, 0, DIRECT);
PID leftPID(&InputLeft, &OutputLeft, &SetpointLeft, 2, 5, 0, DIRECT);

int encoderRPin = 2;
int encoderLPin = 3;
int countR = 0;
int countL = 0;
int rpmR = 0;
int rpmL = 0;
int currentEncoderRight = LOW;
int lastEncoderRight = LOW;
int currentEncoderLeft = LOW;
int lastEncoderLeft = LOW;
boolean wasUpdateRpm = false;

void encoder()
{
  currentEncoderRight = digitalRead( encoderRPin );
  currentEncoderLeft = digitalRead( encoderLPin );
  
  if ( currentEncoderRight != lastEncoderRight )
  {
    countR++;
//    Serial.print("Count R: ");
//    Serial.println( countR );
  }
  
  if ( currentEncoderLeft != lastEncoderLeft )
  {
    countL++;
//    Serial.print("Count L: ");
//    Serial.println( countL );
  }
  lastEncoderRight = currentEncoderRight;
  lastEncoderLeft = currentEncoderLeft;
}

void setup ()
{
  SetpointRight = 30;
  SetpointLeft = 30;
  
  rightPID.SetMode(AUTOMATIC);
  leftPID.SetMode(AUTOMATIC);
  
  pinMode( encoderRPin, INPUT );
  pinMode( encoderLPin, INPUT );
  
  pinMode( motorR[0], OUTPUT );
  pinMode( motorR[1], OUTPUT );
  pinMode( motorL[0], OUTPUT );
  pinMode( motorL[1], OUTPUT );
  
  Serial.begin(9600);
}

void loop()
{
  encoder();
  
  int time = millis();
  
  if( time % 500 == 0 )
  {
    wasUpdateRpm = false;
  }
  
  if( time % 501 == 0 && !wasUpdateRpm )
  {    
    rpmR = countR;
    rpmL = countL;
    
    Serial.print( "rpmR : " );
    Serial.println( rpmR );
    Serial.print( "rpmL : " );
    Serial.println( rpmL );
    
    countR = 0;
    countL = 0;
    
    Serial.println( OutputRight );
    Serial.println( OutputLeft );
    
    wasUpdateRpm = true;
  }
  
  InputRight = rpmR;
  InputLeft = rpmL;
  
  rightPID.Compute();
  leftPID.Compute();
  
  analogWrite( motorR[0], OutputRight );
  analogWrite( motorR[1], 0 );
  analogWrite( motorL[0], OutputLeft );
  analogWrite( motorL[1], 0 );
}

