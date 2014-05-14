#include <Ultrasonic.h>

#define TRIGGER_PIN  12
#define ECHO_PIN     13

Ultrasonic ultrasonic(TRIGGER_PIN, ECHO_PIN);

int motorR = 9;
int motorL = 10;
int vR = 0;
int vL = 0;

int maxVR = 255;
int maxVL = 255;

int minVR = 100;
int minVL = 100;

//long timeChangeSpeed = 3000;

void setup()  
{ 
  pinMode( motorR, OUTPUT );
  pinMode( motorL, OUTPUT );
  Serial.begin(9600);
} 


void loop()  
{ 
  int cmFromWall;
  long microsec = ultrasonic.timing();
  cmFromWall = ultrasonic.convert(microsec, Ultrasonic::CM);
  
  Serial.println(cmFromWall);
  vR = cmFromWall * 4 ;
  vL = cmFromWall * 4 ;
  /*
  if( cmFromWall > 100 )
  {
    vR = maxVR;
    vL = maxVL;
  }
  else
  {
    if( cmFromWall > 35 )
    {
      vR = minVR ;
      vL = minVL;
    }
    else
    {
      vR = 0;
      vL = 0; 
    }
  }*/
  
  analogWrite( motorR, vR ); 
  analogWrite( motorL, vL );
  
  /*
  if( millis() < timeChangeSpeed )
  {
    vR = maxVR;
    vL = maxVL; 
  }
  
  else
  {
    vR = minVR;
    vL = minVL;
    if( millis() > timeChangeSpeed + 3000 )
      timeChangeSpeed += 6000;
  }  
  */                       
}

