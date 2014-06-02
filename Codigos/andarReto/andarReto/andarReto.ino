//HCSR04
#include <Ultrasonic.h>
#define TRIGGER_PIN 12
#define ECHO_PIN    13
#define ACCURACY    100 // Define a precisão do sensor. Quanto maior acurracy, maior precisão porem menor velocidade de atualização da posição

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

Motor leftMotor = { 0, 240, 120 }; // Motor nomeMotor = { velocidade, velocidadeMaxima, velocidadeMinima }
Motor rightMotor = { 250, 255, 130 };


//Encoder
#define ENCODER1_PIN 2
#define ENCODER2_PIN 3

int count1 = 0;
int count2 = 0;
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
  pinMode( ENCODER1_PIN, INPUT );
  pinMode( ENCODER2_PIN, INPUT );
  pinMode( MOTOR_R, OUTPUT );
  pinMode( MOTOR_L, OUTPUT );
  Serial.begin(9600);
}

void loop()
{
  currentE1 = digitalRead( ENCODER1_PIN );
  currentE2 = digitalRead( ENCODER2_PIN );
  
  if ( currentE1 != lastE1 )
  {
    count1++;
    //Serial.print("Count 1: ");
    //Serial.println( count1 );
  }
  
  if ( currentE2 != lastE2 )
  {
    count2++;
    //Serial.print("Count 2: ");
    //Serial.println( count2 );
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
    int rpm1 = (count1 * 3/2);
    int rpm2 = (count2 * 3/2);
    Serial.print("RPM1: ");
    Serial.print(rpm1);
    Serial.print("  RPM2: ");
    Serial.println(rpm2);
    count1 = 0;
    count2 = 0;
    wasEncoder = true;
  }
}

