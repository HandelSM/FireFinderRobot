int FlameSensor; 
void setup()
{
     Serial.begin(9600);  
}
void loop()
{
      FlameSensor=analogRead(5); 
      Serial.println("EH FOGO MANEH");
      Serial.println(FlameSensor);    
      delay(100);
}
