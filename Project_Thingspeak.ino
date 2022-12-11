#include <WiFi.h>
#include "DHT.h"
#include "ThingSpeak.h"
char* ssid = "Anirudh"; 
char* passphrase = "12345678";
WiFiServer server(80);
WiFiClient client;
unsigned long myChannelNumber =  1971926;
const char * myWriteAPIKey = "BP48DWY515JWVIPQ";
unsigned long lastTime = 0;
unsigned long timerDelay = 1000;

#define DHTPIN 5

#define DHTTYPE DHT11 
DHT dht(DHTPIN, DHTTYPE);
void setup()
{
  Serial.begin(115200);

Serial.print("Connecting to ");

Serial.println(ssid);

WiFi.begin(ssid, passphrase);

while (WiFi.status() != WL_CONNECTED) {

delay(500);

Serial.print(".");

}



Serial.println("");

Serial.println("WiFi connected.");

Serial.println("IP address: ");

Serial.println(WiFi.localIP());

server.begin();

//----nitialize dht11

dht.begin();

ThingSpeak.begin(client); // Initialize ThingSpeak

}
void loop()
{
  if ((millis() - lastTime) > timerDelay)

{

delay(2500);

// Reading temperature or humidity takes about 250 milliseconds!

//float h = dht.readHumidity();

// Read temperature as Celsius (the default)

float t = dht.readTemperature();

float f = dht.readTemperature(true);

if (isnan(t) || isnan(f)) {

Serial.println(F("Failed to read from DHT sensor!"));

return;

}

Serial.print("Temperature (ºC): ");

Serial.print(t);

Serial.println("ºC");

Serial.print("Fan Status:");
float FanStatus = 0;//0->OFF
if(t>30)
  FanStatus = 1;
Serial.println(FanStatus);

ThingSpeak.setField(1, t);

ThingSpeak.setField(2, FanStatus);

// Write to ThingSpeak. There are up to 8 fields in a channel, allowing you to store up to 8 different

// pieces of information in a channel. Here, we write to field 1.

int x = ThingSpeak.writeFields(myChannelNumber,

myWriteAPIKey);

if(x == 200){

Serial.println("Channel update successful.");

}

else{

Serial.println("Problem updating channel. HTTP error code " + String(x));

}

lastTime = millis();

}

}
