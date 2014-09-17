#include <Servo.h>
 
#define SERVO 6 // Porta Digital 6 PWM
 
Servo s; // Variável Servo
int pos; // Posição Servo
 
void setup ()
{
  s.attach(SERVO);
  Serial.begin(9600);
  s.write(0); // Inicia motor posição zero
}
 
void loop()
{
  s.write(0);
  delay(1000);
  s.write(70);
  delay(1000);
  s.write(150);
  delay(1000);
  s.write(70);
  delay(1000);
}
