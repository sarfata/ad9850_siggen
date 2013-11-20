

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

void loop() {
  Serial.println("1kHz");
  dds(34360, false, 0);
  delay(5000);

  Serial.println("3 kHz");
  dds(103079ul, false, 0);
  delay(5000);

  Serial.println("1 MHz");
  dds(34359700, false, 0);
  delay(5000);

  Serial.println("10 MHz");
  dds(343597000, false, 0);
  delay(5000);
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
