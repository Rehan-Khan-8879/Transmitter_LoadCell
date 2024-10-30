#include <SPI.h>
#include <RF24.h>
#include <HX711.h>
#include <DHT.h>

#define DHTPIN 2      // Pin where the DHT sensor is connected
#define DHTTYPE DHT11 // Change to DHT22 if using that model

DHT dht(DHTPIN, DHTTYPE);

RF24 radio(9, 10);  // CE, CSN 
const byte address[6] = "00001"; 

// Load cell setup
HX711 scale;
const int DOUT_PIN = 4;  // DOUT pin of HX711
const int SCK_PIN = 5;   // SCK pin of HX711

// Temperature sensor setup
int lm35Pin = A0;  // Analog pin connected to LM35

struct DataPacket {
  float temperature;
  float weight;
  int level;
} data;

void setup() {
  Serial.begin(9600);

  // nRF24L01 setup
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_LOW);
  radio.stopListening();  // Set to transmit mode

  // Load cell setup
  scale.begin(DOUT_PIN, SCK_PIN);
  scale.set_scale(495);  // Calibration factor, adjust as needed
  scale.tare();  // Reset scale to zero

  Serial.println("Transmitter ready");
}

void loop() {
  // Read temperature from LM35
  data.temperature = dht.readTemperature();

  // Read weight from load cell
  if (scale.is_ready()) {
    data.weight = scale.get_units(5);  // Average over 5 readings
      data.level = scale.get_units(5);
  } else {
    data.weight = 0;  // Set to 0 if load cell is not ready
    data.level = 0;
  }

  // Transmit data
  radio.write(&data, sizeof(data));

  Serial.print("Temperature: ");
  Serial.print(data.temperature);
  Serial.print(" °C, Weight: ");
  Serial.print(data.weight);
  Serial.println(" kg");

  delay(1000);  // Delay between transmissions
}
