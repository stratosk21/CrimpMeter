/*
 Example using the SparkFun HX711 breakout board with a scale
 By: Nathan Seidle
 SparkFun Electronics
 Date: November 19th, 2014
 License: This code is public domain but you buy me a beer if you use this and we meet someday (Beerware license).
 
 This example demonstrates basic scale output. See the calibration sketch to get the calibration_factor for your
 specific load cell setup.
 
 This example code uses bogde's excellent library: https://github.com/bogde/HX711
 bogde's library is released under a GNU GENERAL PUBLIC LICENSE
 
 The HX711 does one thing well: read load cells. The breakout board is compatible with any wheat-stone bridge
 based load cell which should allow a user to measure everything from a few grams to tens of tons.

 Arduino pin 2 -> HX711 CLK
 3 -> DAT
 5V -> VCC
 GND -> GND
 
 The HX711 board can be powered from 2.7V to 5V so the Arduino 5V power should be fine.
 
*/

#include "HX711.h" //This library can be obtained here http://librarymanager/All#Avia_HX711
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define calibration_factor 15870.0 //This value is obtained using the SparkFun_HX711_Calibration sketch

#define LOADCELL_DOUT_PIN  3
#define LOADCELL_SCK_PIN  2

//OLED Display
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void displayWeight(int weight, int maxWeight){
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  
  // Display current weight
  display.setCursor(0, 10);
  display.print("Weight:     "); // Add extra spaces
  display.print(weight);
  display.print(" kg");
  display.setCursor(0, 30);
  
  // Display maximum weight
  display.setCursor(0, 20);
  display.print("Max Weight: ");
  display.print(maxWeight);
  display.print(" kg");
  display.setCursor(0, 40);

  display.setTextSize(3);

  display.display();
}

HX711 scale;
float maxWeight = 0.0; // Variable to store the maximum weight

void setup() {
  Serial.begin(9600);
  Serial.println("HX711 scale demo");

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  delay(2000);
  display.clearDisplay();
  display.setTextColor(WHITE);

  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  scale.set_scale(calibration_factor); // This value is obtained by using the SparkFun_HX711_Calibration sketch
  scale.tare(); // Assuming there is no weight on the scale at start-up, reset the scale to 0

  Serial.println("Readings:");
}

void loop() {
  float currentWeight = scale.get_units(1); // Get the current weight reading

  if (currentWeight > maxWeight) {
    maxWeight = currentWeight; // Update the maximum weight if a new maximum is detected
  }

  Serial.print("Reading: ");
  Serial.print(currentWeight, 2); // Display the current weight
  Serial.print(" kg");
  Serial.print(" Max Weight: ");
  Serial.print(maxWeight, 2); // Display the maximum weight
  Serial.println();

  displayWeight(currentWeight, maxWeight); 
}