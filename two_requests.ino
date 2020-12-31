#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

const char* ssid     = "HUAWEI-sM8x";
const char* password = "uESQ6xDn";
const char* host = "192.168.100.9"; // IPv4 adress
const int   port = 8787;     //port Domoticz
const int   watchdog = 6000; // Frequency of sending data to Domoticz
unsigned long previousMillis = millis(); 
HTTPClient http;
String url1, url2;

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
      
        //int sensorValue = analogRead(A0);   // read the input on analog pin 0
        //float light = sensorValue * (100 / 1023.0);   // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 100V)
        //float moisture = light/10;
    
       url1 = "/json.htm?type=command&param=udevice&idx=1&nvalue=0&svalue=";  //request command
       url1 += String(light); 
       url1 += ";";
       sendDomoticz(url1);
   

        url2 = "/json.htm?type=command&param=udevice&idx=2&nvalue=0&svalue="; 
        url2 += String(moisture); 
        url2 += ";";
        sendDomoticz(url2);
   
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
