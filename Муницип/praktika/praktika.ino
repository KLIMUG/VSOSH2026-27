#define l0 10
#define l1 11
#define l2 12
#define l3 13
#define l4 14
#define l5 15

#define kn1 9
#define kn2 8
#define kn3 7
#define pot 27
const int leds[] = {l0, l1, l2, l3, l4, l5};
int number = 0b101111;
bool lastKn1 = 1;
bool lastKn2 = 1;
bool lastKn3 = 1;
bool invers = 0;

void showNumber() {
  int n = number;
  for (int i = 0; i < 6; i++) {
    if (invers) analogWrite(leds[i], (abs(n % 2 - 1)) * (255 - analogRead(28)/(1023/255)));
    else analogWrite(leds[i], (n % 2) * analogRead(28)/(1023/255));
    n = n / 2;
  }

  Serial.print(number);
  Serial.print(' ');
  Serial.println(number, BIN);
}

void setup() {
  Serial.begin(921600);
  pinMode(kn1, INPUT_PULLUP);
  pinMode(kn2, INPUT_PULLUP);
  pinMode(kn3, INPUT_PULLUP);
  pinMode(pot, INPUT);
  for (int pin : leds) pinMode(pin, OUTPUT);

  for (int pin : leds) {
    digitalWrite(pin, 1);
    delay(500);
  }

  for (int pin : leds) {
    digitalWrite(pin, 0);
    delay(500);
  }

  showNumber();
}

void loop() {
  bool button1 = digitalRead(kn1);
  bool button2 = digitalRead(kn2);
  bool button3 = digitalRead(kn3);
  int oldNumber = number;

  if (button1 == 0 && lastKn1 == 1 && number < 63) number++;
  if (button2 == 0 && lastKn2 == 1 && number > 0) number--;
  if (button3 == 0 && lastKn3 == 1 && number > 0) invers = !invers;

  lastKn1 = button1;
  lastKn2 = button2;
  lastKn3 = button3;

  showNumber();
  delay(50);
}
