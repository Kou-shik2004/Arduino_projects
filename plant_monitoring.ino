//soil moisture A0 virtual pin 3
//water pump D5 virtual pin 5
//dht11 D6 temp v6 humid v7


#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#define BLYNK_TEMPLATE_ID "TMPLT8QA_YjX"
#define BLYNK_TEMPLATE_NAME "Plant monitoring"
#define BLYNK_AUTH_TOKEN "3KZcCIYv06lwouTQBzZtef-Zt8w1djKR"
#define BLYNK_FIRMWARE_VERSION        "1.2.0"
#define ESP8266_BAUD 115200
#include <DHT.h>

char auth[]= BLYNK_AUTH_TOKEN;
char ssid[]= "harish";
char pass[]= "12345678";
int water,prevState;
int relaystate=LOW;
float t,h;

BlynkTimer timer;
#define soil A0
#define relay_pin D5
#define dht11 D6
DHT dht(dht11, DHT11);

void setup() {
  Serial.begin(115200);
  delay(100);


 pinMode(relay_pin, OUTPUT);
 pinMode(soil, INPUT);
 digitalWrite(relay_pin,relaystate);//relay has reversed connections

  Blynk.begin(auth, ssid, pass,IPAddress(68,183,87,221),8080); //68.183.87.221


  timer.setInterval(100L, soilMoistureSensor);
  timer.setInterval(100L, DHT11sensor);

}

void soilMoistureSensor() {
  int value = water;
  value = map(value, 0, 1024, 0, 100);
  value = (value - 100) * -1;

  Blynk.virtualWrite(V3, value);


}
void DHT11sensor() {
  float H = h;
  float T = t;

  if (isnan(H) || isnan(T)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  Blynk.virtualWrite(V6, T);
  Blynk.virtualWrite(V7, H);
}
BLYNK_WRITE(V5){
  relaystate= param.asInt();
  digitalWrite(relay_pin,relaystate);
}
BLYNK_CONNECTED(){
  Blynk.syncVirtual(V5);
}
void loop()
{ 
  

 water=analogRead(soil);
 h=dht.readHumidity();
 t=dht.readTemperature();
 Serial.println(water);
 delay(1000);

 Blynk.run();
  timer.run();

  if (water == 1024 && prevState == 0) 
  {
  Serial.println("Pump Started, Water Flowing");
  relaystate=!relaystate;
  digitalWrite(relay_pin,relaystate);//polarities reversed for relay
  Blynk.virtualWrite(V5, relaystate);
  
  prevState = 1;
  delay(400);    
  }     
  
  else if (water == 1024 && prevState == 1)
  {
  
  delay(400);
  }
  else 
  { 
  digitalWrite(relay_pin,relaystate);
  Blynk.virtualWrite(V5, relaystate);
  prevState =0;
  }
  
  
  delay(100);

}