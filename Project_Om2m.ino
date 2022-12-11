#include<HTTPClient.h>
#include <WiFi.h>
#include "time.h"
#include "DHT.h"

#define MAIN_SSID "Narzo"
#define MAIN_PASS "123456789"
#define DHTTYPE DHT11

#define CSE_IP      "192.168.79.231"
#define CSE_PORT    8080
#define HTTPS     false
#define OM2M_ORGIN    "admin:admin"
#define OM2M_MN     "/~/in-cse/in-name/"
#define OM2M_AE     "K_Anirudh"
#define OM2M_DATA_CONT  "Project/TemperatureandFanStatus"

#define DHTPIN 5
DHT dht(DHTPIN, DHTTYPE);

const char* ntpServer = "pool.ntp.org";
unsigned long epochTime; 
long randNumber;



HTTPClient http;

unsigned long getTime() {
  time_t now;
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    //Serial.println("Failed to obtain time");
    return(0);
  }
  time(&now);
  return now;
}

void setup() {
Serial.begin(115200);
configTime(0, 0, ntpServer);
WiFi.begin(MAIN_SSID,MAIN_PASS);

while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print("#");
  }
}

void loop() {
  epochTime = getTime();
  
  //Sensor Block  BME 
 static int i=0;
float Temperature=dht.readTemperature();
 //float Humidity=dht.readHumidity();
char *FanStatus = "OFF";
if(Temperature>30)
  FanStatus = "ON";
 
String data="[" + String(epochTime) + ", " + String(Temperature) + " , " + FanStatus+"]";

String server="http://" + String() + CSE_IP + ":" + String() + CSE_PORT + String()+OM2M_MN;

Serial.println(data);
http.begin(server + String() +OM2M_AE + "/" + OM2M_DATA_CONT + "/");

http.addHeader("X-M2M-Origin", OM2M_ORGIN);
http.addHeader("Content-Type", "application/json;ty=4");
http.addHeader("Content-Length", "100");

String label = "Node-1";

String req_data = String() + "{\"m2m:cin\": {"

  + "\"con\": \"" + data + "\","

  + "\"rn\": \"" + "cin_"+String(i++) + "\","

  + "\"lbl\": \"" + label + "\","

  + "\"cnf\": \"text\""

  + "}}";
int code = http.POST(req_data);
http.end();
Serial.println(code);
delay(1000);
}
