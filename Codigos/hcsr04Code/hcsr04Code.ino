#include <Ultrasonic.h>

#define TRIGGER_PIN  12
#define ECHO_PIN     13

Ultrasonic ultrasonic(TRIGGER_PIN, ECHO_PIN);

void setup()  
{ 
  pinMode( TRIGGER_PIN, OUTPUT );
  pinMode( ECHO_PIN, INPUT );
  Serial.begin(9600);
} 

void loop()  
{ 
  int cmFromWall;
  long microsec = ultrasonic.timing();
  cmFromWall = ultrasonic.convert(microsec, Ultrasonic::CM);
  
  Serial.println(cmFromWall);
}

