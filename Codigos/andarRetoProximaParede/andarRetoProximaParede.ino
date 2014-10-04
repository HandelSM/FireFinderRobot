//HCSR04 lib and pins
#include <NewPing.h>
#include <Servo.h>
#include <PID_v1.h>

#define TRIGGER_PIN_T  12
#define ECHO_PIN_T     13

#define TRIGGER_PIN_F  10
#define ECHO_PIN_F     11

#define MAX_DISTANCE 150

#define SERVO 6

NewPing frontSensor(TRIGGER_PIN_F, ECHO_PIN_F, MAX_DISTANCE);
NewPing turnSensor(TRIGGER_PIN_T, ECHO_PIN_T, MAX_DISTANCE);

float gapF = 0;
float gapT = 0;
const byte tolerance = 3;
const int constGap = 12;

//Motor pins
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

PID rightPID(&InputRight, &OutputRight, &SetpointRight, 2, 3, 0, DIRECT);
PID leftPID(&InputLeft, &OutputLeft, &SetpointLeft, 2, 3, 0, DIRECT);

boolean turn = false;

//Encoder
//
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
  
  unsigned long now = 0;
  unsigned long old = 0;
//
  
unsigned long oldHcsr04 = -50;

boolean servoPosition = false; // false = esquerda; true = direita
Servo s;

void setServoPos( )
{
  if( servoPosition )
  {
    s.write( 0 ); 
  }
  else
  {
    s.write( 170 ); 
  }
}

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

float distance( NewPing sonar )
{
  unsigned int uS = sonar.ping(); // Send ping, get ping time in microseconds (uS).
  //Serial.print("Ping: ");
  float cmFromWall = uS / US_ROUNDTRIP_CM; // Convert ping time to distance in cm and print result (0 = outside set distance range)
  return cmFromWall;
}

void stopIt()
{
    digitalWrite( rightM.way, HIGH );
    digitalWrite( leftM.way, HIGH );
    analogWrite( rightM.vel, 0 );
    analogWrite( leftM.vel, 0 );
}

void moveFoward()
{  
  encoder();
  
  if( gapT > constGap + tolerance && gapT != 0 )
  {
    if( servoPosition )
    {
      analogWrite( leftM.vel, 255 );
    }
    
    else
    {
      analogWrite( rightM.vel, 255 );
    }
    
    delay(10);
  }
  
  else if( gapT < constGap - tolerance && gapT != 0 )
  {
    if( servoPosition )
    {
      analogWrite( rightM.vel, 255 );
    }
    
    else
    {
      analogWrite( leftM.vel, 255 );
    }
    
    delay(10);
  }
  
  if( now - old >= 500 )
  {    
    rpmR = countR;
    rpmL = countL;
    
//    Serial.print( "rpmR : " );
//    Serial.println( rpmR );
//    Serial.print( "rpmL : " );
//    Serial.println( rpmL );
    
    countR = 0;
    countL = 0;
    
//    Serial.println( OutputRight );
//    Serial.println( OutputLeft );
    
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

void setup()
{  
  //Encoder
  pinMode( encoderRPin, INPUT );
  pinMode( encoderLPin, INPUT );
  
  //Hc Sr04
  pinMode( TRIGGER_PIN_F, OUTPUT );
  pinMode( ECHO_PIN_F, INPUT );
  pinMode( TRIGGER_PIN_T, OUTPUT );
  pinMode( ECHO_PIN_T, INPUT );
  
  //Motor
  pinMode( rightM.way, OUTPUT );
  pinMode( leftM.way, OUTPUT );

  //Servo
  s.attach(SERVO);
  setServoPos();
  
  //PID
  SetpointRight = 50;
  SetpointLeft = 50;
  rightPID.SetMode(AUTOMATIC);
  leftPID.SetMode(AUTOMATIC);
  
  Serial.begin(9600);
}

void loop()
{
  now = millis();

  if( now - oldHcsr04 >= 50 )
  {
    gapF = distance( frontSensor );
    gapT = distance( turnSensor );
    
    oldHcsr04 = now;
  }
  
  if( (gapF > 10 || gapF == 0) && (turn == false) )
  {
    moveFoward();
  }
  
  else
  {
    if( !turn )
    {
      digitalWrite( rightM.way, LOW );
      digitalWrite( leftM.way, LOW );
      analogWrite( rightM.vel, OutputRight );
      analogWrite( leftM.vel, OutputLeft );
      delay(20);
      turn = true;
    }
    
    stopIt();
    
  }
}
