#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

const char* ssid     = "XXXXXX";
const char* password = "XXXXXX";
const char* host = "XXX.XXX.XXX.XXX"; // IPv4 adress
const int   port = XXXXX;     //port Domoticz
const int   watchdog = 6000; // Frequency of sending data to Domoticz
unsigned long previousMillis = millis(); 
HTTPClient http;
String url;

void setup() 
{
  Serial.begin(9600);// initialize serial communication at 9600 BPS
   delay(10);
  Serial.println("Connecting Wifi...");

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)   //connecting to wifi
    {
       delay(500);
       Serial.print(".");                                     
       Serial.println("");
       Serial.println("WiFi connected");
       Serial.println("IP address: ");
      Serial.print(WiFi.localIP()); 
    }
 }



void loop() 
{
  unsigned long currentMillis = millis();

  if ( currentMillis - previousMillis > watchdog ) 
  {
    previousMillis = currentMillis;
    if(WiFi.status() != WL_CONNECTED)
      {  
        Serial.println("WiFi not connected !");
      } 
    else 
      { Serial.println("Send data to Domoticz");
        int sensorValue = analogRead(A0);   // read the input on analog pin 0
        float output = sensorValue * (100 / 1023.0);   // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 100V)
    
        url = "/json.htm?type=command&param=udevice&idx=1&nvalue=0&svalue=";  //request command
        url += String(output); url += ";";
        sendDomoticz(url);
       }
    }
}


void sendDomoticz(String url)      //updating data
{
  Serial.print("connecting to ");
  Serial.println(host);
  Serial.print("Requesting URL: ");
  Serial.println(url);
  http.begin(host,port,url);
  int httpCode = http.GET();
    if (httpCode)
    { if(httpCode == 200)
     {
        String payload = http.getString();
        Serial.println("Domoticz response "); 
        Serial.println(payload);
      
    }
    }
  else Serial.println("Not connected to Domoticz");
  Serial.println (httpCode);
  Serial.println("closing connection");
  http.end();
}
