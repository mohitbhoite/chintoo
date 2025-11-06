/******************************************************/
//       THIS IS A GENERATED FILE - DO NOT EDIT       //
/******************************************************/

#include "Particle.h"
#line 1 "e:/github/chintoo/src/chintoo.ino"
/*
 * Project chintoo-weather
 * Description: Chintoo is a simple internet-connected seven-segment display that fetches time, weather, and custom notifications over Wi-Fi.
 * Author: Mohit Bhoite
 * Date: March 2020
 */

#include "neopixel.h"
 
//SYSTEM_MODE(MANUAL);
void setup();
void loop();
void gotWeatherData(const char *name, const char *data);
void showPattern(int patternNum);
void display();
#line 11 "e:/github/chintoo/src/chintoo.ino"
SYSTEM_THREAD(ENABLED);

#include "SevSeg.h"
SevSeg sevseg; //Instantiate a seven segment controller object


byte pattern[16][8] = { {B01011101,B00000000,B00000000,B01011101,B00000000},
                      {B00001001,B00000000,B00000000,B00001001,B00000000},
                      {B00111111,B10000000,B00000000,B00111111,B00000000},
                      {B01000000,B10000000,B00000000,B01000000,B00000000},
                       
                      {B00000000,B00000000,B00000000,B00000001,B00000000},
                      {B00000000,B00000000,B00000001,B00000000,B00000000},
                      {B00000000,B00000001,B00000000,B00000000,B00000000},
                      {B00000001,B00000000,B00000000,B00000000,B00000000},
                      {B00100000,B00000000,B00000000,B00000000,B00000000},
                      {B00010000,B00000000,B00000000,B00000000,B00000000},
                      {B00001000,B00000000,B00000000,B00000000,B00000000},
                      {B00000000,B00001000,B00000000,B00000000,B00000000},
                      {B00000000,B00000000,B00001000,B00000000,B00000000},
                      {B00000000,B00000000,B00000000,B00001000,B00000000},
                      {B00000000,B00000000,B00000000,B00000100,B00000000},
                      {B00000000,B00000000,B00000000,B00000010,B00000000}
                     };

byte temp[]={0x5b,0xcf,0x06,0x39};

unsigned long previousMillis = 0; 

// constants won't change:
const long interval = 60*1*1000; // Fetch weather once a minute. This is too often! Update to longer time
int counter,stateCounter=0;
int patternCount = 4;
int spinCount = 0;
bool state=true;
bool countDown = TRUE;

// IMPORTANT: Set pixel COUNT, PIN and TYPE
#define PIXEL_PIN D0
#define PIXEL_COUNT 1
#define PIXEL_TYPE WS2812B

Adafruit_NeoPixel dot(PIXEL_COUNT, PIXEL_PIN, PIXEL_TYPE);

// Prototypes for local build, ok to leave in for Build IDE
void rainbow(uint8_t wait);
uint32_t Wheel(byte WheelPos);

Timer timer(1, display);

void setup() {
    
    Particle.subscribe("hook-response/get_weather", gotWeatherData, MY_DEVICES);

    dot.begin();
    dot.show(); // Initialize all pixels to 'off'
    //rainbow(100);
    dot.setPixelColor(0,200,100,20);
    dot.show();
    
    
    byte numDigits = 4;

  
  //Chintoo
  byte digitPins[] = {TX,D7,D4,A2};//
  byte segmentPins[] = {D6,D3,RX,A4,A5,D2,A3,WKP}; //
  
  
  //byte segmentPins[] = {D0,D1,D2,D3,D4,D5,D6,D7};
  bool resistorsOnSegments = TRUE; // 'false' means resistors are on digit pins
  byte hardwareConfig = COMMON_CATHODE; // See SevSeg lib README.md for options
  bool updateWithDelays = false; // Default. Recommended
  bool leadingZeros = false; // Use 'true' if you'd like to keep the leading zeros
  
  sevseg.begin(hardwareConfig, numDigits, digitPins, segmentPins, resistorsOnSegments, updateWithDelays, leadingZeros);
  sevseg.setBrightness(100);

  timer.start();
}

void loop() {
  
  unsigned long currentMillis = millis();
   if (currentMillis - previousMillis >= interval) 
   {
    dot.setPixelColor(0,250,200,0);
    dot.show();
     for(int j=0;j<4;j++)
    {
      for(int i=4;i<16;i++)
      {
        showPattern(i);
        delay(50);
      }
    }  
    sevseg.blank();
    sevseg.setChars("SF");
    delay(1000);
    previousMillis = currentMillis;
    Particle.publish("get_weather");
   }
    
   //Scroll through all patterns
    // for(int i=0;i<12;i++)
    //     {
    //         showPattern(i);
    //         randomSeed(i);
    //         dot.setPixelColor(0, Wheel(random(0, 255) & 255));
        
    //         //dot.setPixelColor(0, 0,0,0);
    //         dot.show();
    //         delay(500);
    //     }
}

// This function will get called when weather data comes in
void gotWeatherData(const char *name, const char *data) {
        
        char buffer [4];
        //char far = "F";
        sprintf(buffer," %s%s",data,"F");
        //String tampan = datat+String("F");
        int t = atoi(data);
        //sevseg.setNumber(t*100,2);
        dot.setPixelColor(0,200,0,200);
        dot.show();
        sevseg.setChars(buffer);
        Particle.publish(buffer);

}

void showPattern(int patternNum)
{
    sevseg.setSegments(pattern[patternNum]);
}

void rainbow(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<dot.numPixels(); i++) {
      dot.setPixelColor(i, Wheel((i+j) & 255));
    }
    dot.show();
    delay(wait);
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  if(WheelPos < 85) {
   return dot.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if(WheelPos < 170) {
   WheelPos -= 85;
   return dot.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
   WheelPos -= 170;
   return dot.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}

void display()
{
    sevseg.refreshDisplay();
}
/// END ///