#include "DHT.h"
#include <Servo.h>
#define DHTPIN 5
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

Servo myservo; //create servo object to control a servo
void setup()
{
  Serial.begin(115200);
  //Serial.println();
  myservo.attach(13); // attaches the servo on GPIO2(D4)to the servo object
  dht.begin();
  
   // Connect DHT sensor to GPIO 15 (D8)
}

void loop()
{
  //Serial.print(F("Hai...."));
  delay(2000);

  float t = dht.readTemperature();

  Serial.print(F("%  Temperature: "));
  Serial.print(t);
  //Serial.print(F("Up...."));
  Serial.print("\n");
  if(t>30)
  {
   int pos;  
   for(pos = 0; pos <= 180; pos += 1)
   {
     //goes from 0 degress to 360 degress in steps of 1 degree
     myservo.write(pos);
     delay(15);  // wait for 15ms after each 1 degree rotation
   }
   for(pos = 180; pos >= 0; pos -= 1)
   {
     //goes from 360 degress to 0 degress in steps of 1 degree
     myservo.write(pos);
     delay(15);  // wait for 15ms after each 1 degree rotation
   }
  } // end of if condition
  
} // end of loop.
