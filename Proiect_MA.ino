const int LED1 = D5;
const int LED2 = D4;
const int LED3 = D3;
const int LED4 = D2;
const int LED5 = D1;
const int MOTOR = D0;
const int buttonHUM=D6;
const int buttonLIGHT=D7;

const int sense_Pin = A0; // sensor input at Analog pin A0

int value = 0;
int buttonHUM_value = 0;
int s = 0;
int sm = 0;

void LED_PRINT_LEVEL(int level) //levels shows number of divisions to turn on
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

void setup() {
   Serial.begin(115200);
   Serial.println("The board has started");
   pinMode(LED1, OUTPUT);
   pinMode(LED2, OUTPUT);
   pinMode(LED3, OUTPUT);
   pinMode(LED4, OUTPUT);
   pinMode(LED5, OUTPUT);
   pinMode(MOTOR, OUTPUT);
   pinMode(buttonHUM, INPUT_PULLUP);
   pinMode(buttonLIGHT, INPUT_PULLUP);
   delay(2000);
}

void loop() {
   Serial.print("MOISTURE LEVEL : ");
   value= analogRead(sense_Pin);
   buttonHUM_value= digitalRead(buttonHUM);
   Serial.println(value);
   digitalWrite(LED1, LOW);
   digitalWrite(LED2, LOW);
   digitalWrite(LED3, LOW);
   digitalWrite(LED4, LOW);
   digitalWrite(LED5, LOW);
   if(value>950)
        {
            if (buttonHUM_value==0) LED_PRINT_LEVEL(1);
            if (sm>10){
              digitalWrite(MOTOR,HIGH);
              s=1;
              sm=0;
            }
            else sm++;
        }
    else if (value>900)
        {
            if (buttonHUM_value==0) LED_PRINT_LEVEL(2);
        }
    else if (value>850)
        {
            if (buttonHUM_value==0) LED_PRINT_LEVEL(3);
        }
    else if (value>800)
        {
            if (buttonHUM_value==0) LED_PRINT_LEVEL(4);
        }
    else
        {
            if (buttonHUM_value==0) LED_PRINT_LEVEL(5);
        }
        
    if (s>0)
    {
       if(s>5){
        digitalWrite(MOTOR,LOW);
        s=0;
        sm=0;
       }
       else s++;
    }
    delay(1000);
}                               
