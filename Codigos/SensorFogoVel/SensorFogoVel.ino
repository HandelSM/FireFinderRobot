#define MOTOR_L  8
#define MOTOR_R 9

typedef struct motor
{
  float vel;
  int maxVel;
  int minVel;
};

typedef struct motor Motor;

Motor leftMotor = { 155, 255, 120 }; // Motor nomeMotor = { velocidade, velocidadeMaxima, velocidadeMinima }
Motor rightMotor = { 155, 255, 130 };

void fogo (int f)
{
  if (f>200)
  {
    rightMotor.vel = 0;
    leftMotor.vel = 0;
  }
  else 
  {
    rightMotor.vel = 155;
    leftMotor.vel = 155;  
  } 
}

void setup()
{
     pinMode (MOTOR_R, OUTPUT);
     pinMode (MOTOR_L, OUTPUT);
     Serial.begin(9600);
}
void loop()
{  
  
     int FlameSensor = analogRead(A0);
     
     fogo(FlameSensor);
     analogWrite( MOTOR_R, rightMotor.vel );
     analogWrite( MOTOR_L, leftMotor.vel );
     
     Serial.println(FlameSensor);
     Serial.println(rightMotor.vel);
     Serial.println(leftMotor.vel);
       
      delay(100);
}
