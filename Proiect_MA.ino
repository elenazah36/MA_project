const int LED1 = D5;
const int LED2 = D4;
const int LED3 = D3;
const int LED4 = D2;
const int LED5 = D1;
const int MOTOR = D0;            //switch for pump
const int buttonHUM=D6;          //button for soil moisture sensor display
const int buttonLIGHT=D7;        //button for photoresistor display
const int sensorSELECT=D8;       //sensor selector (0-photoresistor, 1-soil moisture)
const int sense_Pin = A0;        //sensor input at Analog pin A0

const int MEASUREMENT_INT=10000;  //time interval between two measurements
const int WATER_INT=10000;       //time interval between two waterings
const int WATER_DUR=5000;        //duration of watering
int moisture_level=0;
int light_level=0;

int current_ms=0;
int measurement_ms=0;
int pump_on_ms=0;
int pump_off_ms=0;
char pump_state=0;

int light=0, moisture=0;
int buttonHUM_value = 0;
int buttonLIGHT_value = 0;

////////////////////////////  WI-FI CONFIGURATION  //////////////////////////////////

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

const char* ssid     = "xxxxx";
const char* password = "xxxxx";
const char* host = "x.x.x.x"; // IPv4 adress
const int   port = xxxx;     //port Domoticz
const int   watchdog = 6000; // Frequency of sending data to Domoticz
unsigned long previous_ms = millis(); 
HTTPClient http;
String url1, url2;


////////////////////////////  LED_PRINT_LEVEL  /////////////////////////////////////

void LED_PRINT_LEVEL(int level) //turns on number of LED dividions according to "level"
{
  switch(level)
  {
    case 1:
    {
       digitalWrite(LED1, HIGH);
    }
    break;
    case 2:
    {
       digitalWrite(LED1, HIGH);
       digitalWrite(LED2, HIGH);
    }
    break;
    case 3:
    {
       digitalWrite(LED1, HIGH);
       digitalWrite(LED2, HIGH);
       digitalWrite(LED3, HIGH);
    }
    case 4:
    {
       digitalWrite(LED1, HIGH);
       digitalWrite(LED2, HIGH);
       digitalWrite(LED3, HIGH);
       digitalWrite(LED4, HIGH);
    }
    break;
    case 5:
    {
       digitalWrite(LED1, HIGH);
       digitalWrite(LED2, HIGH);
       digitalWrite(LED3, HIGH);
       digitalWrite(LED4, HIGH);
       digitalWrite(LED5, HIGH);
    }
  }
    
}


////////////////////////////////  READ_SENSOR  ////////////////////////////////////

void READ_SENSORS()       // duration of MEASUREMENT_INT+1000ms
{
   if (current_ms - measurement_ms == MEASUREMENT_INT - 1000){        //powers moisture sensor 1 s before the measurement
      digitalWrite(sensorSELECT, HIGH);                                
      delay(1);
   }
   else if (current_ms - measurement_ms == MEASUREMENT_INT)            //moisture sensor measurement
   {
      Serial.print("MOISTURE VALUE: ");
      moisture = analogRead(sense_Pin);
      Serial.println(moisture);
      digitalWrite(sensorSELECT, LOW);                                 //switch to photoresistor
      delay(1);
   }
   else if (current_ms - measurement_ms == MEASUREMENT_INT + 1000){   //light sensor measurement
      Serial.print("LIGHT VALUE: ");
      light = analogRead(sense_Pin);
      Serial.println(light);
      measurement_ms=current_ms;                                      //update time of last measurement
      delay(1);
   }
   
}



///////////////////////////////////  WATER  ////////////////////////////////////////

void WATER(){
    if((current_ms - pump_off_ms > WATER_INT) && moisture_level==1 && pump_state==0){       //need watering
      digitalWrite(MOTOR,HIGH);
      pump_on_ms=current_ms;
      pump_state=1;
    }
    if (current_ms - pump_on_ms > WATER_DUR and pump_state==1){
      digitalWrite(MOTOR,LOW);
      pump_off_ms=current_ms;
      pump_state=0;
    }
}


//////////////////////////// GET_MOISTURE_LEVEL ////////////////////////////////////

int GET_MOISTURE_LEVEL(){
  if(moisture>800) return 1;
  if(moisture>600) return 2;
  if(moisture>400) return 3;
  if(moisture>200) return 4;
  return 5;
}


///////////////////////////// GET_LIGHT_LEVEL //////////////////////////////////////

int GET_LIGHT_LEVEL(){
  if(light>700) return 5;
  if(light>500) return 4;
  if(light>300) return 3;
  if(light>100) return 2;
  return 1;
}


///////////////////////////// CONNECTING TO SERVER //////////////////////////////////

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


////////////////////////////////// SETUP ///////////////////////////////////////////

void setup() {
   Serial.begin(115200);
   Serial.println("The board has started");

   ////////CONFIGURAION OF PINS////////
   pinMode(LED1, OUTPUT);
   pinMode(LED2, OUTPUT);
   pinMode(LED3, OUTPUT);
   pinMode(LED4, OUTPUT);
   pinMode(LED5, OUTPUT);
   pinMode(MOTOR, OUTPUT);
   pinMode(buttonHUM, INPUT_PULLUP);
   pinMode(buttonLIGHT, INPUT_PULLUP);
   pinMode(sensorSELECT, OUTPUT);

   delay(2000);
   
   current_ms=millis();
   //measurement_ms=current_ms;
   //pump_on_ms=current_ms;
   //pump_off_ms=current_ms;
  
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

///////////////////////// MAIN  ////////////////////////////////////
void loop() {
   digitalWrite(LED1, LOW);
   digitalWrite(LED2, LOW);
   digitalWrite(LED3, LOW);
   digitalWrite(LED4, LOW);
   digitalWrite(LED5, LOW);
   
   current_ms=millis();
   
   READ_SENSORS();
  
  if ( current_ms - previous_ms > watchdog )   //hope this doesn't interfere with the rest of the code, otherwise I'll take seperate variables
  {
    previous_ms = current_ms;
    if(WiFi.status() != WL_CONNECTED)
      {  
          Serial.println("WiFi not connected !");
      } 
    else 
      { 
      
         Serial.println("Send data to Domoticz");

         url1 = "/json.htm?type=command&param=udevice&idx=1&nvalue=0&svalue=";  //request command
         url1 += String(light); 
         url1 += ";";
         sendDomoticz(url1);


          url2 = "/json.htm?type=command&param=udevice&idx=3&nvalue=0&svalue="; 
          url2 += String(moisture); 
          url2 += ";";
          sendDomoticz(url2);

       }
    }

   moisture_level=GET_MOISTURE_LEVEL();
   light_level=GET_LIGHT_LEVEL();
   
   buttonHUM_value = digitalRead(buttonHUM);
   buttonLIGHT_value = digitalRead(buttonLIGHT);
   
   if (buttonHUM_value==0) LED_PRINT_LEVEL(moisture_level);              // show moisture level on leds if button is pressed
   if (buttonLIGHT_value==0) LED_PRINT_LEVEL(light_level);               // show light level on leds if button is pressed
   
   WATER();
   
}                               
