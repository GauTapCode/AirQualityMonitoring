#include <ESP8266WiFi.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "MQ135.h"

Adafruit_SSD1306 display(128, 32, &Wire, -1);

// Define Pin on ESP
const int pushButton = 2;     
const int ledPin =  0;        
          
int displayState = 0;

// Data of temp icon
static const unsigned char PROGMEM drop_iconTemp[] = {
  B00000011, B10000000,
  B00000110, B11000000,
  B00000100, B01000000,
  B00000101, B01000000,
  B00000101, B01000000,
  B00000101, B01000000,
  B00000101, B01000000,
  B00000101, B01000000,
  B00001101, B01100000,
  B00001001, B00100000,
  B00011011, B10110000,
  B00010011, B10010000,
  B00011011, B10110000,
  B00001000, B00100000,
  B00001110, B11100000,
  B00000011, B10000000
};

static const unsigned char PROGMEM drop_iconHud[] = {
  B00000010, B00000000,
  B00000110, B00000000,
  B00001111, B00000000,
  B00011111, B10000000,
  B00011111, B10000000,
  B00111111, B11000000,
  B00111111, B11000000,
  B01111111, B11100000,
  B01111111, B11100000,
  B01111111, B11100000,
  B01111111, B11100000,
  B01111110, B11100000,
  B01111100, B01100000,
  B00111110, B11000000,
  B00011111, B10000000,
  B00001111, B00000000
};

static const unsigned char PROGMEM drop_iconMq135[] = {
  B00000000, B00000000,
  B00001110, B00111000,
  B00011111, B01111100,
  B00111111, B11111110,
  B01111111, B11111111,
  B01111110, B00111111,
  B01111110, B00111111,
  B01111000, B00001111,
  B01111000, B00001111,
  B00111000, B00001110,
  B00011110, B00111100,
  B00001110, B00111000,
  B00000111, B11110000,
  B00000011, B11100000,
  B00000001, B11000000,
  B00000000, B10000000
};

void setup() 
{
  Serial.begin(115200);

  //button setup
  pinMode(pushButton, INPUT);  
  pinMode(ledPin, OUTPUT);     

  displayState = 0;

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  
  display.display();
}

void debounce_button()
{
  //buttonState = digitalRead(pushButton);      
  if (!digitalRead(pushButton))
  {
    delay(20);
    if (!digitalRead(pushButton))
    {
      
      displayState++;
      if(displayState==4) displayState=0;
      

      while(!digitalRead(pushButton));
    }
  }
}


void loop() 
{
  MQ135 gasSensor = MQ135(A0);
  float air_quality = gasSensor.getPPM();

  debounce_button();
  
  //OFF the oled
  if (displayState == 0) 
  {
    //Led check
    digitalWrite(ledPin,LOW);

    //Display on Oled 
    display.clearDisplay(); 
    display.display();
  }  

  //Display Temperature 
  if (displayState == 1) 
  {
    //Led check
    digitalWrite(ledPin,HIGH);

    display.clearDisplay(); 

    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(35, 0); ///x,y
    display.println("TEMPERATURE");
    display.drawBitmap(40, 15, drop_iconTemp, 16, 16, WHITE); 
    display.setCursor(55, 20); ///x,y
    display.println(":777");
    display.display(); 
  }  

  //Display Hudmidity 
  if (displayState == 2) 
  {
    //Led check
    digitalWrite(ledPin,LOW);

    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(40, 0); ///x,y
    display.println("HUMIDITY"); 
    display.drawBitmap(40, 15, drop_iconHud, 16, 16, WHITE); 
    display.setCursor(55, 20); ///x,y
    display.println(":777");
    display.display(); 
  }  

  //Display air quality  
  if (displayState == 3) 
  {
    digitalWrite(ledPin,HIGH);

    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(30, 0); ///x,y
    display.println("AIR QUALITY"); 
    display.drawBitmap(20, 15, drop_iconMq135, 16, 16, WHITE); 
    display.setCursor(40, 20); ///x,y 
    display.print(": ");

    display.print(air_quality);
    display.setTextSize(1);
    //display.setCursor(60, 30);
    display.println(" PPM");
    display.display();
    delay(500);
  }  


}