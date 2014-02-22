

// Pin assignment
static const int W_CLK = 4; // AD9850 W_CLK
static const int FQ_UD = 2;  // AD9850 FQ_UD
static const int DATA = 3;  // AD9850 DATA (D7)

void setup() {
  Serial.begin(9600);
  pinMode(W_CLK, OUTPUT);
  pinMode(FQ_UD, OUTPUT);
  pinMode(DATA, OUTPUT);
}


// we keep the freq in Hz
uint32_t frequency = 10000000;

void loop() {
  // deltaphase = frequency * 2^32 / 125 * 10^3 (quartz freq in khz)
  dds(34.359738368 * frequency, false, 0);

  Serial.print("Frequency: ");
  Serial.print(frequency);
  Serial.println(" Hz");

  while (Serial.peek() == -1) {
    delay(50);
  }
  int c = Serial.read();
  if (c == 'k')
    frequency += 50;
  if (c == 'j')
    frequency -= 50;
  if (c == 'K')
    frequency += 5000;
  if (c == 'J')
    frequency -= 5000;

  if (c >= '0' && c <= '9') {
    Serial.print("Enter frequency in Hz: ");
    Serial.print(c - '0');
    frequency = c - '0';
    while (c != '\n') {
      c = Serial.read();
      if (c >= '0' && c <= '9') {
        Serial.print(c - '0');
        frequency *= 10;
        frequency += c - '0';
      }
    }
    Serial.println();
  }
}

void dds(unsigned long frequency, bool power_down, uint8_t phase) {
  // Make sure we start with the clock low
  digitalWrite(W_CLK, LOW);
  digitalWrite(FQ_UD, LOW);

  // Output the 4 bytes with frequency info
  for (int i = 0; i < 4; i++) {
    shiftOut(DATA, W_CLK, LSBFIRST, frequency & 0xff);
    frequency = frequency >> 8;
  }
  // send 0 for w32/w33, powerdown and phase.
  uint8_t command = phase << 3;
  if (power_down) {
    // Turn on the power_down bit if required
    phase |= 4;
  }
  shiftOut(DATA, W_CLK, LSBFIRST, command);

  // Pulse FQ_UD to load the new data.
  digitalWrite(FQ_UD, HIGH);
  delay(1);
  digitalWrite(FQ_UD, LOW);
}
