#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include "WIFIConnector_MKR1000.h"
#include "MQTTConnector.h" 


#define PIN_LED 3     // Control signal, connect to DI of the LED
#define NUM_LED 1     // Number of LEDs in a strip

const int TOUCH_SENSOR = 6;
const int MOTOR = 2;

Adafruit_NeoPixel RGB_Strip = Adafruit_NeoPixel(NUM_LED, PIN_LED, NEO_GRB + NEO_KHZ800);

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint16_t wait) {
  for (uint16_t i = 0; i < RGB_Strip.numPixels(); i++) {
    RGB_Strip.setPixelColor(i, c);
    RGB_Strip.show();
    delay(wait);
  }
}



void setup() {
  RGB_Strip.begin();
  RGB_Strip.show();
  RGB_Strip.setBrightness(200);    // Set brightness, 0-255 (darkest - brightest)
  pinMode(TOUCH_SENSOR, INPUT);
  pinMode(MOTOR, OUTPUT);
  digitalWrite(MOTOR, LOW);

  wifiConnect();                  
  MQTTConnect(); 

}


void loop() {

  ClientMQTT.loop(); 

  appendPayload("Touche", digitalRead(TOUCH_SENSOR));
  appendPayload("Moteur", digitalRead(MOTOR));
  sendPayload();

  if(touchStatus == "true"){
    colorWipe(RGB_Strip.Color(0, 255, 0), 1000);  // Green
    digitalWrite(MOTOR, HIGH);
    appendPayload("LED", 1);

    
  }

  if(touchStatus == "false"){
    colorWipe(RGB_Strip.Color(255, 0, 0), 1000);  // Red
    digitalWrite(MOTOR, LOW);
    appendPayload("LED", 1);
  }
  delay(700);
}

