//HCSR04 lib and pins
#include <NewPing.h>
#include <Servo.h>
#include <PID_v1.h>

#define TRIGGER_PIN_T  10
#define ECHO_PIN_T     11

#define TRIGGER_PIN_F  12
#define ECHO_PIN_F     13

#define MAX_DISTANCE 150

#define SERVO 9

#define BUZZER_PIN 3

#define FLAME A0

NewPing frontSensor(TRIGGER_PIN_F, ECHO_PIN_F, MAX_DISTANCE);
NewPing turnSensor(TRIGGER_PIN_T, ECHO_PIN_T, MAX_DISTANCE);

float gapF = 0;
float gapT = 0;
const byte tolerance = 5;
int constGap = 12;

//Motor pins
typedef struct motor
{
  byte vel;
  byte way;
};
typedef struct motor Motor;

Motor rightM = { 5, 4 };
Motor leftM = { 6, 7 };

Motor current;
Motor other;

double SetpointRight, InputRight, OutputRight;
double SetpointLeft, InputLeft, OutputLeft;

PID rightPID(&InputRight, &OutputRight, &SetpointRight, 2, 3, 0, DIRECT);
PID leftPID(&InputLeft, &OutputLeft, &SetpointLeft, 2, 3, 0, DIRECT);

boolean FOGO = false;

//Encoder
//
  int encoderRPin = 2;
  int encoderLPin = 8;
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

void beep(unsigned char delayms)
{
  analogWrite( BUZZER_PIN, 255 );      // Almost any value can be used except 0 and 255
                           // experiment to get the best tone
  delay(delayms);          // wait for a delayms ms
  analogWrite( BUZZER_PIN, 0 );       // 0 turns it off
  delay(delayms);          // wait for a delayms ms   
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

void adjust()
{
  int sideDistance = distance (turnSensor);
  
  if (sideDistance < constGap - tolerance)
   {
    Serial.println("Entrei if ");

     digitalWrite(current.way, HIGH);
     digitalWrite(other.way, HIGH);
     analogWrite( current.vel, 255 );
     analogWrite( other.vel, 150 );
     delay( 150 );
     
   }
   
  else if (sideDistance > constGap + tolerance)
  {
    
     Serial.println("Entrei else");
     digitalWrite(current.way, HIGH);
     digitalWrite(other.way, HIGH);
     analogWrite( current.vel, 150 );
     analogWrite( other.vel, 255 );
     delay( 150 );
  }
}


void moveFoward()
{
 
  adjust();
  encoder();
  
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
    
   // Serial.println( OutputRight );
   // Serial.println( OutputLeft );
    
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
  
  
  
//  encoder();
//  
//  if( gapT > constGap + tolerance && gapT != 0 )
//  {
//    if( servoPosition )
//    {
//      analogWrite( leftM.vel, 255 );
//    }
//    
//    else
//    {
//      analogWrite( rightM.vel, 255 );
//    }
//    
//    delay(10);
//  }
//  
//  else if( gapT < constGap - tolerance && gapT != 0 )
//  {
//    if( servoPosition )
//    {
//      analogWrite( rightM.vel, 255 );
//    }
//    
//    else
//    {
//      analogWrite( leftM.vel, 255 );
//    }
//    
//    delay(10);
//  }
//  
//  if( now - old >= 500 )
//  {    
//    rpmR = countR;
//    rpmL = countL;
//    
////    Serial.print( "rpmR : " );
////    Serial.println( rpmR );
////    Serial.print( "rpmL : " );
////    Serial.println( rpmL );
//    
//    countR = 0;
//    countL = 0;
//    
////    Serial.println( OutputRight );
////    Serial.println( OutputLeft );
//    
//    old = now;
//  }
//  
//  InputRight = rpmR;
//  InputLeft = rpmL;
//  
//  rightPID.Compute();
//  leftPID.Compute();
//  
//  digitalWrite( rightM.way, HIGH );
//  digitalWrite( leftM.way, HIGH );
//  analogWrite( rightM.vel, OutputRight );
//  analogWrite( leftM.vel, OutputLeft );
}

void resetVariables()
{
  countL = 0;
  countR = 0;
  old = millis();
}

void turnRight()
{
  digitalWrite( rightM.way, LOW );
  digitalWrite( leftM.way, HIGH );
  analogWrite( rightM.vel, 255 );
  analogWrite( leftM.vel, 255 );
  
  delay( 150 );
  
  stopIt();
  
  delay( 1000 );
  
  resetVariables();
}

void turnLeft()
{
  digitalWrite( rightM.way, HIGH );
  digitalWrite( leftM.way, LOW );
  analogWrite( rightM.vel, 255 );
  analogWrite( leftM.vel, 255 );
  
  delay( 150 );
  
  stopIt();
  
  delay( 1000 );
  
  resetVariables();
}

void whereGo()
{
  delay(50);
  
  gapF = distance( frontSensor );
  
  if( gapF > 20 || gapF == 0 )
  {
//    if( rightGap <= leftGap )
//    {
//      constGap = rightGap;
//    }
//    else
//    {
//      constGap = leftGap;
//      servoPosition = false;
//      setServoPos();
//      delay(1500);
//    }
  
  }
  else
  {
    int rightGap;
    int leftGap;
    
    servoPosition = false;
    setServoPos();
    delay(1500);
    
    leftGap = distance( turnSensor );
    
    servoPosition = true;
    setServoPos();
    delay(1500);
    
    rightGap = distance( turnSensor );
    delay(30);
    
    if( rightGap < 15 )
    {
      if(leftGap < 15)
      {
        turnRight();
        current = leftM;
        other = rightM;
      }
      else
      {
        turnLeft();
        current = rightM;
        other = leftM;
      }
    }
    
    else
    {
      if( rightGap >= leftGap )
      {
        turnRight();
      }
      else
      {
        turnLeft();
      }
    }
    
    whereGo();
  }
  
  resetVariables();
}

void setup()
{  
  beep(50);
  beep(50);
  
  //Flame sensor
  pinMode( FLAME, INPUT );
  
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
  
  whereGo();
}

void loop()
{
  
  int flameSensor = analogRead(FLAME);
  
  //Serial.println(flameSensor);
  
  if( flameSensor > 1000000 || FOGO == true )
  {
    beep( 200 );
    stopIt(); 
    
    FOGO = true;
  }
  
  else
  {
    now = millis();
    
    if( now - oldHcsr04 >= 50 )
    {
      gapF = distance( frontSensor );
      gapT = distance( turnSensor );
      
      oldHcsr04 = now;
    }
  
    if( (gapF > 17 || gapF == 0) )
    {
      moveFoward();
    }
    
    else
    {
      digitalWrite( rightM.way, LOW );
      digitalWrite( leftM.way, LOW );
      analogWrite( rightM.vel, 250 );
      analogWrite( leftM.vel, 255 );
      delay(10);
      stopIt();
      whereGo();
    }
  }
}
