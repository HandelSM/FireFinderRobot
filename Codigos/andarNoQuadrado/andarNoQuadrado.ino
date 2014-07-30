//HCSR04 lib and pins
#include <NewPing.h>

#define TRIGGER_PIN  12
#define ECHO_PIN     13
#define MAX_DISTANCE 150

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);

#include <PID_v1.h>

//Motor pins
int motorR[2] = { 5, 6 };
int motorL[2] = { 9, 10 };

double SetpointRight, InputRight, OutputRight;
double SetpointLeft, InputLeft, OutputLeft;

PID rightPID(&InputRight, &OutputRight, &SetpointRight, 2, 6, 0, DIRECT);
PID leftPID(&InputLeft, &OutputLeft, &SetpointLeft, 2, 6, 0, DIRECT);

boolean turn = false;

//Encoder
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
unsigned long oldHcsr04 = 0;

float gap = 0;

boolean FOGO = false;

#define BUZZER_PIN 11

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
  //delay(30);                      // Wait 50ms between pings (about 20 pings/sec). 29ms should be the shortest delay between pings.
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
    
//    Serial.print( "rpmR : " );
//    Serial.println( rpmR );
//    Serial.print( "rpmL : " );
//    Serial.println( rpmL );
    
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
  
  analogWrite( motorR[0], OutputRight );
  analogWrite( motorR[1], 0 );
  analogWrite( motorL[0], OutputLeft );
  analogWrite( motorL[1], 0 );
}

void moveRight()
{
  encoder();
  
  //Serial.print( "countR : " );
  //Serial.println( countR );
  Serial.print( "countL : " );
  Serial.println( countL );
  
  if( countL < 20 )
  {
    float strong = 1.3;
    analogWrite( motorR[0], 0 );
    analogWrite( motorR[1], 255);//OutputRight * strong);
    analogWrite( motorL[0], 255);//OutputLeft * strong);
    analogWrite( motorL[1], 0 );
  }
  else
  {
    analogWrite( motorR[0], 0 );
    analogWrite( motorR[1], 0 );
    analogWrite( motorL[0], 0 );
    analogWrite( motorL[1], 0 );
    delay(1000);
    old = millis();
    turn = false;
    countL = 0;
    countR = 0;
  }
}

void setup()
{
  beep(50);
  beep(50);
  beep(50);  
  
  //PID
  SetpointRight = 40;
  SetpointLeft = 40;
  rightPID.SetMode(AUTOMATIC);
  leftPID.SetMode(AUTOMATIC);
  
  //Encoder
  pinMode( encoderRPin, INPUT );
  pinMode( encoderLPin, INPUT );
  
  //Hc Sr04
  pinMode( TRIGGER_PIN, OUTPUT );
  pinMode( ECHO_PIN, INPUT );
  
  //Motor
  pinMode( motorR[0], OUTPUT );
  pinMode( motorR[1], OUTPUT );
  pinMode( motorL[0], OUTPUT );
  pinMode( motorL[1], OUTPUT );
  
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
    analogWrite( motorR[0], 0 );
    analogWrite( motorR[1], 0 );
    analogWrite( motorL[0], 0 );
    analogWrite( motorL[1], 0 ); 
    
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
    
    Serial.println( gap );
    
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
