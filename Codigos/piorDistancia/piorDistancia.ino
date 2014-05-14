//HCSR04 lib and pins
#include <Ultrasonic.h>
#define TRIGGER_PIN  12
#define ECHO_PIN     13

//Motor pins
#define MOTOR_L      10
#define MOTOR_R       9

float oldDist = 999999;

Ultrasonic ultrasonic( TRIGGER_PIN, ECHO_PIN );

typedef struct motor
{
  float vel;
  int maxVel;
  int minVel;
};
typedef struct motor Motor;

Motor rightMotor = { 0, 255, 130 }; // Motor nomeMotor = { velocidade, velocidadeMaxima, velocidadeMinima }

Motor leftMotor = { 255, 240, 120 };

float distance()
{
  float cmFromWall;
  long microsec = ultrasonic.timing();
  cmFromWall = ultrasonic.convert( microsec, Ultrasonic::CM );
  return cmFromWall;
}

void ChangePosition(float d)
{
  
  float newDist = distance();
  
  if( newDist > oldDist)
  {
    leftMotor.vel = 0;
  }
  else
  {
    oldDist = newDist; 
    Serial.println("Entrou");
  }
  
 
}



void setup()
{
  pinMode( TRIGGER_PIN, OUTPUT );
  pinMode( ECHO_PIN, INPUT );
  pinMode( MOTOR_R, OUTPUT );
  pinMode( MOTOR_L, OUTPUT );
  Serial.begin(9600);
}

void loop()  
{ 
  int d = distance();
  Serial.println( d );
  ChangePosition( d );
  analogWrite( MOTOR_R, rightMotor.vel );
  analogWrite( MOTOR_L, leftMotor.vel );                       
}

