typedef struct motor
{
  byte vel;
  byte way;
};
typedef struct motor Motor;

Motor rightM = { 6, 7 };
Motor leftM = { 5, 4 };

int encoderRPin = 2;

int countR = 0;

int currentEncoderRight = LOW;
int lastEncoderRight = LOW;

void encoder()
{
  currentEncoderRight = digitalRead( encoderRPin );
  
  if ( currentEncoderRight != lastEncoderRight )
  {
    countR++;
    Serial.print("Count R: ");
    Serial.println( countR );
    //Serial.println( i );
  }
  
  lastEncoderRight = currentEncoderRight;
}

void setup ()
{ 
  pinMode( encoderRPin, INPUT );
  //pinMode( encoderLPin, INPUT );
  
  pinMode( rightM.way, OUTPUT );
  pinMode( leftM.way, OUTPUT );
  
  Serial.begin(9600);
}

void loop()
{ 
  encoder();
  
  if( countR < 33 )
  {
    digitalWrite( rightM.way, HIGH );
    digitalWrite( leftM.way, LOW );
    analogWrite( rightM.vel, 105 );
    analogWrite( leftM.vel, 0 );
  }
  else
  {
    analogWrite( rightM.vel, 0 );
    analogWrite( leftM.vel, 0 ); 
    delay(1000);
    countR = 0;
  }
}


