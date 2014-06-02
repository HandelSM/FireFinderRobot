int encoder1Pin = 2;
int encoder2Pin = 3;
int count1 = 0;
int count2 = 0;
int currentE1 = LOW;
int lastE1 = LOW;
int currentE2 = LOW;
int lastE2 = LOW;
boolean was = false;

void setup ()
{
  pinMode( encoder1Pin, INPUT );
  pinMode( encoder2Pin, INPUT );
  Serial.begin(9600);
}

void loop()
{
  currentE1 = digitalRead( encoder1Pin );
  currentE2 = digitalRead( encoder2Pin );
  
  if ( currentE1 != lastE1 )
  {
    count1++;
    //Serial.print("Count 1: ");
    //Serial.println( count1 );
  }
  
  if ( currentE2 != lastE2 )
  {
    count2++;
    //Serial.print("Count 2: ");
    //Serial.println( count2 );
  }
  
  lastE1 = currentE1;
  lastE2 = currentE2;
  
  int time = millis();
  
  if( time % 1000 == 0 )
  {
    was = false;
  }
  
  if ( time % 1001 == 0 && !was )
  {
    int rpm1 = (count1 * 3/2);
    int rpm2 = (count2 * 3/2);
    Serial.print("RPM1: ");
    Serial.print(rpm1);
    Serial.print("  RPM2: ");
    Serial.println(rpm2);
    count1 = 0;
    count2 = 0;
    was = true;
  }
}

