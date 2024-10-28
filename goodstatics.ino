//Encoder should be set up with pins on digital 2 and 3, with common on ground

//Display libraries, Adafruit OLEDs using I2C
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>

//Encoder library
#include <RotaryEncoder.h>

//for pinmode only (probably unecessary)
#define trnsfr 8

Adafruit_SSD1306 display(128, 64, &Wire, 4);

//encoder on pins 2 and 3
RotaryEncoder encoder(2, 3);

//logic vars for frequency differentiation (stuff we need to know to get the "push for decimal, don't for whole num" functions working, also transfer func)
float oldFrequency = 0;
float frequency = 0;
float stbyfreq = 0;
float decact = 0;
float decstby = 0;
float tempdec = 0;
int wholeact = 0;
int wholestby = 0;
int tempwhole = 0;
float tempfreq = 0;



void setup()
{
  Wire.begin();
  display.begin(SSD1306_SWITCHCAPVCC, 0x3D);
  Serial.begin(57600);
  pinMode(6, INPUT);
  pinMode(trnsfr, INPUT);
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(2);
  display.setCursor(0,0);
  display.print("Welcome to Good Statics!");
  display.display();
  delay(5000);
  display.clearDisplay();
} // setup()

void updateScreen(float freqi, float freqii) {
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.print("COM1 ACTV");
  display.setCursor(0, 10);
  display.setTextSize(2);
  display.print(freqi);
  display.setCursor(0, 26);
  display.setTextSize(1);
  display.print("COM1 STBY");
  display.setCursor(0, 36);
  display.setTextSize(2);
  display.print(freqii);
  display.display();
}


void transfer() {
  Serial.println("s");
  tempfreq = frequency;
  frequency = stbyfreq;
  oldFrequency = stbyfreq;
  stbyfreq = tempfreq;
  tempwhole = wholeact;
  wholeact = wholestby;
  wholestby = tempwhole;
  tempdec = decact;
  decact = decstby;
  decstby = tempdec;
  updateScreen(frequency, stbyfreq);  
  delay(500);  
}

void loop()
{
  int pos = 0;
  
  encoder.tick();
  int newPos = encoder.getPosition(); //get position
  int dec = digitalRead(6);
  if (newPos != pos && dec == HIGH){
      if (newPos < 0) {
        decact = decact - 0.01;
      }
      else {
        decact = decact + 0.01;
      }
      encoder.setPosition(0);
  }
  else if (newPos != pos) {
    if (newPos < 0) {
      wholeact = wholeact - 1; 
    }
    else {
      wholeact = wholeact + 1;
    }
    encoder.setPosition(0);
  }
  if (wholeact > 136) {
    wholeact = 118;
  }
  else if (wholeact < 118) {
    wholeact = 136;
  }
  if (decact < 0.0) {
    decact = 0.99;
  }
  else if (decact > 0.99) {
    decact = 0.0;
  }
  frequency = decact + wholeact;
  if (frequency != oldFrequency) {
    Serial.println(frequency);
    oldFrequency = frequency;
    updateScreen(frequency, stbyfreq);
  }
  if (digitalRead(trnsfr) == HIGH) {
    transfer();
  }
  
   // if
} // loop ()

// The End
