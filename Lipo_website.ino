/*  Arduino 4 channel LiPo battery charger monitor
    (c) A.G.Doswell 2017
    License: The MIT License (See full license at the bottom of this file)

    4 x TP4056 1A LiPo charger modules coupled to analogue inputs A0 to A3.
    Also TP4056 Pin 7 connected to digital pins 3-6 to show charge completed.

    A separate power supply for the 4 modules and the Arduino is preferred

    2.2" ILI9340 TFT display connected via simple resistive level changer to:
    Pin 13 - Clk
    Pin 12 - MISO
    Pin 11 - MOSI
    Pin 10 - CS
    Pin 9 - DC
    Pin 8 - RST

    See http://www.andydoz.blogspot.com/
    for more information and the circuit diagram.

*/

#include "SPI.h"
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9340.h"
#define _sclk 13
#define _miso 12
#define _mosi 11
#define _cs 10
#define _dc 9
#define _rst 8
int ch1Voltage; // raw 10 bit voltage returned from A to D
boolean ch1Complete; // complete flag from lipo module
int ch2Voltage;
boolean ch2Complete;
int ch3Voltage;
boolean ch3Complete;
int ch4Voltage;
boolean ch4Complete;
int barVal; // maximum pixel value for bar graph
float ch1VoltageReal; // real coltage value
int percentage; // percentage of charge
float ch2VoltageReal;
float ch3VoltageReal;
float ch4VoltageReal;

Adafruit_ILI9340 tft = Adafruit_ILI9340(_cs, _dc, _rst);

void setup() {

  pinMode (3, INPUT); // channel 1 complete from pin 7 of changing module
  pinMode (4, INPUT); // channel 2 complete from pin 7 of changing module
  pinMode (5, INPUT); // channel 3 complete from pin 7 of changing module
  pinMode (6, INPUT); // channel 4 complete from pin 7 of changing module
  tft.begin(); // set up tft
  tft.setRotation(1);
  tft.fillScreen(ILI9340_BLACK);
}

void loop() {

  readChannels (); // get raw voltages from channels & complete flags

  barVal = map (ch1Voltage, 573, 860, 0, 320);// maximum pixel posistion for bar graph

  ch1VoltageReal = (ch1Voltage * 0.004918); // calculate real voltage (note map not used here as it won't do floats)
  percentage = map (ch1Voltage, 562, 854, 0, 100); // calculate percentage
  percentage = constrain (percentage, 0, 100); // prevents sillyness in the event of a duff cell or a poor 5v supply to the arduino, causing the reference to drift
  tft.setCursor(0, 0);
  tft.setTextColor(ILI9340_CYAN); tft.setTextSize(2);
  tft.fillRect (0, 0, 250, 16, 0); // clear old display & update info
  tft.print (percentage);
  tft.print ("%   Voltage ");
  tft.print (ch1VoltageReal);
  tft.print (" V");

  if (ch1Voltage < 562 && ch1Complete == false) { // undervoltage trickle charge
    tft.setCursor(0, 27);
    tft.setTextColor(ILI9340_YELLOW); tft.setTextSize(2);
    bar (0, 25); // draw bar graph
    tft.println ("Cell on trickle charge");
  }

  if (ch1Voltage >= 562 && ch1Complete == false ) { //normal 1 amp charge
    bar(barVal, 25);
  }

  if (ch1Complete == true && (ch1VoltageReal >= 4.2)) { // charge complete
    tft.setCursor(0, 27);
    tft.setTextColor(ILI9340_GREEN); tft.setTextSize(2);
    bar (barVal, 25);
    tft.println ("Cell charge complete");
  }

  if (ch1Complete == true && (ch1VoltageReal < 4.2)) {
    tft.setCursor(0, 27);
    tft.setTextColor(ILI9340_RED); tft.setTextSize(2);
    bar (0, 25);
    tft.println ("No cell present");
  }



  //do it all again for channel 2
  barVal = map (ch2Voltage, 573, 860, 0, 320);
  ch2VoltageReal = (ch2Voltage * 0.004918);
  percentage = map (ch2Voltage, 562, 854, 0, 100);
  percentage = constrain (percentage, 0, 100);
  tft.setCursor(0, 55);
  tft.setTextColor(ILI9340_CYAN); tft.setTextSize(2);
  tft.fillRect (0, 55, 250, 16, 0);
  tft.print (percentage);
  tft.print ("%   Voltage ");
  tft.print (ch2VoltageReal);
  tft.print (" V");

  if (ch2Voltage < 562 && ch2Complete == false) {

    tft.setCursor(0, 82);
    tft.setTextColor(ILI9340_YELLOW); tft.setTextSize(2);
    bar (0, 80);
    tft.println ("Cell on trickle charge");
  }

  if (ch2Voltage >= 562 && ch2Complete == false ) {

    bar(barVal, 80);
  }

  if (ch2Complete == true && (ch2VoltageReal >= 4.2)) {
    tft.setCursor(0, 82);
    tft.setTextColor(ILI9340_GREEN); tft.setTextSize(2);
    bar (barVal, 80);
    tft.println ("Cell charge complete");
  }

  if (ch2Complete == true && (ch2VoltageReal < 4.2)) {
    tft.setCursor(0, 82);
    tft.setTextColor(ILI9340_RED); tft.setTextSize(2);
    bar (0, 80);
    tft.println ("No cell present");
  }



  //do it all again for channel 3
  barVal = map (ch3Voltage, 573, 860, 0, 320);
  ch3VoltageReal = (ch3Voltage * 0.004918);
  percentage = map (ch3Voltage, 562, 854, 0, 100);
  percentage = constrain (percentage, 0, 100);
  tft.setCursor(0, 110);
  tft.setTextColor(ILI9340_CYAN); tft.setTextSize(2);
  tft.fillRect (0, 110, 250, 16, 0);
  tft.print (percentage);
  tft.print ("%   Voltage ");
  tft.print (ch3VoltageReal);
  tft.print (" V");


  if (ch3Voltage < 562 && ch3Complete == false) {
    tft.setCursor(0, 137);
    tft.setTextColor(ILI9340_YELLOW); tft.setTextSize(2);
    bar (0, 135);
    tft.println ("Cell on trickle charge");
  }

  if (ch3Voltage >= 562 && ch3Complete == false ) {

    bar(barVal, 135);
  }

  if (ch3Complete == true && (ch3VoltageReal >= 4.2)) {
    tft.setCursor(0, 137);
    tft.setTextColor(ILI9340_GREEN); tft.setTextSize(2);
    bar (barVal, 135);
    tft.println ("Cell charge complete");
  }

  if (ch3Complete == true && (ch3VoltageReal < 4.2)) {
    tft.setCursor(0, 137);
    tft.setTextColor(ILI9340_RED); tft.setTextSize(2);
    bar (0, 135);
    tft.println ("No cell present");
  }



  //do it all again for channel 4
  barVal = map (ch4Voltage, 573, 860, 0, 320);
  ch4VoltageReal = (ch4Voltage * 0.004918);
  percentage = map (ch4Voltage, 562, 854, 0, 100);
  percentage = constrain (percentage, 0, 100);
  tft.setCursor(0, 165);
  tft.setTextColor(ILI9340_CYAN); tft.setTextSize(2);
  tft.fillRect (0, 165, 250, 16, 0);
  tft.print (percentage);
  tft.print ("%   Voltage ");
  tft.print (ch4VoltageReal);
  tft.print (" V");
  
  if (ch4Voltage < 562 && ch4Complete == false) {
    tft.setCursor(0, 192);
    tft.setTextColor(ILI9340_YELLOW); tft.setTextSize(2);
    bar (0, 190);
    tft.println ("Cell on trickle charge");
  }

  if (ch4Voltage >= 562 && ch4Complete == false ) {

    bar(barVal, 190);
  }

  if (ch4Complete == true && (ch4VoltageReal >= 4.2)) {
    tft.setCursor(0, 192);
    tft.setTextColor(ILI9340_GREEN); tft.setTextSize(2);
    bar (barVal, 190);
    tft.println ("Cell charge complete");
  }

 if (ch4Complete == true && (ch4VoltageReal < 4.2)) {
    tft.setCursor(0, 192);
    tft.setTextColor(ILI9340_RED); tft.setTextSize(2);
    bar (0, 190);
    tft.println ("No cell present");
  }


}

