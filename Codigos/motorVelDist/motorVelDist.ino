//HCSR04 lib and pins
#include <NewPing.h>

#define TRIGGER_PIN  12
#define ECHO_PIN     13
#define MAX_DISTANCE 150

//Motor pins
int motorR[2] = { 5, 6 };
int motorL[2] = { 9, 10 };

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);

typedef struct motor
{
  float vel;
  int maxVel;
  int minVel;
};
typedef struct motor Motor;

float distance()
{
  delay(30);                      // Wait 50ms between pings (about 20 pings/sec). 29ms should be the shortest delay between pings.
  unsigned int uS = sonar.ping(); // Send ping, get ping time in microseconds (uS).
  //Serial.print("Ping: ");
  float cmFromWall = uS / US_ROUNDTRIP_CM; // Convert ping time to distance in cm and print result (0 = outside set distance range)
  return cmFromWall;
}

void setup()
{
  pinMode( TRIGGER_PIN, OUTPUT );
  pinMode( ECHO_PIN, INPUT );
  
  pinMode( motorR[0], OUTPUT );
  pinMode( motorR[1], OUTPUT );
  pinMode( motorL[0], OUTPUT );
  pinMode( motorL[1], OUTPUT );
  Serial.begin(9600);
}

void loop()  
{  
  float gap = distance();
  
  Serial.println( gap );
  
  
  if( gap > 20 || gap == 0 )
  {
    analogWrite( motorR[0], 255 );
    analogWrite( motorR[1], 0 );
    analogWrite( motorL[0], 255 );
    analogWrite( motorL[1], 0 );                       
  }
  else
  {
    analogWrite( motorR[0], 0 );
    analogWrite( motorR[1], 250 );
    analogWrite( motorL[0], 250 );
    analogWrite( motorL[1], 0 );   
  }
}

