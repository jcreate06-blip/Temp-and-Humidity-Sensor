/**
 * @file    Temp_Humidity_Project.ino
 * @brief   Reads temperature and humidity from a DHT11 sensor and displays
 *          the results on a 16x2 LCD. A toggle button switches the temperature
 *          display between Celsius and Fahrenheit.
 *
 * Wiring Summary:
 *  - DHT11 data pin   Arduino pin 2
 *  - LCD RS           Arduino pin 3
 *  - LCD EN           Arduino pin 4
 *  - LCD D4-D7        Arduino pins 8, 9, 10, 11
 *  - Toggle button    Arduino pin 13 (uses internal pull-up)
 */

// ── Libraries ────────────────────────────────────────────────────────────────
#include <LiquidCrystal.h>  
#include "dht.h"            // DHT sensor library for reading temperature and humidity

// ── Pin Definitions ──────────────────────────────────────────────────────────
/** @brief Arduino digital pin connected to the DHT11 data line. */
#define DHTPIN 2

// ── Object Instantiation ─────────────────────────────────────────────────────
/** @brief DHT sensor object used to trigger reads and access data. */
dht DHT;

/**
 * @brief LCD object configured for 4-bit mode.
 *        Parameters: (RS, EN, D4, D5, D6, D7)
 */
LiquidCrystal lcd(3, 4, 8, 9, 10, 11);

// ── Pin Constants ─────────────────────────────────────────────────────────────
/** @brief Digital pin connected to the Celsius/Fahrenheit toggle button. */
const int switchPin = 13;

// ── State Variables ───────────────────────────────────────────────────────────
/** @brief Current state of the toggle button (HIGH = not pressed). */
int switchState = HIGH;

/** @brief Previous state of the toggle button, used for edge detection. */
int prevSwitchState = HIGH;

/** @brief Display mode flag: true = Celsius, false = Fahrenheit. */
bool isC = true;

// ── Timing Variables ──────────────────────────────────────────────────────────
/** @brief Timestamp (ms) of the last successful sensor read. */
unsigned long lastUpdate = 0;

/** @brief Minimum time (ms) between sensor reads. DHT11 max sample rate is ~1 Hz. */
const unsigned long interval = 1000;


// ── setup() ──────────────────────────────────────────────────────────────────
/**
 * @brief Runs once on power-up or reset.
 *        Initialises serial comms, configures pins, and shows a
 *        brief splash screen on the LCD before the main loop begins.
 */
void setup() {
  Serial.begin(9600); // Open serial port at 9600 baud for debug output

  // Configure toggle button pin with the internal pull-up resistor.
  // This means the pin reads HIGH when the button is open and LOW when pressed.
  pinMode(switchPin, INPUT_PULLUP);

  lcd.begin(16, 2); // Initialise the LCD: 16 columns, 2 rows

  // ── Splash screen ──
  lcd.print("Temp & Humidity");   // Row 0: title
  lcd.setCursor(0, 1);
  lcd.print("Scanning");          // Row 1: status message
  delay(1500);                    // Hold splash for 1.5 s so the user can read it
  lcd.clear();                    // Clear display before the main loop writes data
}


// ── loop() ───────────────────────────────────────────────────────────────────
/**
 * @brief Main program loop — runs repeatedly after setup() completes.
 *
 *        Each iteration:
 *          1. Checks the toggle button for a C°/F° switch request.
 *          2. Once per interval, reads the DHT11 and refreshes the LCD.
 */
void loop() {

  // ── Button handling (edge detection) ──────────────────────────────────────
  switchState = digitalRead(switchPin); // Sample the current button state

  // Only act on a *change* in state to avoid toggling repeatedly while held
  if (switchState != prevSwitchState) {
    if (switchState == LOW) {       // LOW = button just pressed (pull-up wiring)
      isC = !isC;                   // Flip between Celsius and Fahrenheit
      delay(100);                   // Small debounce delay to filter contact bounce
    }
  }

  prevSwitchState = switchState; // Store state for comparison on the next iteration

  // ── Non-blocking sensor update ────────────────────────────────────────────
  unsigned long now = millis();                   // Current uptime in milliseconds
  if (now - lastUpdate >= interval) {             // True once every 'interval' ms
    lastUpdate = now;                             // Reset the timer

    // Read temperature and humidity from the DHT11.
    // Returns DHTLIB_OK (0) on success, or a non-zero error code on failure.
    int chk = DHT.read11(DHTPIN);

    // ── Error handling ──
    if (chk != DHTLIB_OK) {
      // Display a descriptive error so wiring faults are easy to diagnose
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("DHT error: ");
      lcd.print(chk);          // Print the numeric error code for reference
      lcd.setCursor(0, 1);
      lcd.print("Check wiring");
      return;                  // Skip the rest of the loop until next interval
    }

    // ── Retrieve sensor values ──
    float h  = DHT.humidity;     // Relative humidity in % RH
    float tC = DHT.temperature;  // Temperature in degrees Celsius

    lcd.clear(); // Clear stale data before writing new values

    // ── Row 0: Temperature ──────────────────────────────────────────────────
    lcd.setCursor(0, 0);
    lcd.print("Temp:  ");

    if (isC) {
      // Display temperature as-is from the sensor (already in Celsius)
      lcd.print(tC, 1);         // 1 decimal place, e.g. "23.5"
      lcd.print((char)223);     // Degree symbol 
      lcd.print("C");
    } else {
      // Convert Celsius to Fahrenheit:  F = C × (9/5) + 32
      float tF = tC * 9.0 / 5.0 + 32.0;
      lcd.print(tF, 1);         // 1 decimal place, e.g. "74.3"
      lcd.print((char)223);     // Degree symbol
      lcd.print("F");
    }

    // ── Row 1: Humidity ──────────────────────────────────────────────────────
    lcd.setCursor(0, 1);
    lcd.print("Hum:   ");
    lcd.print(h, 1);   // 1 decimal place, e.g. "55.0"
    lcd.print("%");
  }
}
