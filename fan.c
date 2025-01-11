#include <Arduino.h>


#define ESC_PIN 4
#define BUTTON_UP 0   
#define BUTTON_DOWN 1 

int speed = 0;         
const int maxSpeed = 2000; 
const int minSpeed = 1000; 

void setup() {
  // ตั้งค่า ESC
  ledcAttachPin(ESC_PIN, 0); 
  ledcSetup(0, 50, 16);      
  
  
  ledcWrite(0, minSpeed);

  
  pinMode(BUTTON_UP, INPUT_PULLUP);   
  pinMode(BUTTON_DOWN, INPUT_PULLUP);
}

void loop() {
  
  if (digitalRead(BUTTON_UP) == LOW) {
    if (speed < maxSpeed) {
      speed += 10;
    }
    delay(100); 
  }

  if (digitalRead(BUTTON_DOWN) == LOW) {
    if (speed > minSpeed) {
      speed -= 10; 
    }
    delay(100); 
  }

  ledcWrite(0, speed);
}
