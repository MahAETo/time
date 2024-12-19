const byte inputPin = 2;  // Input pin for the frequency signal
const byte voltagePin = A0; // Analog pin for voltage measurement

float frequency;  // Variable to store frequency
float voltage;    // Variable to store voltage

void setup() {
  Serial.begin(115200);
  pinMode(inputPin, INPUT);
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
  voltage = (rawValue / 1023.0) * 5.0;   // Convert to voltage (assuming a 5V reference)

  // Display results
  Serial.print("Frequency: ");
  Serial.print(frequency);
  Serial.println(" Hz");

  Serial.print("Voltage: ");
  Serial.print(voltage);
  Serial.println(" V");

  delay(1000); // Update every second
}
