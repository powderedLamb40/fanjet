#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define OLED_I2C_ADDRESS 0x3C

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

#define ESC_PIN 2
#define BUTTON_UP 1   
#define BUTTON_DOWN 0

int speedPercent = 0;          // ค่าเปอร์เซ็นต์ความเร็ว
const int speedStep = 5;       // ระดับการเพิ่ม/ลด %
const int maxSpeed = 100;      // ความเร็วสูงสุดใน %
const int minSpeed = 0;        // ความเร็วต่ำสุดใน %
const int pwmMin = 870;        // ค่า PWM ต่ำสุด
const int pwmMax = 2118;       // ค่า PWM สูงสุด

unsigned long lastButtonPress = 0; // เวลาที่กดปุ่มล่าสุด
const unsigned long debounceDelay = 150; // ระยะเวลา debounce

void setup() {
  Serial.begin(115200);
  Wire.begin(20, 21);

  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_I2C_ADDRESS)) {
    Serial.println(F("failed"));
    for (;;);
  }
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  pinMode(ESC_PIN, OUTPUT);
  pinMode(BUTTON_UP, INPUT);   // ตั้งค่าพินปุ่มเพิ่มความเร็วเป็น INPUT
  pinMode(BUTTON_DOWN, INPUT); // ตั้งค่าพินปุ่มลดความเร็วเป็น INPUT
  
  ledcSetup(0, 61, 14);
  ledcAttachPin(ESC_PIN, 0);
  ledcWrite(0, map(speedPercent, 0, 100, pwmMin, pwmMax)); // เขียนค่าเริ่มต้น PWM

  displaySpeed();
}

void loop() {
  unsigned long currentMillis = millis();

  // อ่านสถานะของปุ่มเพิ่มความเร็ว
  if (digitalRead(BUTTON_UP) == HIGH && (currentMillis - lastButtonPress) > debounceDelay) {
    lastButtonPress = currentMillis;
    if (speedPercent < maxSpeed) {
      speedPercent += speedStep;
      if (speedPercent > maxSpeed) speedPercent = maxSpeed;
      ledcWrite(0, map(speedPercent, 0, 100, pwmMin, pwmMax)); // แปลงค่าเป็น PWM
      displaySpeed();
    }
  }

  // อ่านสถานะของปุ่มลดความเร็ว
  if (digitalRead(BUTTON_DOWN) == HIGH && (currentMillis - lastButtonPress) > debounceDelay) {
    lastButtonPress = currentMillis;
    if (speedPercent > minSpeed) {
      speedPercent -= speedStep;
      if (speedPercent < minSpeed) speedPercent = minSpeed;
      ledcWrite(0, map(speedPercent, 0, 100, pwmMin, pwmMax)); // แปลงค่าเป็น PWM
      displaySpeed();
    }
  }
}

// ฟังก์ชันแสดงค่าความเร็วบนหน้าจอ OLED
void displaySpeed() {
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println(F("ESC Speed Controller By PP"));
  display.setCursor(0, 20);
  display.print(F("Speed: "));
  display.print(speedPercent);
  display.println(F(" %"));

  // วาดแถบบาร์ความเร็ว
  int barWidth = map(speedPercent, 0, 100, 0, SCREEN_WIDTH); // คำนวณความกว้างของแถบบาร์
  display.fillRect(0, 30, barWidth, 5, SSD1306_WHITE); // วาดแถบบาร์

  display.display();
}
