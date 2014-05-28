//HCSR04 lib and pins
#include <Ultrasonic.h>
#define TRIGGER_PIN  12
#define ECHO_PIN     13

//Motor pins
#define MOTOR_L      9
#define MOTOR_R      10

float oldDist = 999999;

const int ACCURACY = 300; // Define a precisão do sensor. Quanto maior acurracy, maior precisão porem menor velocidade de atualização da posição
float samples[ACCURACY];
int index = 0; // Indice do sample
boolean was = false;

Ultrasonic ultrasonic( TRIGGER_PIN, ECHO_PIN );

typedef struct motor
{
  float vel;
  int maxVel;
  int minVel;
};
typedef struct motor Motor;

Motor leftMotor = { 0, 240, 120 }; // Motor nomeMotor = { velocidade, velocidadeMaxima, velocidadeMinima }

Motor rightMotor = { 250, 255, 130 };

int distance()
{
  float cmFromWall;
  long microsec = ultrasonic.timing();
  cmFromWall = ultrasonic.convert(microsec, Ultrasonic::CM);
  
  float sum = 0;
  
  if( was )
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
    was = true;
  }
  for( int i = 0; i < ACCURACY; i++ )
  {
     sum += samples[ i ];
  }
  
  return (int) (sum/ACCURACY);
}

void ChangePosition(float d)
{
  static int parou = 0;
  float newDist = distance();
  
  if( newDist > oldDist )
  {
    rightMotor.vel = 0;
    if( !parou )
    {
      parou = 1;
      Serial.println("Parou!");
    }
  }
  else
  {
    oldDist = newDist;
    //Serial.print("Entrou! dist = ");
    //Serial.println(newDist);
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

