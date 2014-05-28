#include <Ultrasonic.h>

#define TRIGGER_PIN  12 
#define ECHO_PIN     13

Ultrasonic ultrasonic(TRIGGER_PIN, ECHO_PIN);

const int ACCURACY = 50; // Define a precisão do sensor. Quanto maior acurracy, maior precisão porem menor velocidade de atualização da posição
float samples[ACCURACY];
int index = 0; // Indice do sample
boolean was = false;

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

void setup()  
{ 
  pinMode( TRIGGER_PIN, OUTPUT );
  pinMode( ECHO_PIN, INPUT );
  Serial.begin(9600);
  Serial.println( "Start" );
}

void loop()  
{
    Serial.print( "Distance: " );
    Serial.println( distance() );
}

