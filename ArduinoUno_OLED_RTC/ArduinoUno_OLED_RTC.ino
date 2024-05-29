#include <Wire.h>
#include <RTClib.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// OLED display parameters
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
#define SCREEN_ADDRESS 0x3C  // Use 0x3D if 0x3C doesn't work

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

RTC_DS1307 rtc;

// Dummy battery percentage for display purposes
int batteryPercentage = 75;

void setup() {
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }
  display.display();
  delay(2000);
  display.clearDisplay();

  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }

  if (!rtc.isrunning()) {
    Serial.println("RTC is NOT running, let's set the time!");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
}

void printTwoDigits(int number) {
  if (number < 10) {
    display.print('0');
  }
  display.print(number);
}

void drawBattery(int percentage) {
  int width = 20;
  int height = 8;
  int x = SCREEN_WIDTH - width - 2;
  int y = 2;

  // Draw battery outline
  display.drawRect(x, y, width, height, SSD1306_WHITE);
  display.drawRect(x + width, y + 2, 2, height - 4, SSD1306_WHITE);

  // Draw battery level
  int fillWidth = (percentage * (width - 4)) / 100;
  display.fillRect(x + 2, y + 2, fillWidth, height - 4, SSD1306_WHITE);
}

void loop() {
  DateTime now = rtc.now();

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  // Time string
  String timeString = String(now.hour()) + ":";
  if (now.minute() < 10) timeString += '0';
  timeString += String(now.minute()) + ":";
  if (now.second() < 10) timeString += '0';
  timeString += String(now.second());

  // Date string
  String dateString = String(now.year()) + "/";
  if (now.month() < 10) dateString += '0';
  dateString += String(now.month()) + "/";
  if (now.day() < 10) dateString += '0';
  dateString += String(now.day());

  // Day of the week
  const char* daysOfTheWeek[] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
  String dayString = daysOfTheWeek[now.dayOfTheWeek()];

  // Calculate the width of the strings in pixels
  int16_t x1, y1;
  uint16_t width, height;

  display.getTextBounds(timeString, 0, 0, &x1, &y1, &width, &height);
  int16_t xTime = (SCREEN_WIDTH - width) / 2;
  int16_t yTime = (SCREEN_HEIGHT / 2) - height - 8;

  display.getTextBounds(dateString, 0, 0, &x1, &y1, &width, &height);
  int16_t xDate = (SCREEN_WIDTH - width) / 2;
  int16_t yDate = (SCREEN_HEIGHT / 2);

  display.getTextBounds(dayString, 0, 0, &x1, &y1, &width, &height);
  int16_t xDay = (SCREEN_WIDTH - width) / 2;
  int16_t yDay = yDate + height + 8;

  // Draw battery
  drawBattery(batteryPercentage);

  // Draw time, date, and day
  display.setCursor(xTime, yTime);
  display.print(timeString);

  display.setCursor(xDate, yDate);
  display.print(dateString);

  display.setCursor(xDay, yDay);
  display.print(dayString);

  display.display();
  delay(1000);
}
