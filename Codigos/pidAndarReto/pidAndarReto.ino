#include <PID_v1.h>

int motorR[2] = { 8, 9 };
int motorL[2] = { 10, 11 };

//Define Variables we'll be connecting to
double SetpointRight, InputRight, OutputRight;
double SetpointLeft, InputLeft, OutputLeft;

//Specify the links and initial tuning parameters
PID rightPID(&InputRight, &OutputRight, &SetpointRight, 10,0,1, DIRECT);
PID leftPID(&InputLeft, &OutputLeft, &SetpointLeft, 10,0,1, DIRECT);

int encoderRPin = 2;
int encoderLPin = 3;
int countR = 0;
int countL = 0;
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
  SetpointRight = 60;
  SetpointLeft = 60;
  
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
  
  int rpmR = 0;
  int rpmL = 0;
  
  int time = millis();
  
  if( time % 1000 == 0 )
  {
    wasUpdateRpm = false;
  }
  
  if( time % 1001 == 0 && !wasUpdateRpm )
  {    
    rpmR = countR ;// 40;
    rpmL = countL ;// 40;
    
//    Serial.print( "rpmR : " );
//    Serial.println( rpmR );
//    Serial.print( "rpmL : " );
//    Serial.println( rpmL );
    
    Serial.println( rpmR - rpmL );
    
    countR = 0;
    countL = 0;
    
    wasUpdateRpm = true;
  }
  
  InputRight = rpmR;
  InputLeft = rpmL;
  
//  Serial.print( "Left: " );
//  Serial.println( rpmL );
//  Serial.print( "Right: " );
//  Serial.println( rpmR );
  
  rightPID.Compute();
  leftPID.Compute();
  
//  Serial.print( "RightOutput: " );
//  Serial.println( OutputRight );
//  Serial.print( "LeftOutput: " );
//  Serial.println( OutputLeft );
  
  //OutputRight = 128;
  //OutputLeft = 105;
  
  analogWrite( motorR[0], OutputRight );
  analogWrite( motorR[1], 0 );
  analogWrite( motorL[0], OutputLeft );
  analogWrite( motorL[1], 0 );
}

