#include <Arduino.h>

#pragma region SerialDebug Settings
// SerialDebug Library

// Disable all debug ? Good for release builds (production)
// as nothing from SerialDebug is compiled, zero overhead :-)
// For it just uncomment the DEBUG_DISABLED
// #define DEBUG_DISABLED true

// Define the initial debug level here (uncomment to do it)
#define DEBUG_INITIAL_LEVEL DEBUG_LEVEL_VERBOSE

// Disable SerialDebug debugger ? No more commands and features as functions and globals
// Uncomment this to disable it
#define DEBUG_DISABLE_DEBUGGER false

// Disable auto function naming (good if your debug message already contains it)
//#define DEBUG_AUTO_FUNC_DISABLED true

// Force debug messages to use flash?
// Disable native Serial.printf (if using)
// Good for low memory, due to flash use, but is slowwer and doesn't use macros
//#define DEBUG_USE_FLASH_F true

// Set serial speed (for https://protosupplies.com/product/esp8266-nodemcu-v1-0-esp-12e-wifi-module/ it is 115200).
#define SERIAL_SPEED 115200

#pragma endregion

#include "SerialDebug.h"
#include "constants.h"

// WiFi.
#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h> 
#include <WiFiManager.h>

bool checkButtonPressed(uint8_t pin)
{
  return digitalRead(pin) == HIGH;
}

void setup()
{
  // Set serial rate.
  Serial.begin(SERIAL_SPEED);
#ifdef __AVR_ATmega32U4__
  while (!Serial)
  {
  } // Wait for serial connection to open (only necessary on some boards).
#else
  delay(500); // Wait a fixed time for serial.
#endif
  pinMode(CONNECT_BUTTON_PIN, OUTPUT);
}

void loop()
{
  // Handle Serial Debug.
  debugHandle();

  if (checkButtonPressed(CONNECT_BUTTON_PIN))
  {
    debugA("Button is pressed.");
    // WiFiManager.
    // Local intialization. Once its business is done, there is no need to keep it around.
    WiFiManager wifiManager;

    // Reset settings - for testing.
    // wifiManager.resetSettings();

    // Sets timeout until configuration portal gets turned off.
    // Useful to make it all retry or go to sleep in seconds.
    // wifiManager.setTimeout(120);

    // Start an access point with the specified and enter into a blocking loop awaiting configuration.
    if (!wifiManager.startConfigPortal(HUT_BUTTON_SETUP_SSID)) {
      Serial.println("failed to connect and hit timeout");
      delay(3000);
      // Reset and try again, or maybe put it to deep sleep.
      ESP.reset();
      delay(5000);
    }

    // If you get here you have connected to the WiFi.
    debugA("Connected to WiFi.");
  }
  else
  {
    debugA("Button is not pressed.");
  }
}
