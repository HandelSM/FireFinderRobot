int motorR[2] = { 5, 6 };
int motorL[2] = { 9, 10 };

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
  
  pinMode( motorR[0], OUTPUT );
  pinMode( motorR[1], OUTPUT );
  pinMode( motorL[0], OUTPUT );
  pinMode( motorL[1], OUTPUT );
  
  Serial.begin(9600);
}

void loop()
{  
  analogWrite( motorR[0], 0 );
  analogWrite( motorR[1], 255);
  analogWrite( motorL[0], 255);
  analogWrite( motorL[1], 0 );
  delay( 300 );

  analogWrite( motorR[0], 0 );
  analogWrite( motorR[1], 0);
  analogWrite( motorL[0], 0);
  analogWrite( motorL[1], 0 );
  delay(3000);
}

