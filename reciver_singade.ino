#include <SPI.h>
#include <RF24.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

RF24 radio(9, 10);  // CE, CSN pins for nRF24L01
const byte address[6] = "00001";  // Same address as transmitter

// Data packet structure
struct DataPacket {
  float temperature;
  float weight;
} receivedData;

// LCD setup (2x40 display)
LiquidCrystal_I2C lcd(0x27, 20, 4);  // Adjust I2C address if needed

void setup() {
  Serial.begin(9600);

  // Initialize nRF24L01
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_LOW);
  radio.startListening();  // Set to receive mode

  // Initialize LCD
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Waiting for data...");

  Serial.println("Receiver ready");
}

void loop() {
  if (radio.available()) {
    // Read the data from the transmitter
    radio.read(&receivedData, sizeof(receivedData));

    // Print to Serial Monitor
    Serial.print("Temperature: ");
    Serial.print(receivedData.temperature);
    Serial.print(" °C, Weight: ");
    Serial.print(receivedData.weight);
    Serial.println(" gram");

    // Display on LCD
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Temperature:");
    lcd.print(receivedData.temperature, 2);  // Display temperature with 2 decimal places
    lcd.print("C");

    lcd.setCursor(0, 1);
    lcd.print("Weight: ");
    lcd.print(receivedData.weight, 2);  // Display weight with 2 decimal places
    lcd.print(" GRAM");



    lcd.setCursor(0,2);
    lcd.print("volume:");
    lcd.print(receivedData.weight);
    lcd.print(" mL");

    lcd.setCursor(7,3);
    lcd.print("TEAM 7");
  }

  delay(500);  // Short delay to allow for stable display
}