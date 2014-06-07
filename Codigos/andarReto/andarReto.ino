//HCSR04
#include <Ultrasonic.h>
#define TRIGGER_PIN 12
#define ECHO_PIN    13
#define ACCURACY    100 // Quanto maior acurracy, maior precisão porem menor velocidade de atualização da posição

float samples[ACCURACY];
int index = 0; // Indice do sample
boolean wasSensor = false;

Ultrasonic ultrasonic( TRIGGER_PIN, ECHO_PIN );

//Motor
#define MOTOR_L  9
#define MOTOR_R  10

typedef struct motor
{
  float vel;
  int maxVel;
  int minVel;
};
typedef struct motor Motor;

Motor leftMotor = { 255, 240, 120 }; // Motor nomeMotor = { velocidade, velocidadeMaxima, velocidadeMinima }
Motor rightMotor = { 255, 255, 130 };


//Encoder
#define ENCODERR_PIN 2
#define ENCODERL_PIN 3

int countR = 0;
int countL = 0;
int currentE1 = LOW;
int lastE1 = LOW;
int currentE2 = LOW;
int lastE2 = LOW;
boolean wasEncoder = false;

//

int distance()
{
  float cmFromWall;
  long microsec = ultrasonic.timing();
  cmFromWall = ultrasonic.convert(microsec, Ultrasonic::CM);
  
  float sum = 0;
  
  if( wasSensor )
  {
    samples[ index%ACCURACY ] = cmFromWall;
    index++;
  }
  else
  {
    for( int i = 0; i < ACCURACY; i++ )
    {
      samples[i] = cmFromWall;
    }
    wasSensor = true;
  }
  for( int i = 0; i < ACCURACY; i++ )
  {
     sum += samples[ i ];
  }
  
  return (int) (sum/ACCURACY);
}

void setup ()
{
  pinMode( ENCODERR_PIN, INPUT );
  pinMode( ENCODERL_PIN, INPUT );
  pinMode( TRIGGER_PIN, OUTPUT );
  pinMode( ECHO_PIN, INPUT );
  pinMode( MOTOR_R, OUTPUT );
  pinMode( MOTOR_L, OUTPUT );
  Serial.begin(9600);
}

void loop()
{
  currentE1 = digitalRead( ENCODERR_PIN );
  currentE2 = digitalRead( ENCODERL_PIN );
  
  if ( currentE1 != lastE1 )
  {
    countR++;
  }
  
  if ( currentE2 != lastE2 )
  {
    countL++;
  }
  
  lastE1 = currentE1;
  lastE2 = currentE2;
  
  int time = millis();
  
  if( time % 1000 == 0 )
  {
    wasEncoder = false;
  }
  
  if ( time % 1001 == 0 && !wasEncoder )
  {
    int rpmR = (countR * 3/2);
    int rpmL = (countL * 3/2);
    
    if( rpmR > rpmL )
    {
      rightMotor.vel = rightMotor.vel - ( rpmR - rpL )/2;
      leftMotor.vel = leftMotor.vel + ( rpmR - rpL )/2;
    }
    
    if( rpmR < rpmL )
    {
      rightMotor.vel = rightMotor.vel + ( rpmR - rpL )/2;
      leftMotor.vel = leftMotor.vel - ( rpmR - rpL )/2;     
    }
    
    countR = 0;
    countL = 0;
    wasEncoder = true;
  }
  
  analogWrite( MOTOR_R, rightMotor.vel );
  analogWrite( MOTOR_L, leftMotor.vel );
}

