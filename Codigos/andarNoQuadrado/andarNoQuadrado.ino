//HCSR04 lib and pins
#include <NewPing.h>

#define TRIGGER_PIN  12
#define ECHO_PIN     13
#define MAX_DISTANCE 150

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);

#include <PID_v1.h>

//Motor pins
typedef struct motor
{
  byte vel;
  byte way;
};
typedef struct motor Motor;

Motor rightM = { 5, 4 };
Motor leftM = { 6, 7 };

double SetpointRight, InputRight, OutputRight;
double SetpointLeft, InputLeft, OutputLeft;

PID rightPID(&InputRight, &OutputRight, &SetpointRight, 2, 3, 0, DIRECT);
PID leftPID(&InputLeft, &OutputLeft, &SetpointLeft, 2, 3, 0, DIRECT);

boolean turn = false;

//Encoder
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
unsigned long oldHcsr04 = 0;

float gap = 0;

boolean FOGO = false;

#define BUZZER_PIN 3

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

float distance()
{
  unsigned int uS = sonar.ping(); // Send ping, get ping time in microseconds (uS).
  //Serial.print("Ping: ");
  float cmFromWall = uS / US_ROUNDTRIP_CM; // Convert ping time to distance in cm and print result (0 = outside set distance range)
  return cmFromWall;
}

void moveFoward()
{  
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
    
    Serial.println( OutputRight );
    Serial.println( OutputLeft );
    
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

void moveRight()
{ 
  byte randNumber = random(2);
  
  Serial.println(randNumber);
  
  if( randNumber )
  {
    digitalWrite( rightM.way, LOW );
    digitalWrite( leftM.way, HIGH );
    analogWrite( rightM.vel, OutputRight );
    analogWrite( leftM.vel, OutputLeft );
  }
  
  else
  {
    digitalWrite( rightM.way, HIGH );
    digitalWrite( leftM.way, LOW );
    analogWrite( rightM.vel, OutputRight );
    analogWrite( leftM.vel, OutputLeft );    
  }
  delay( 250 );
  
  digitalWrite( rightM.way, HIGH );
  digitalWrite( leftM.way, HIGH );
  analogWrite( rightM.vel, 0 );
  analogWrite( leftM.vel, 0 );
  
  delay( 800 );
  
  countL = 0;
  countR = 0;
  
  digitalWrite( rightM.way, HIGH );
  digitalWrite( leftM.way, HIGH );
  analogWrite( rightM.vel, OutputRight );
  analogWrite( leftM.vel, OutputLeft );
  
  turn = false;
  
  old = millis();
}

void setup()
{
  beep(50);
  beep(50);
  beep(50);  
  
  digitalWrite( 8, HIGH ); //Mais um VCC
  
  //PID
  SetpointRight = 60;
  SetpointLeft = 60;
  rightPID.SetMode(AUTOMATIC);
  leftPID.SetMode(AUTOMATIC);
  
  //Encoder
  pinMode( encoderRPin, INPUT );
  pinMode( encoderLPin, INPUT );
  
  //Hc Sr04
  pinMode( TRIGGER_PIN, OUTPUT );
  pinMode( ECHO_PIN, INPUT );
  
  //Motor
  pinMode( rightM.way, OUTPUT );
  pinMode( leftM.way, OUTPUT );
  
  //Buzzer
  pinMode( BUZZER_PIN, OUTPUT );
  
  Serial.begin(9600);
}

void loop()
{  
  int flameSensor = analogRead(A0);
  
  if( flameSensor > 100 || FOGO == true )
  {
    beep( 200 );
    digitalWrite( rightM.way, HIGH );
    digitalWrite( leftM.way, HIGH );
    analogWrite( rightM.vel, 0 );
    analogWrite( leftM.vel, 0 );
    
    FOGO = true;
  }
  
  else
  {
    now = millis();
  
    if( now - oldHcsr04 >= 50 )
    {
      gap = distance();
      
      oldHcsr04 = now;
    }
    
    //Serial.println( gap );
    
    if( (gap > 20 || gap == 0) && (turn == false) )
    {
      moveFoward();
    }
    
    else
    {
      if( turn == false )
      {
        countR = 0;
        countL = 0;
        turn = true;
      }
      moveRight();
    }
  }
}
