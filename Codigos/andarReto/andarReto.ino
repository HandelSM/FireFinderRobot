//Motor
#define MOTOR_L1  8
#define MOTOR_L2  9
#define MOTOR_R1  10
#define MOTOR_R2  11

typedef struct motor
{
  float vel;
  int maxVel;
  int minVel;
};
typedef struct motor Motor;

Motor leftMotor = { 155, 255, 120 }; // Motor nomeMotor = { velocidade, velocidadeMaxima, velocidadeMinima }
Motor rightMotor = { 155, 255, 130 };

//Encoder
#define ENCODERR_PIN 2
#define ENCODERL_PIN 3

int countR = 0;
int countL = 0;
int currentEncoderR = LOW;
int lastEncoderR = LOW;
int currentEncoderL = LOW;
int lastEncoderL = LOW;
boolean wasUpdateVel = false;

void encoder()
{
  currentEncoderR = digitalRead( ENCODERR_PIN );
  currentEncoderL = digitalRead( ENCODERL_PIN );
  
  if ( currentEncoderR != lastEncoderR )
  {
    countR++;
  }
  lastEncoderR = currentEncoderR;
  
  if ( currentEncoderL != lastEncoderL )
  {
    countL++;
  }
  lastEncoderL = currentEncoderL;
}

void setup ()
{
  pinMode( ENCODERR_PIN, INPUT );
  pinMode( ENCODERL_PIN, INPUT );
  pinMode( MOTOR_R1, OUTPUT );
  pinMode( MOTOR_L1, OUTPUT );
  pinMode( MOTOR_R2, OUTPUT );
  pinMode( MOTOR_L2, OUTPUT );
  Serial.begin(9600);
}

void loop()
{  
  encoder();
  
  int time = millis();
  
  if( time % 1000 == 0 )
  {
    wasUpdateVel = false;
  }
  
  if ( time % 1001 == 0 && !wasUpdateVel )
  {
    
    Serial.print("countL: ");
    Serial.println(countL);
    Serial.print("countR: ");
    Serial.println(countR);
    
    if( countR < countL )
    {
      rightMotor.vel += (countL - countR)/2;
    }
    
    if( countR > countL )
    {
      leftMotor.vel += (countR - countL)/2;
    }
    
    if( rightMotor.vel > 255 )
    {
      rightMotor.vel = 255;
      leftMotor.vel -= 1;
    }
    if( leftMotor.vel > 255 )
    {
      leftMotor.vel = 255;
      rightMotor.vel -= 1;
    }
    Serial.print("motorR: ");
    Serial.println(rightMotor.vel);
    Serial.print("motorL: ");
    Serial.println(leftMotor.vel);
    
    countR = 0;
    countL = 0;
    wasUpdateVel = true;
  }
  
  analogWrite( MOTOR_R1, rightMotor.vel );
  analogWrite( MOTOR_R2, 0 );
  analogWrite( MOTOR_L1, leftMotor.vel );
  analogWrite( MOTOR_L2, 0 );
}
