#include <Arduino.h>
#include <Wire.h>
#include <OneWire.h> 
#include <DallasTemperature.h>
#include <LiquidCrystal_I2C.h>

void printAddress(DeviceAddress deviceAddress);
void printTemperature(DeviceAddress deviceAddress, uint8_t position);

// set the LCD address to 0x27 for a 16 chars 2 line display
// A FEW use address 0x3F
// Set the pins on the I2C chip used for LCD connections:
//                    addr, en,rw,rs,d4,d5,d6,d7,bl,blpol
LiquidCrystal_I2C lcd(0x3F, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  // Set the LCD I2C address

/********************************************************************/
// Data wire is plugged into pin2 on the Arduino 
#define ONE_WIRE_BUS 2
#define RELAY_CH1    3
#define RELAY_CH2    4
#define RELAY_CH3    5
#define RELAY_CH4    6
/********************************************************************/
// Setup a oneWire instance to communicate with any OneWire devices  
// (not just Maxim/Dallas temperature ICs) 
OneWire oneWire(ONE_WIRE_BUS); 
/********************************************************************/
// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);
/********************************************************************/ 

// Assign address manually. The addresses below will beed to be changed
// to valid device addresses on your bus. Device address can be retrieved
// by using either oneWire.search(deviceAddress) or individually via
// sensors.getAddress(deviceAddress, index)
// Note that you will need to use your specific address here
DeviceAddress t1 = { 0x28, 0xFF, 0x4D, 0x86, 0xC1, 0x16, 0x04, 0x42 }; // gray, white, black;
DeviceAddress t2 = { 0x28, 0xFF, 0x36, 0x3B, 0xB3, 0x16, 0x05, 0xAD }; // green, blue, red
DeviceAddress t3 = { 0x28, 0xFF, 0x67, 0xD7, 0xB5, 0x16, 0x03, 0xA2 }; // brown,red,orange

/*-----( Declare Variables )-----*/
//NONE

void setup() {
  Serial.begin(9600);  // Used to type in characters
  Serial.println("SolarCtl");

  lcd.begin(16,2);   // initialize the lcd for 16 chars 2 lines, turn on backlight
  lcd.setCursor(0,0); // char, line
  lcd.print("SolarCtl");
  lcd.setCursor(0, 1);
  lcd.print("Locating...");

  pinMode(RELAY_CH1, OUTPUT);
  pinMode(RELAY_CH2, OUTPUT);
  pinMode(RELAY_CH3, OUTPUT);
  pinMode(RELAY_CH4, OUTPUT);

  // locate devices on the bus
  Serial.print("Locating devices...");
  sensors.begin();
  Serial.print("Found ");
  Serial.print(sensors.getDeviceCount(), DEC);
  Serial.println(" devices.");

  // report parasite power requirements
  Serial.print("Parasite power is: "); 
  if (sensors.isParasitePowerMode()) Serial.println("ON");
  else Serial.println("OFF");

  // set the resolution to 12 bit
  sensors.setResolution(t1, 12);
  sensors.setResolution(t2, 12);
  sensors.setResolution(t3, 12);
}

void loop() {
    // call sensors.requestTemperatures() to issue a global temperature 
  // request to all devices on the bus
  Serial.print("Requesting temperatures...");
  sensors.requestTemperatures(); // Send the command to get temperatures
  Serial.println("DONE");

  // It responds almost immediately. Let's print out the data
  printTemperature(t1,  0);
  printTemperature(t2,  5);
  printTemperature(t3, 10);
  delay(1000);

  digitalWrite(RELAY_CH1, HIGH);
  delay(500);
  digitalWrite(RELAY_CH1, LOW);
  delay(500);

  digitalWrite(RELAY_CH2, HIGH);
  delay(500);
  digitalWrite(RELAY_CH2, LOW);
  delay(500);

  digitalWrite(RELAY_CH3, HIGH);
  delay(500);
  digitalWrite(RELAY_CH3, LOW);
  delay(500);

  digitalWrite(RELAY_CH4, HIGH);
  delay(500);
  digitalWrite(RELAY_CH4, LOW);
  delay(500);
}

// function to print a device address
void printAddress(DeviceAddress deviceAddress) {
  for (uint8_t i = 0; i < 8; i++) {
    if (deviceAddress[i] < 16) Serial.print("0");
    Serial.print(deviceAddress[i], HEX);
  }
}

void printTemperature(DeviceAddress deviceAddress, uint8_t position) {
  float tempC = sensors.getTempC(deviceAddress);
  Serial.print("Temp C: ");
  Serial.print(tempC);
  lcd.setCursor(position, 1); // char, line
  lcd.print(tempC);
  lcd.setCursor(position + 4, 1);
  lcd.print(" ");
}
