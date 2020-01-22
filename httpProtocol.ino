// This example retrieves last value of a variable from the Ubidots API
// using HTTP protocol.

/****************************************
 * Include Libraries
 ****************************************/

#include "Ubidots.h"
#include "DHT.h"


#define DHTPIN 0     // Digital pin connected to the DHT sensor
#define DHTTYPE DHT22
/****************************************
 * Define Constants
 ****************************************/

const char* UBIDOTS_TOKEN = "BBFF-3rLH0Kb48sKtx9VjLunqx7qySqFypy";                                     // Put here your Ubidots TOKEN
const char* WIFI_SSID = "Moto";                                         // Put here your Wi-Fi SSID
const char* WIFI_PASS = "azertyazerty";                                         // Put here your Wi-Fi password
const char* DEVICE_LABEL_TO_RETRIEVE_VALUES_FROM = "device";  // Replace with your device label
const char* VARIABLE_LABEL_TO_RETRIEVE_VALUES_FROM = "switch";       // Replace with your variable label

Ubidots ubidots(UBIDOTS_TOKEN, UBI_HTTP);
DHT dht(DHTPIN, DHTTYPE);
float temperature = 0;
float humidite = 0;
float led = 0;

/****************************************
 * Auxiliar Functions
 ****************************************/

// Put here your auxiliar functions

/****************************************
 * Main Functions
 ****************************************/

void setup() {
  Serial.begin(115200);
  ubidots.wifiConnect(WIFI_SSID, WIFI_PASS);
  pinMode(LED_BUILTIN, OUTPUT);     // Initialize the LED_BUILTIN pin as an output
  dht.begin();
  //ubidots.setDebug(true); //Uncomment this line for printing debug messages
}

void loop() {
   
  /* Obtain last value from a variable as float using HTTP */
  led = ubidots.get(DEVICE_LABEL_TO_RETRIEVE_VALUES_FROM, VARIABLE_LABEL_TO_RETRIEVE_VALUES_FROM);
  
  // Evaluates the results obtained
  if (led != ERROR_VALUE) {
    Serial.print("Value:");
    if(led) {
      Serial.println("ON");
      digitalWrite(LED_BUILTIN, LOW);   // Turn the LED on 
    }
    else {
      Serial.println("OFF");
      digitalWrite(LED_BUILTIN, HIGH);   // Turn the LED off
    }
  }
   // Read temperature as Celsius (the default)
   temperature = dht.readTemperature();
   humidite = dht.readHumidity();
   
   ubidots.add("temperature", temperature);
   ubidots.add("humidite", humidite);

     // Check if any reads failed and exit early (to try again).
   if (isnan(temperature)) {
     Serial.println(F("Failed to read from DHT sensor!"));
     return;
   }
   bool bufferSent = false;
 
   bufferSent = ubidots.send(DEVICE_LABEL_TO_RETRIEVE_VALUES_FROM);
    if (bufferSent) {
     // Do something if values were sent properly
     Serial.println("Values sent by the device");
    }
  delay(1000);
}
