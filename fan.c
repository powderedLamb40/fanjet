#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <ESP32Servo.h>

// กำหนดขนาดหน้าจอ OLED
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define OLED_I2C_ADDRESS 0x3C

// กำหนดการเชื่อมต่อจอ OLED
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// กำหนดพินสำหรับ ESC และปุ่ม
#define ESC_PIN 2
#define BUTTON_UP 0   // พินสำหรับปุ่มเพิ่มความเร็ว
#define BUTTON_DOWN 1 // พินสำหรับปุ่มลดความเร็ว

Servo esc;              // สร้างออบเจ็กต์ Servo สำหรับควบคุม ESC
int speed = 1000;       // ค่าความเร็วเริ่มต้น (หน่วยไมโครวินาที)
const int maxSpeed = 2000; // ค่าความเร็วสูงสุด
const int minSpeed = 1000; // ค่าความเร็วต่ำสุด

void setup() {
  // เริ่มต้น Serial Monitor
  Serial.begin(115200);

  // เริ่มต้นหน้าจอ OLED
  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_I2C_ADDRESS)) { // ใช้ address 0x3C แทน SSD1306_I2C_ADDRESS
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  // กำหนด ESC_PIN ให้ควบคุม ESC
  esc.attach(ESC_PIN, minSpeed, maxSpeed);

  // เริ่มต้น ESC ด้วยค่าความเร็วต่ำสุด
  esc.writeMicroseconds(minSpeed);

  // ตั้งค่าพินสำหรับปุ่ม
  pinMode(BUTTON_UP, INPUT_PULLUP);   // ใช้ Pull-up สำหรับปุ่ม
  pinMode(BUTTON_DOWN, INPUT_PULLUP);

  // แสดงข้อความเริ่มต้น
  displaySpeed();
}

void loop() {
  // อ่านสถานะของปุ่มเพิ่มความเร็ว
  if (digitalRead(BUTTON_UP) == LOW) {
    if (speed < maxSpeed) {
      speed += 100; // เพิ่มความเร็วทีละ 50 ไมโครวินาที
      esc.writeMicroseconds(speed); // อัปเดต ESC ด้วยค่าความเร็ว
      displaySpeed(); // อัปเดตหน้าจอ
    }
    delay(100); // หน่วงเวลาเพื่อลดความไวของปุ่ม
  }

  // อ่านสถานะของปุ่มลดความเร็ว
  if (digitalRead(BUTTON_DOWN) == LOW) {
    if (speed > minSpeed) {
      speed -= 50; // ลดความเร็วทีละ 50 ไมโครวินาที
      esc.writeMicroseconds(speed); // อัปเดต ESC ด้วยค่าความเร็ว
      displaySpeed(); // อัปเดตหน้าจอ
    }
    delay(100); // หน่วงเวลาเพื่อลดความไวของปุ่ม
  }
}

// ฟังก์ชันแสดงค่าความเร็วบนหน้าจอ OLED
void displaySpeed() {
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println(F("ESC Speed Controller"));
  display.setCursor(0, 20);
  display.print(F("Speed: "));
  display.print(speed);
  display.println(F(" us"));
  display.display();
}
