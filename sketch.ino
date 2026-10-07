#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "DHT.h"

#define DHTPIN 4
#define DHTTYPE DHT22

#define VOLTAGE_PIN 34
#define CURRENT_PIN 35

#define LED_PIN 5
#define BUTTON_PIN 18

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

DHT dht(DHTPIN, DHTTYPE);
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

bool applianceState = false;

void setup() {
  Serial.begin(115200);

  dht.begin();

  pinMode(LED_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  digitalWrite(LED_PIN, LOW);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED initialization failed!");
    while (true);
  }

  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);

  display.setCursor(0, 0);
  display.println("Smart Energy");
  display.println("Monitoring");
  display.display();

  delay(2000);
}

void loop() {

  // Read DHT22
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  // Read potentiometers
  int voltageRaw = analogRead(VOLTAGE_PIN);
  int currentRaw = analogRead(CURRENT_PIN);

  // Convert simulated values
  float voltage = (voltageRaw / 4095.0) * 240.0;
  float current = (currentRaw / 4095.0) * 10.0;

  // Calculate power
  float power = voltage * current;

  // Button control
  if (digitalRead(BUTTON_PIN) == LOW) {
    applianceState = !applianceState;
    digitalWrite(LED_PIN, applianceState ? HIGH : LOW);

    delay(300);
  }

  Serial.println();
  Serial.println("========== ENERGY MONITOR ==========");

  Serial.print("Temperature : ");
  Serial.print(temperature);
  Serial.println(" °C");

  Serial.print("Humidity    : ");
  Serial.print(humidity);
  Serial.println(" %");

  Serial.print("Voltage     : ");
  Serial.print(voltage);
  Serial.println(" V");

  Serial.print("Current     : ");
  Serial.print(current);
  Serial.println(" A");

  Serial.print("Power       : ");
  Serial.print(power);
  Serial.println(" W");

  Serial.print("Appliance   : ");
  Serial.println(applianceState ? "ON" : "OFF");

  Serial.println("====================================");

  // OLED display
  display.clearDisplay();

  display.setCursor(0, 0);
  display.println("ENERGY MONITOR");

  display.print("V: ");
  display.print(voltage, 1);
  display.println(" V");

  display.print("I: ");
  display.print(current, 2);
  display.println(" A");

  display.print("P: ");
  display.print(power, 1);
  display.println(" W");

  display.print("Temp: ");
  display.print(temperature, 1);
  display.println(" C");

  display.print("Load: ");
  display.println(applianceState ? "ON" : "OFF");

  display.display();

  delay(2000);
}
