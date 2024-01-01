#include <SoftwareSerial.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <TinyGPS++.h>


#define SCREEN_WIDTH 64 // OLED display width, in pixels
#define SCREEN_HEIGHT 48 // OLED display height, in pixels

//On an arduino UNO: A4(SDA), A5(SCL)
#define OLED_RESET 0 //Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C //See datasheet for Address
//Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
Adafruit_SSD1306 display(OLED_RESET);

#define rxPin 3
#define txPin 1
SoftwareSerial neogps(rxPin,txPin);

TinyGPSPlus gps;

void setup() {
  Serial.begin(115200);
  //Begin serial communication Arduino IDE (Serial Monitor)

  //Begin serial communication Neo6mGPS
  neogps.begin(9600);
  
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 64x48)

  display.clearDisplay();
  display.display();
  delay(2000);

}

void loop() {
    
  boolean newData = false;
  for (unsigned long start = millis(); millis() - start < 1000;)
  {
    while (neogps.available())
    {
      if (gps.encode(neogps.read()))
      {
        newData = true;
      }
    }
  }

  //If newData is true
  if(newData == true)
  {
    newData = false;
    print_speed();
  }
  else
  {
    display.clearDisplay();
    display.setTextColor(WHITE);
    display.setCursor(0, 0);
    display.setTextSize(2);
    display.print("No Data");
    display.display();
  }  
  
}

void print_speed()
{
  display.clearDisplay();
  display.setTextColor(WHITE);
       
  if (gps.location.isValid() == 1)
  {
   //String gps_speed = String(gps.speed.kmph());
    display.setCursor(0, 0);
    display.setTextSize(2);
    int speed_value=gps.speed.kmph();
    display.print(speed_value);
    
    display.setCursor(40, 0);
    display.setTextSize(1);
    display.print("km/h");

    display.setTextSize(1);
    display.setCursor(0, 30);
    display.print("SAT:");
    display.setCursor(25, 30);
    display.print(gps.satellites.value());

    display.setTextSize(1);
    display.setCursor(0, 40);
    display.print("ALT:");
    display.setCursor(30, 40);
    display.print(gps.altitude.meters(), 0);

    display.display();
    
  }

}
