// ==========================================
// Hardware Components
// ------------------------------------------
// Development Board: ESP8266 ESP-12F CH340G V2 (WeMos D1 Mini)
// GPS Module: NEO-6M GPS Module (GY-NEO6MV2)
// OLED Display: 0.96" I2C SSD1306 OLED (128x64)
// ==========================================

#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <TinyGPS++.h>
#include <SoftwareSerial.h>

// OLED Display Configuration
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C  // I2C address for OLED screen

Adafruit_SSD1306 display(OLED_RESET);  // OLED display object

// GPS Configuration
#define GPS_RX 13  // GPIO13 (D7)
#define GPS_TX 15  // GPIO15 (D8)

SoftwareSerial gpsSerial(GPS_RX, GPS_TX);  // GPS serial communication
TinyGPSPlus gps;  // GPS object

// Bucharest DST settings (UTC +2 or +3 depending on DST)
#define DST_START_MONTH 3  // March
#define DST_END_MONTH 10   // October
#define DST_START_DAY 31   // Last Sunday of March
#define DST_END_DAY 31     // Last Sunday of October

void setup() {
  Serial.begin(9600);
  gpsSerial.begin(9600);  // Start GPS communication

  // Initialize OLED display
  display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);  // Initialize display
  display.clearDisplay();
  display.display();
  delay(2000);
}

void loop() {
  boolean newData = false;

  // Read GPS data for 1 second
  for (unsigned long start = millis(); millis() - start < 1000;) {
    while (gpsSerial.available()) {
      if (gps.encode(gpsSerial.read())) {
        newData = true;
      }
    }
  }

  if (newData) {
    newData = false;
    print_gps_data();
  } else {
    display.clearDisplay();
    display.setCursor(0, 0);
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.println("No");
    display.println("Data");
    display.display();
    Serial.println("No GPS data received.");
  }
}

void print_gps_data() {
  display.clearDisplay();
  display.setTextColor(WHITE);

  if (gps.location.isValid()) {
    // Print Speed
    display.setCursor(0, 0);
    display.setTextSize(2);
    int speed_value = gps.speed.kmph();
    display.print(speed_value);
    display.setCursor(40, 0);
    display.setTextSize(1);
    display.print("km/h");

    // Print Satellites
    display.setCursor(0, 20);
    display.setTextSize(1);
    display.print("SAT: ");
    display.print(gps.satellites.value());

    // Print Altitude
    display.setCursor(0, 30);
    display.print("ALT: ");
    display.print(gps.altitude.meters(), 0);

    // Print Time (adjusted for Bucharest time zone and DST)
    display.setCursor(0, 40);
    if (gps.time.isValid()) {
      int hour = gps.time.hour();
      int minute = gps.time.minute();
      int second = gps.time.second();

      // Adjust for Bucharest base time (UTC+2)
      hour += 2;
      if (hour >= 24) {
        hour -= 24;
      }

      // Then adjust for DST if applicable (adds +1 hour to UTC+2 to become UTC+3)
      if (isDST(gps.date.year(), gps.date.month(), gps.date.day())) {
        hour += 1;
        if (hour >= 24) {
          hour -= 24;
        }
      }

      display.print(hour);
      display.print(":");
      display.print(minute);
      display.print(":");
      display.print(second);
    } else {
      display.print("Time: Invalid");
    }

    display.display();

    // Print GPS data to Serial Monitor
    Serial.print("\rLat: ");
    Serial.print(gps.location.lat(), 6);
    Serial.print(" | Lng: ");
    Serial.print(gps.location.lng(), 6);
    Serial.print(" | Speed: ");
    Serial.print(gps.speed.kmph());
    Serial.print(" km/h | Alt: ");
    Serial.print(gps.altitude.meters());
    Serial.print(" m | SAT: ");
    Serial.print(gps.satellites.value());
    Serial.print(" | Date: ");
    Serial.print(gps.date.day());
    Serial.print("/");
    Serial.print(gps.date.month());
    Serial.print("/");
    Serial.print(gps.date.year());
    Serial.print(" | Time: ");
    // Use the same time adjustment for Serial output
    int serialHour = gps.time.hour() + 2;
    if (serialHour >= 24) serialHour -= 24;
    if (isDST(gps.date.year(), gps.date.month(), gps.date.day())) {
      serialHour += 1;
      if (serialHour >= 24) serialHour -= 24;
    }
    Serial.print(serialHour);
    Serial.print(":");
    Serial.print(gps.time.minute());
    Serial.print(":");
    Serial.print(gps.time.second());
    Serial.println("      ");  // Extra spaces to clear previous characters

  } else {
    Serial.println("GPS data is invalid.");
  }
}

// Function to check if the date is within the Bucharest DST period (March 31 - October 31)
bool isDST(int year, int month, int day) {
  int dstStartDay = getLastSunday(DST_START_MONTH, year);
  int dstEndDay = getLastSunday(DST_END_MONTH, year);

  if (month > DST_START_MONTH && month < DST_END_MONTH) {
    return true;
  } else if (month == DST_START_MONTH && day >= dstStartDay) {
    return true;
  } else if (month == DST_END_MONTH && day <= dstEndDay) {
    return true;
  }
  return false;
}

// Function to calculate the last Sunday of a month
int getLastSunday(int month, int year) {
  int daysInMonth = getDaysInMonth(month, year);
  for (int day = daysInMonth; day >= 1; day--) {
    if (getDayOfWeek(day, month, year) == 0) {  // Sunday
      return day;
    }
  }
  return 31;
}

// Function to get the number of days in a given month
int getDaysInMonth(int month, int year) {
  if (month == 1 || month == 3 || month == 5 || month == 7 ||
      month == 8 || month == 10 || month == 12) {
    return 31;
  } else if (month == 4 || month == 6 || month == 9 || month == 11) {
    return 30;
  } else if (month == 2) {
    return (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0)) ? 29 : 28;
  }
  return 30;
}

// Function to get the day of the week (0 = Sunday, 1 = Monday, etc.)
int getDayOfWeek(int day, int month, int year) {
  int t;
  int m = month;
  if (m <= 2) {
    m += 12;
    year--;
  }
  t = (year + year / 4 - year / 100 + year / 400 + (31 * m) / 12 + day) % 7;
  return t;
}
