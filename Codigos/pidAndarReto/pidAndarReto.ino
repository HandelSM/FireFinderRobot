#include <PID_v1.h>

typedef struct motor
{
  byte vel;
  byte way;
};
typedef struct motor Motor;

Motor rightM = { 6, 7 };
Motor leftM = { 5, 4 };

double SetpointRight, InputRight, OutputRight;
double SetpointLeft, InputLeft, OutputLeft;

PID rightPID(&InputRight, &OutputRight, &SetpointRight, 2, 4, 0, DIRECT);
PID leftPID(&InputLeft, &OutputLeft, &SetpointLeft, 2, 4, 0, DIRECT);

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

unsigned long old = 0;

void encoder()
{
  //delay(5);
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
  SetpointRight = 20;
  SetpointLeft = 20;
  
  rightPID.SetMode(AUTOMATIC);
  leftPID.SetMode(AUTOMATIC);
  
  pinMode( encoderRPin, INPUT );
  pinMode( encoderLPin, INPUT );
  
  pinMode( rightM.way, OUTPUT );
  pinMode( leftM.way, OUTPUT );

  
  Serial.begin(9600);
}

void loop()
{
  encoder();
  
  unsigned long now = millis();
  
  if( now - old >= 250 )
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
    
    //wasUpdateRpm = true;
    
    old = now;
  }
  
  InputRight = rpmR;
  InputLeft = rpmL;
  
  rightPID.Compute();
  leftPID.Compute();
  
  digitalWrite( rightM.way, HIGH );
  digitalWrite( leftM.way, HIGH );
  analogWrite( rightM.vel, OutputRight );
  analogWrite( leftM.vel, OutputLeft );
}

