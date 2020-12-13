const int LED1 = D5;
const int LED2 = D4;
const int LED3 = D3;
const int LED4 = D2;
const int LED5 = D1;
const int MOTOR = D0;   //switch for pump
const int buttonHUM=D6; //button for soil moisture sensor display
const int buttonLIGHT=D7;  //button for photoresistor display
const int sensorSELECT=D8;  //sensor selector (0-photoresistor, 1-soil moisture)
const int sense_Pin = A0; // sensor input at Analog pin A0


const int S=5; // duration of watering
const int SM=10; // time interval between waterings
const int TIME_UNIT=2000; //time unit of one cicle (ms)
const int SENSOR_READ_TIME=1000;


int light=0, moisture=0;
int buttonHUM_value = 0;
int s = 0;  //duration of watering index
int sm = 0; //time interval between waterings index


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

void READ_SENSORS()       // contains 2 x SENSOR_READ_TIME delay
{
   digitalWrite(sensorSELECT, HIGH);
   delay(SENSOR_READ_TIME);
   Serial.print("MOISTURE VALUE: ");
   moisture = analogRead(sense_Pin);
   Serial.println(moisture);
   
   digitalWrite(sensorSELECT, LOW);
   delay(SENSOR_READ_TIME);
   Serial.print("LIGHT VALUE: ");
   light = analogRead(sense_Pin);
   Serial.println(light);
}


////////////////////////// GET_MOISTURE_LEVEL ////////////////////////////////////

int GET_MOISTURE_LEVEL(){
  if(moisture>800) return 1;
  if(moisture>600) return 2;
  if(moisture>400) return 3;
  if(moisture>200) return 4;
  return 5;
}


////////////////////////// GET_LIGHT_LEVEL ////////////////////////////////////

int GET_LIGHT_LEVEL(){
  return 0;
}


//////////////////////////////// SETUP /////////////////////////////////////////

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
}



///////////////////////// MAIN  ////////////////////////////////////
void loop() {
   digitalWrite(LED1, LOW);
   digitalWrite(LED2, LOW);
   digitalWrite(LED3, LOW);
   digitalWrite(LED4, LOW);
   digitalWrite(LED5, LOW);
   digitalWrite(sensorSELECT, LOW);

   int moisture_level=GET_MOISTURE_LEVEL(moisture);
   
   buttonHUM_value = digitalRead(buttonHUM);
   if (buttonHUM_value==0) LED_PRINT_LEVEL(moisture_level);  // show moisture level on leds if button is pressed
   
   if(moisture_level==1){   //need watering
      if (sm>SM){                 //turns on the pump if a certain time has passed since last watering
        digitalWrite(MOTOR,HIGH);
        s=1;
        sm=0;                
        }
      else sm++;
    }
        
    if (s>0) 
    {
       if(s>S){
        digitalWrite(MOTOR,LOW);
        s=0;
        sm=0;
       }
       else s++;
    }
    READ_SENSORS();
    delay(TIME_UNIT-2*SENSOR_READ_TIME);
}                               
