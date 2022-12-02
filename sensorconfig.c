#include <WiFi.h>
#include <HTTPClient.h> 
#include "DHT.h"

#define DHTPIN 14  
#define Fan 12
#define LED 13  
#define TRIG_PIN 23
#define ECHO_PIN 22
#define DHTTYPE DHT11   
DHT dht(DHTPIN, DHTTYPE);
float duration_us, distance_cm;

const char* ssid = "iQOO Z3 5G";
const char* password =  "1234567890";
char* str="";
void setup() {
  

  Serial.begin(115200);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  Serial.println("Connected to the WiFi network");
    // configure the trigger pin to output mode
  pinMode(TRIG_PIN, OUTPUT);
  // configure the echo pin to input mode
  pinMode(ECHO_PIN, INPUT);
  pinMode(LED, OUTPUT);
  pinMode(Fan, OUTPUT);

}

void loop() {
   float temp = dht.readTemperature();
   float hum = dht.readHumidity(); 
   
   digitalWrite(TRIG_PIN, HIGH);
   delayMicroseconds(10);
   digitalWrite(TRIG_PIN, LOW);
   duration_us = pulseIn(ECHO_PIN, HIGH);
   distance_cm = 0.017 * duration_us;
   if (distance_cm >= 15)
   {
    digitalWrite(LED, HIGH);
   }
   else
   {
    digitalWrite(LED, LOW);
   }

   if (temp >= 30)
   {
    digitalWrite(Fan, LOW);
   }
   else
   {
    digitalWrite(Fan, HIGH);
   }

   Serial.print("distance: ");
   Serial.print(distance_cm);
   Serial.println(" cm");
   Serial.println(temp);
   Serial.println(hum);
   

 if(WiFi.status()== WL_CONNECTED){   //Check WiFi connection status

   HTTPClient http;   

   http.begin("https://innohack.pythonanywhere.com/mysite/sensordata");
   http.addHeader("Content-Type", "application/json");  
   String httpRequestData = "{\"value1\":\"" + String(temp) + "\",\"value2\":\"" + String(hum) + "\",\"value3\":\"" + String(distance_cm) + "\"}";
   int httpResponseCode = http.POST(httpRequestData);   

   if(httpResponseCode>0){

    Serial.println(httpResponseCode); 

   }else{

    Serial.println("Error on sending POST");

   }

   http.end();  //Free resources

 }else{

    Serial.println("Error in WiFi connection");   

 }

  delay(2000);  //Send a request every 10 seconds

}
