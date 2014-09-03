#include <PID_v1.h>

typedef struct motor
{
  byte vel;
  byte way;
};
typedef struct motor Motor;

Motor rightM = { 6, 7 }; //Slave
Motor leftM = { 5, 4 }; //Master

double SetpointRight, InputRight, OutputRight;

PID rightPID(&InputRight, &OutputRight, &SetpointRight, 1, 2, 0, DIRECT);

//ENCODER
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
  currentEncoderRight = digitalRead( encoderRPin );
  currentEncoderLeft = digitalRead( encoderLPin );
  
  if ( currentEncoderRight != lastEncoderRight )
  {
    countR++;
  }
  
  if ( currentEncoderLeft != lastEncoderLeft )
  {
    countL++;
  }
  lastEncoderRight = currentEncoderRight;
  lastEncoderLeft = currentEncoderLeft;
}

void setup ()
{  
  SetpointRight = 170;
  
  OutputRight = 150;
  
  rightPID.SetMode(AUTOMATIC);
  //leftPID.SetMode(AUTOMATIC);
  
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
  
  if( now - old >= 500 )
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
    old = now;
  }
  
  InputRight = rpmR;
  SetpointRight = rpmL;
  
  rightPID.Compute();
  
  digitalWrite( rightM.way, HIGH );
  digitalWrite( leftM.way, HIGH );
  analogWrite( rightM.vel, OutputRight );
  analogWrite( leftM.vel, 193 );
}
