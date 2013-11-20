

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

static inline void shift_bit(uint8_t value) {
  digitalWrite(DATA, value);
  digitalWrite(W_CLK, HIGH);
  delay(1);
  digitalWrite(W_CLK, LOW);
}

void dds(unsigned long frequency, bool power_down, uint8_t phase) {
  // Make sure we start with the clock low
  digitalWrite(W_CLK, LOW);
  digitalWrite(FQ_UD, LOW);  
  
  for (int i = 0; i < 32; i++) {
    shift_bit(frequency & 1);
    frequency = frequency >> 1;
  }
  // send 0 for w32/w33, powerdown and phase.
  for (int i = 0; i < 8; i++) {
    shift_bit(0);
  }
  digitalWrite(FQ_UD, HIGH);
  delay(1);
  digitalWrite(FQ_UD, LOW);  
}
