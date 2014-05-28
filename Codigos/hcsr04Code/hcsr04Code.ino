#include <Ultrasonic.h>

#define TRIGGER_PIN  12 
#define ECHO_PIN     13

int accuracy = 100;

Ultrasonic ultrasonic(TRIGGER_PIN, ECHO_PIN);

float distance = 0;
int i = 0;

void setup()  
{ 
  pinMode( TRIGGER_PIN, OUTPUT );
  pinMode( ECHO_PIN, INPUT );
  Serial.begin(9600);
}

void loop()  
{
  if( i < accuracy )
  {
    float cmFromWall;
    long microsec = ultrasonic.timing();
    cmFromWall = ultrasonic.convert(microsec, Ultrasonic::CM);
    distance += cmFromWall;
    i++;
  }
  
  else
  {
    distance = (int)(distance / accuracy);
    Serial.println( distance );
    distance = 0;
    i = 0;
  }
}