int bar (int x, int y) { // draws the bar graph
  for (int x1 = 0; x1 < 321; x1++) {
    int colour = 0xF800; // red
    if (x1 > 180)  colour = 0xFFE0; // yellow
    if (x1 > 280)  colour = 0x07E0; //green
    if (x1 > x) colour =  0x0000; //black
    tft.drawLine( x1, y, x1, y + 20, colour);
  }
  tft.drawLine( 0, y - 1 , 320 , y - 1, 0xFFFF); //
  tft.drawLine( 0, y + 21 , 320 , y + 21, 0xFFFF);
}

void readChannels () {
  int i;
  int ch1Temp;
  int ch2Temp;
  int ch3Temp;
  int ch4Temp;

  ch1Voltage = 1023;
  for (int i = 0; i < 100 ; i++) { // read the a-d 100 times, and get the lowest value, preventing jitter on the a-d
    ch1Temp = analogRead (A0);
    if (ch1Temp < ch1Voltage) {
      ch1Voltage = ch1Temp;
    }
    delay (10);
  }
  ch1Complete = digitalRead (3); // gets the charge complete info from the LiPo module

  ch2Voltage = 1023;
  for (int i = 0; i < 100 ; i++) {
    ch2Temp = analogRead (A1);
    if (ch2Temp < ch2Voltage) {
      ch2Voltage = ch2Temp;
    }
    delay (10);
  }

  ch2Complete = digitalRead (4);

  ch3Voltage = 1023;
  for (int i = 0; i < 100 ; i++) {
    ch3Temp = analogRead (A2);
    if (ch3Temp < ch3Voltage) {
      ch3Voltage = ch3Temp;
    }
    delay (10);
  }

  ch3Complete = digitalRead (5);

  ch4Voltage = 1023;
  for (int i = 0; i < 100 ; i++) {
    ch4Temp = analogRead (A3);
    if (ch4Temp < ch4Voltage) {
      ch4Voltage = ch4Temp;
    }
    delay (10);
  }
  ch4Complete = digitalRead (6);
}
/*
   Copyright (c) 2017 Andrew Doswell

   Permission is hereby granted, free of charge, to any person obtaining a copy
   of this software and associated documentation files (the "Software"), to deal
   in the Software without restriction, including without limitation the rights
   to use, copy, modify, merge, publish, distribute and sublicense
   copies of the Software, and to permit persons to whom the Software is
   furnished to do so, subject to the following conditions:

   The above copyright notice and this permission notice shall be included in
   all copies or substantial portions of the Software.

   Any commercial use is prohibited without prior arrangement.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESerial FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
   AUTHOR(S) OR COPYRIGHT HOLDER(S) BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
   THE SOFTWARE.
*/





