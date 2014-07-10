#include <PID_v1.h>

int motor =  9;

double Setpoint, Input, Output;

PID rightPID(&Input, &Output, &Setpoint, 0.1, 3, 0, DIRECT);

int encoderPin = 2;

int count = 0;

int currentEncoder = LOW;
int lastEncoder = LOW;

boolean wasUpdate = false;

int rotation = 0;

void encoder()
{
  currentEncoder = digitalRead( encoderPin );
  
  if ( currentEncoder != lastEncoder )
  {
    count++;
//    Serial.print("Count R: ");
//    Serial.println( countR );
  }
  
  lastEncoder = currentEncoder;
}

void setup ()
{
  Setpoint = 50;
  
  rightPID.SetMode(AUTOMATIC);
  
  pinMode( encoderPin, INPUT );
  
  pinMode( motor, OUTPUT );
  
  Serial.begin(9600);
}

void loop()
{
  encoder();
  
  int time = millis();
  
  if( time % 500 == 0 )
  {
    wasUpdate = false;
  }
  
  if( time % 501 == 0 && !wasUpdate )
  {    
    rotation = count;
    
    Serial.print( "rotation: " );
    Serial.println( rotation );
    
    count = 0;
    
    wasUpdate = true;
  }
  
  Input = rotation;
  
  rightPID.Compute();
  
  //Serial.print( "RightOutput: " );
  //Serial.println( Output );
  
  analogWrite( motor, Output );
}

