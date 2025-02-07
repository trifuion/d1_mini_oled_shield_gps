# GPS Speed, Altitude, and Time Display with DST Adjustment

This project displays GPS data (speed, altitude, satellites) on an OLED screen and adjusts the displayed time based on the Daylight Saving Time (DST) for Bucharest, Romania. The project uses an ESP8266 development board (WeMos D1 Mini), a NEO-6M GPS module, and a 0.96" I2C OLED display.

## Hardware Components
- **Development Board**: ESP8266 ESP-12F CH340G V2 (WeMos D1 Mini)
- **GPS Module**: NEO-6M GPS Module (GY-NEO6MV2)
- **OLED Display**: 0.96" I2C SSD1306 OLED (128x64)

## Libraries Required
- **Adafruit_SSD1306**: Library for controlling the SSD1306 OLED display.
- **TinyGPS++**: Library for parsing GPS data.
- **SoftwareSerial**: Library for serial communication with the GPS module.

## Project Description
The project reads GPS data such as speed, altitude, and the number of satellites, then displays this information on a 0.96" OLED screen. The time is adjusted for Bucharest’s Daylight Saving Time (DST), which follows the schedule of UTC +2 hours, moving to UTC +3 hours during DST (from the last Sunday of March until the last Sunday of October).

### GPS Data Displayed:
1. **Speed**: In kilometers per hour (km/h).
2. **Altitude**: In meters (m).
3. **Satellites**: Number of satellites the GPS module is connected to.
4. **Time**: The time is adjusted for Bucharest's local time (with DST).

## Setup Instructions

### Wiring the Components:
- **GPS Module (NEO-6M)**:
  - **RX** to **GPIO13** (D7) on WeMos D1 Mini
  - **TX** to **GPIO15** (D8) on WeMos D1 Mini
  - **GND** to **GND**
  - **VCC** to **3.3V**
  
- **OLED Display** (I2C):
  - **SCL** to **D1** (GPIO5) on WeMos D1 Mini
  - **SDA** to **D2** (GPIO4) on WeMos D1 Mini
  - **GND** to **GND**
  - **VCC** to **5V**

### Libraries Installation:
1. Open the Arduino IDE and go to **Sketch > Include Library > Manage Libraries**.
2. Install the following libraries:
   - **Adafruit SSD1306** by Adafruit
   - **TinyGPS++** by Mikal Hart

### Code Upload:
1. Open the Arduino IDE.
2. Paste the provided code in a new sketch.
3. Select **WeMos D1 Mini** under **Tools > Board**.
4. Select the correct port under **Tools > Port**.
5. Upload the code to the WeMos D1 Mini.

### Display:
- The OLED screen will show:
  - The speed in km/h.
  - The altitude in meters.
  - The number of satellites.
  - The current time adjusted for Bucharest's DST.

## Code Explanation

### Key Functions:
1. **print_gps_data()**:
   - Reads and displays the GPS data on the OLED screen (speed, altitude, satellites, and time).
   - Adjusts the time for Bucharest DST.
   
2. **isDST()**:
   - Checks if the current date falls within the Daylight Saving Time (DST) period for Bucharest.
   
3. **getLastSunday()**:
   - Calculates the date of the last Sunday in a given month to determine when DST starts or ends.

4. **getDaysInMonth()**:
   - Returns the number of days in a given month, considering leap years.

5. **getDayOfWeek()**:
   - Returns the day of the week for a given date (0 = Sunday, 1 = Monday, etc.).

## Example Output on OLED Display:
- **Speed**: 52 km/h
- **Satellites**: 10
- **Altitude**: 230 m
- **Time**: 14:45:23

### Serial Monitor Output:
- **Lat**: 44.4268 | **Lng**: 26.1025 | **Speed**: 52 km/h | **Altitude**: 230 m | **Sat**: 10 | **Date**: 07/02/2025 | **Time**: 14:45:23

## Notes:
- The GPS module needs to be outdoors or near a window for optimal signal reception.
- Make sure to adjust the wiring for your specific setup.

## License:
This project is open source under the MIT License.
