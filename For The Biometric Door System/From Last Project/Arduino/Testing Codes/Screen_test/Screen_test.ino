/*
  LiquidCrystal Library - Hello World
  www.circuits4you.com
 Demonstrates the use a 16x2 LCD display.  The LiquidCrystal
 library works with all LCD displays that are compatible with the 
 Hitachi HD44780 driver. There are many of them out there, and you
 can usually tell them by the 16-pin interface.
 
 This sketch prints "Hello World! and circuits4you.com" to the LCD
 and shows smiley.
 
  The circuit:
 * LCD RS pin to digital pin 12
 * LCD Enable pin to digital pin 11
 * LCD D4 pin to digital pin 5
 * LCD D5 pin to digital pin 4
 * LCD D6 pin to digital pin 3
 * LCD D7 pin to digital pin 2
 * LCD R/W pin to ground
 */

// include the library code:
#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(A5, A4, A3, A2, A1, A0);

byte smiley[8] = {
  0b00000,
  0b00000,
  0b01010,
  0b00000,
  0b00000,
  0b10001,
  0b01110,
  0b00000
};

void setup() {
  // set up the LCD's number of columns and rows: 
  lcd.begin(16, 2);
  
  // create a new character
  lcd.createChar(1, smiley);

  // Print a message to the LCD.
  lcd.write(1); //Print custom character smiley
  lcd.print("hello, world!");
  lcd.write(1); //Print custom character smiley
}

void loop() {
  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  lcd.setCursor(0, 0);
  lcd.print("Welcome to the class");
  lcd.setCursor(0, 1);
  lcd.print("Please take your attendace.");
  
  for(int i = 0; i < 20; ++i){
    delay(500);
    lcd.scrollDisplayLeft();
  }
}
