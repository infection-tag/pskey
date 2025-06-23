// Currently just sending outputs to light due to Wokwi limitations

const int rfOutPin = 2;
const int buttonPin1 = 3;
const int buttonPin2 = 4;

// Microsecond-level signal patterns
const unsigned int signal1[] = {500, 1500, 500, 500, 1000, 2000};  // Example RF signal 1
const unsigned int signal2[] = {400, 1200, 400, 400, 600, 1600};   // Example RF signal 2

void setup() {
  pinMode(rfOutPin, OUTPUT);
  pinMode(buttonPin1, INPUT_PULLUP);
  pinMode(buttonPin2, INPUT_PULLUP);

  Serial.begin(9600);
  Serial.println("Microsecond Key Fob Emulator Ready");
}

void loop() {
  if (digitalRead(buttonPin1) == LOW) {  // Button 1 pressed
    Serial.println("Transmitting Signal 1");
    transmitSignal(signal1, sizeof(signal1) / sizeof(signal1[0]));
    delay(300);  // Simple debounce
  }

  if (digitalRead(buttonPin2) == LOW) {  // Button 2 pressed
    Serial.println("Transmitting Signal 2");
    transmitSignal(signal2, sizeof(signal2) / sizeof(signal2[0]));
    delay(300);  // Simple debounce
  }
}

void transmitSignal(const unsigned int signal[], int length) {
  for (int i = 0; i < length; i++) {
    digitalWrite(rfOutPin, i % 2 == 0 ? HIGH : LOW);
    delayMicroseconds(signal[i]);  // Precise pulse width
  }
  digitalWrite(rfOutPin, LOW);  // Ensure pin is low after transmission
  Serial.println("Transmission Complete");
}
