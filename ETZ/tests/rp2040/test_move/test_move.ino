#define dir1_pin 15
#define step1_pin 14
#define dir2_pin 13
#define step2_pin 12


void setup() {
  Serial.begin(921600);
  pinMode(dir1_pin, OUTPUT);
  pinMode(step1_pin, OUTPUT);
  pinMode(dir2_pin, OUTPUT);
  pinMode(step2_pin, OUTPUT);
  digitalWrite(step1_pin, LOW);
  digitalWrite(step2_pin, LOW);
}

void loop() {

  digitalWrite(dir1_pin, 1);
  digitalWrite(dir2_pin, 0);
  delayMicroseconds(5);
  for (long i = 0; i < 5000; i++) {
    digitalWrite(step1_pin, HIGH);
    digitalWrite(step2_pin, HIGH);
    delayMicroseconds(80);
    digitalWrite(step1_pin, LOW);
    digitalWrite(step2_pin, LOW);
    delayMicroseconds(80);
  }
  delay(500);

  digitalWrite(dir1_pin, 0);
  digitalWrite(dir2_pin, 1);
  delayMicroseconds(5);
  for (long i = 0; i < 5000; i++) {
    digitalWrite(step1_pin, HIGH);
    digitalWrite(step2_pin, HIGH);
    delayMicroseconds(80);
    digitalWrite(step1_pin, LOW);
    digitalWrite(step2_pin, LOW);
    delayMicroseconds(80);
  }
  delay(500);


  digitalWrite(dir1_pin, 0);
  digitalWrite(dir2_pin, 0);
  delayMicroseconds(5);
  for (long i = 0; i < 5000; i++) {
    digitalWrite(step1_pin, HIGH);
    digitalWrite(step2_pin, HIGH);
    delayMicroseconds(80);
    digitalWrite(step1_pin, LOW);
    digitalWrite(step2_pin, LOW);
    delayMicroseconds(80);
  }
  delay(500);


  digitalWrite(dir1_pin, 1);
  digitalWrite(dir2_pin, 1);
  delayMicroseconds(5);
  for (long i = 0; i < 5000; i++) {
    digitalWrite(step1_pin, HIGH);
    digitalWrite(step2_pin, HIGH);
    delayMicroseconds(80);
    digitalWrite(step1р0зх
    _pin, LOW);
    digitalWrite(step2_pin, LOW);
    delayMicroseconds(80);
  }
  delay(500);
}
