#include <Wire.h>
#include <LiquidCrystal_I2C.h>

const byte inputPin = 3;  // Input pin for the frequency signal
const byte voltagePin = A0; // Analog pin for voltage measurement
const byte relayPin = 2;   // Pin connected to the relay module

float frequency;  // Variable to store frequency
float voltage;    // Variable to store voltage

LiquidCrystal_I2C lcd(0x27, 16, 2); // Set the I2C address for the LCD (adjust if needed)

void setup() {
  Serial.begin(115200);
  pinMode(inputPin, INPUT);
  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, HIGH); // Turn relay ON initially (HIGH means ON in this setup)

  lcd.init();        // Initialize the LCD
  lcd.backlight();   // Turn on the backlight
  lcd.setCursor(0, 0);
  lcd.print("Initializing...");
  delay(2000);
}

void loop() {
  // Measure frequency
  unsigned long pulseHigh = pulseIn(inputPin, HIGH, 1000000); // Timeout set to 1 second
  unsigned long pulseLow = pulseIn(inputPin, LOW, 1000000);

  if (pulseHigh > 0 && pulseLow > 0) {  // Valid pulse detected
    unsigned long fullPulse = pulseHigh + pulseLow; // Total Time period in microseconds
    frequency = 1000000.0 / fullPulse; // Calculate frequency in Hz
  } else {
    frequency = 0; // No pulse detected
  }

  // Measure voltage
  int rawValue = analogRead(voltagePin); // Read analog voltage
  voltage = (rawValue / 1023.0) * 5.0 * 44;   // Convert to voltage (assuming a 5V reference and a 44:1 voltage divider)

  // Check conditions to control the relay
  if (voltage > 225 || voltage < 215 || frequency > 52 || frequency < 48) {
    digitalWrite(relayPin, LOW); // Turn relay OFF
  } else {
    digitalWrite(relayPin, HIGH); // Turn relay ON
  }

  // Display results on Serial Monitor
  Serial.print("Frequency: ");
  Serial.print(frequency);
  Serial.println(" Hz");

  Serial.print("Voltage: ");
  Serial.print(voltage);
  Serial.println(" V");

  // Display results on LCD
  lcd.setCursor(0, 0);
  lcd.print("Freq: ");
  lcd.print(frequency);
  lcd.print(" Hz   ");

  lcd.setCursor(0, 1);
  lcd.print("Volt: ");
  lcd.print(voltage);
  lcd.print(" V    ");

  delay(1000); // Update every second
}
