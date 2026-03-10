# Temp-and-Humidity-Sensor
Arduino UNO project that reads temperature and humidity from a DHT11 sensors and displays the data live readings onto a 16x2 LCD display, with a toggle button that switches the display between Celcius and Fahrenheit.

# Features 
- Live display of both temperature and humidity readings.
- Toggle switch that swaps reading from Celsius and Fahrenheit.
- Error displayed if issues come from wiring

# Hardware Needed
- Arduino UNO
- DHT11 Sensor
- LCD 16x2 Display
- Push Button
- Jumper Wires

# Wiring
|Components  | Arduino Pins  |
|----------------------------|
|DHT11 Data  |             2 |
|LCD RS      |             3 |
|LCD EN      |             4 |
|LCD D4-D7   |     8,9,10,11 |
|Button      |            13 |

#Libraries 
- LiquidCrystal // Arduino built in library
- DHTlib // by Rob Tillaart

#How To Use
- Wire as shown in the image, you can change where the pins connect to where but you will have to change your code.
- Type up the code, verify and upload in the Arduino IDE.
- Press the button to switch between Celsius and Fahrenheit
