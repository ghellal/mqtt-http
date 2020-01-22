#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "DHT.h"

#define DHTPIN 0    // Pin sur lequel est branché le DHT
#define DHTTYPE DHT22         // DHT 22


#define temperature_topic "sensor/temperature"  //Topic température
#define humidity_topic "sensor/humidity"        //Topic humidité

const char* ssid = "Moto";
const char* password =  "azertyazerty";

const char* mqttServer = "tailor.cloudmqtt.com";
const int mqttPort =  10224;
const char* mqttUser = "xfttkvuy";
const char* mqttPassword = "lWGX5qLkI2uz";

float temperature;
float humidity;

WiFiClient espClient;
PubSubClient client(espClient);
DHT dht(DHTPIN, DHTTYPE); 
 


 
void setup() {
  Serial.begin(115200);
  setup_wifi();
  config_mqtt();
  pinMode(LED_BUILTIN, OUTPUT);

  //connexion DHT
  dht.begin();
  client.subscribe("led");
}


void config_mqtt(){
   //configuration connexion serveur MQTT + callback 
  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);
  }

void setup_wifi()
{
  //configuration connexion wifi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to the WiFi network");
}
 
void callback(char* topic, byte* payload, unsigned int length) {
 
  Serial.print("Message arrived in topic: ");
  Serial.println(topic);
 
  Serial.print("Message:");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  if((char)payload[0] == 'f') digitalWrite(LED_BUILTIN, HIGH);
  else digitalWrite(LED_BUILTIN, LOW);
 
  Serial.println();
  Serial.println("-----------------------");
 
}
 
void loop() {
  while (!client.connected()) {
    Serial.println("Connecting to MQTT...");
    //client.subscribe("led");
    
    if (client.connect("ESP8266Client", mqttUser, mqttPassword )) 
    {
     Serial.println("connected");  
    } 
    else {
      Serial.print("failed with state ");
      Serial.print(client.state());
      delay(500);
    }
  }

  temperature = dht.readTemperature();
  humidity = dht.readHumidity();

  //Serial.println(temperature);
  client.publish(temperature_topic, String(temperature).c_str(), true); 
  client.publish(humidity_topic, String(humidity).c_str(), true);  
  client.subscribe("led");
  //client.subscribe("esp/test");
  client.loop();
}
