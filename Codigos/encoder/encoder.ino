unsigned long tempo = 0;
unsigned long tempo_atual;
unsigned long tempo_voltas;
int encoder1Pin = 2;
int encoder2Pin = 3;
int count1 = 0;
int count2 = 0;
int currentE1 = LOW;
int lastE1 = LOW;
int currentE2 = LOW;
int lastE2 = LOW;
int velocidade;

void setup ()
{
  pinMode( encoder1Pin, INPUT );
  pinMode( encoder2Pin, INPUT );
  Serial.begin(9600);
}

void loop()
{
  //tempo_atual = millis();
  
  
  currentE1 = digitalRead( encoder1Pin );
  currentE2 = digitalRead( encoder2Pin );
  
  if ( currentE1 != lastE1 )
  {
    count1++;
    Serial.print("Count 1: ");
    Serial.println( count1 );
  }
  
  if ( currentE2 != lastE2 )
  {
    count2++;
    Serial.print("Count 2: ");
    Serial.println( count2 );
  }
  
  lastE1 = currentE1;
  lastE2 = currentE2;
  /*  
  if (cont == 120)
  {
    tempo_voltas = tempo_atual - tempo;
    tempo = millis();
    int volta = tempo_voltas / 3;
    velocidade = 60000 / volta;
    Serial.print("RPM:");
    Serial.println(velocidade);
    cont = 0;
  }*/
}

