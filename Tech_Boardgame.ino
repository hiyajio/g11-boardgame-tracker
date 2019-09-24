// Adafruit IO Digital Input Example
// Tutorial Link: https://learn.adafruit.com/adafruit-io-basics-digital-input
//
// Adafruit invests time and resources providing this open source code.
// Please support Adafruit and open source hardware by purchasing
// products from Adafruit!
//
// Written by Todd Treece for Adafruit Industries
// Copyright (c) 2016 Adafruit Industries
// Licensed under the MIT license.
//
// All text above must be included in any redistribution.

/************************** Configuration ***********************************/

// edit the config.h tab and enter your Adafruit IO credentials
// and any additional configuration needed for WiFi, cellular,
// or ethernet clients.
#include <ESP8266WiFi.h>
#include <SPI.h>
#include "config.h"
#include <Wire.h>
#include <Adafruit_PN532.h>
#include <LiquidCrystal.h>

// If using the breakout with SPI, define the pins for SPI communication.
#define PN532_SCK  (16)
#define PN532_MOSI (14)
#define PN532_SS   (12)
#define PN532_MISO (13)

// If using the breakout or shield with I2C, define just the pins connected
// to the IRQ and reset lines.  Use the values below (2, 3) for the shield!
#define PN532_IRQ   (16)
#define PN532_RESET (14)  // Not connected by default on the NFC Shield

// Use this line for a breakout with a software SPI connection (recommended):
Adafruit_PN532 nfc(PN532_SCK, PN532_MISO, PN532_MOSI, PN532_SS);

#if defined(ARDUINO_ARCH_SAMD)
// for Zero, output on USB Serial console, remove line below if using programming port to program the Zero!
// also change #define in Adafruit_PN532.cpp library file
   #define Serial SerialUSB
#endif

/************************ Example Starts Here *******************************/

// set up the feed
AdafruitIO_Feed *welcome-feed = io.feed("welcome-feed"); //Change if different key(name) for feed used

void setup() {
  // start the serial connection
  Serial.begin(115200);
  #ifndef ESP8266
    while (!Serial); // for Leonardo/Micro/Zero
  #endif

  nfc.begin();
  
  // configure board to read RFID tags
  nfc.SAMConfig();

  // connect to io.adafruit.com
  Serial.print(F("Connecting to Adafruit IO"));
  io.connect();

  // wait for a connection
  while(io.status() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  // we are connected
  Serial.println("Connected!");
  Serial.println();
  Serial.println(io.statusText());
}

uint8_t success;

uint8_t uid[] = { 0, 0, 0, 0 };
uint8_t uid1[] = { 0x8C, 0xBC, 0x20, 0x00 }; // REPLACE
uint8_t uid2[] = { 0x64, 0x8D, 0x77, 0xCA }; // REPLACE
uint8_t uid3[] = { 0x74, 0xFC, 0x7B, 0xCA }; // REPLACE
//Add or subtracts uid arrays as needed depending on number of cards used.

uint8_t uidLength; 

int i = 0;
int j = 0;
int k = 0;
//Only use if counter is needed. 
//For example, to have a checked in/out status change.

void loop() {
  // io.run(); is required for all sketches.
  // it should always be present at the top of your loop
  // function. it keeps the client connected to
  // io.adafruit.com, and processes any incoming data.
  io.run();
  
  Serial.print(F("Received <- "));
  Serial.print(F("Ready to Scan"));
  Serial.print("");

  success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength);
    if (success) {
      nfc.PrintHex(uid, uidLength); // Comment out once you have switched out 
                                    // the UID values for your own cards
      Serial.print(F("Logging boardgame..."));
      if (uid[0] == uid1[0]) {
        i++;
        if (i % 2 == 0) {
          Serial.println(F("Returned: Chess"));
          boardgame->save("Returned: Chess");
          delay(3000);
        } else {
          Serial.println(F("Checking Out: Chess"));
          boardgame->save("Checking Out: Chess");
          delay(3000);
        }
      } else if (uid[0] == uid2[0]) {
        j++;
        if (j % 2 == 0) {
          Serial.println(F("Returned: Checkers"));
          boardgame->save("Returned: Checkers");
          delay(3000);
        } else {
          Serial.println(F("Checking Out: Checkers"));
          boardgame->save("Checking Out: Checkers");
          delay(3000);
        }
      } else if (uid[0] == uid3[0]) {
        k++;
        if (k % 2 == 0) {
          Serial.println(F("Returned: Monopoly"));
          boardgame->save("Returned: Monopoly");
          delay(3000);
        } else {
          Serial.println(F("Checking Out: Monopoly"));
          boardgame->save("Checking Out: Monopoly");
          delay(3000);
        }
      }
    } 
}
