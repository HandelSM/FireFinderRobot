//HCSR04 lib and pins
#include <Ultrasonic.h>
#define TRIGGER_PIN  12
#define ECHO_PIN     13

//Motor pins
#define MOTOR_L      10
#define MOTOR_R       9

Ultrasonic ultrasonic( TRIGGER_PIN, ECHO_PIN );

typedef struct motor
{
  float vel;
  int maxVel;
  int minVel;
};
typedef struct motor Motor;

Motor rightMotor = { 0, 255, 130 }; // Motor nomeMotor = { velocidade, velocidadeMaxima, velocidadeMinima }

Motor leftMotor = { 0, 240, 120 };

float distance()
{
  float cmFromWall;
  long microsec = ultrasonic.timing();
  cmFromWall = ultrasonic.convert( microsec, Ultrasonic::CM );
  return cmFromWall;
}

void updateVel( float d, struct motor m )
{
  float vel = d * m.maxVel/200; // Para ter a velocidade máxima, precisa estar a 200cm, ou mais, da parede.
  
  if( vel < m.minVel && d > 20 )
  {
    vel = m.minVel;
  }
  
  if( vel > m.maxVel )
  {
    vel = m.maxVel;
  }
  
  m.vel = vel;
}

void setup()
{
  pinMode( MOTOR_R, OUTPUT );
  pinMode( MOTOR_L, OUTPUT );
  Serial.begin(9600);
}

void loop()  
{ 
  Serial.println( distance() );
  
  updateVel( distance(), rightMotor );
  updateVel( distance(), leftMotor );
  
  analogWrite( MOTOR_R, rightMotor.vel );
  analogWrite( MOTOR_L, leftMotor.vel );                       
}

