#include <Ultrasonic.h>
#include <PID_v1.h>

#define TRIGGER_PIN  12
#define ECHO_PIN     13

int motorR[2] = { 5, 6 };
int motorL[2] = { 9, 10 };

Ultrasonic ultrasonic( TRIGGER_PIN, ECHO_PIN );

double SetpointRight, InputRight, OutputRight;
double SetpointLeft, InputLeft, OutputLeft;

PID rightPID(&InputRight, &OutputRight, &SetpointRight, 0.1, 3, 0, DIRECT);
PID leftPID(&InputLeft, &OutputLeft, &SetpointLeft, 0.1, 3, 0, DIRECT);

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
boolean turn = false;

float distance()
{
  float cmFromWall;
  long microsec = ultrasonic.timing();
  cmFromWall = ultrasonic.convert( microsec, Ultrasonic::CM );
  return cmFromWall;
}

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
  SetpointRight = 40;
  SetpointLeft = 40;
  
  rightPID.SetMode(AUTOMATIC);
  leftPID.SetMode(AUTOMATIC);
  
  pinMode( TRIGGER_PIN, OUTPUT );
  pinMode( ECHO_PIN, INPUT );
  
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
  if( distance() > 10 && turn == false )
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
  
  else
  {
    if( turn == false )
    {
      countR = 0;
      countL = 0;
      
      analogWrite( motorR[0], 0 );
      analogWrite( motorR[1], 0 );
      analogWrite( motorL[0], 0 );
      analogWrite( motorL[1], 0 );
      
      turn = true;
    }
    
    encoder();
    
    if( countL < 40 )
    {
      analogWrite( motorR[0], 0 );
      analogWrite( motorR[1], 0 );
      analogWrite( motorL[0], 255 );
      analogWrite( motorL[1], 0 );
    }
    
    else
    {
      turn = false; 
    }
  }
}
