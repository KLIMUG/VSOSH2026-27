#define pot_pin 28
#define kn_1 15
#define kn_2 14
#define kn_3 13 

#define led_1 9
#define led_2 10
#define led_3 11
#define led_4 12

#define led_r1 8
#define led_r2 7

const int leds[4] = {led_1, led_2, led_3, led_4};
const int kns[3] = {kn_1, kn_2, kn_3};
const int chs_mode_2[5] = {0, 1, 2, 4, 8};

uint8_t mode = 0;
bool last_kn_1 = 1;
bool last_kn_2 = 1;
bool last_kn_3 = 1;
uint8_t ch = 0;
int t = millis();
int delay_time_2 = 1000;
int ch_modul_2 = 0;
int light = 

void show_in_bin(int a, int light){
  int n = a;
      for (int i = 0; i < 4; i++){
          analogWrite(leds[i], light * (n%2));
          n = n / 2;
      }
}

void setup() {
  for (int led : leds) pinMode(led, OUTPUT);
  for (int kn : kns) pinMode(kn, INPUT_PULLUP);
  for (int led : leds) digitalWrite(led, HIGH);
}

void loop() {
  switch (mode){
    case 0:
      digitalWrite(led_r1, 0);
      digitalWrite(led_r2, 1);
      ch = analogRead(pot_pin)/(1023/15);
      show_in_bin(ch, 667);
      break;

    case 1:
      digitalWrite(led_r1, 1);
      digitalWrite(led_r2, 0);
      ch = chs_mode_2[ch_modul_2];
      if (millis() - t > delay_time_2){
        if (ch_modul_2 < 4) ch_modul_2++; else ch_modul_2 = 0;
        t = millis();
      }

      if (digitalRead(kn_1) == 0 and last_kn_1 != digitalRead(kn_1))
        if (delay_time_2 < 2000) delay_time_2 = delay_time_2 + 100;
      if (digitalRead(kn_2) == 0 and last_kn_2 != digitalRead(kn_2))
        if (delay_time_2 > 100) delay_time_2 = delay_time_2 - 100;

      show_in_bin(ch, 512);

    case 2:
      digitalWrite(led_r1, 1);
      digitalWrite(led_r2, 1);
      if (digitalRead(kn_1) == 0 and last_kn_1 != digitalRead(kn_1))
        if (ch < 15) ch ++;
      if (digitalRead(kn_2) == 0 and last_kn_2 != digitalRead(kn_2))
        if (ch > 0) ch = ch - 1;

      last_kn_1 = digitalRead(kn_1);
      last_kn_2 = digitalRead(kn_2);   

      show_in_bin(ch, analogRead(pot_pin)/4);   

  }

  if (digitalRead(kn_3) == 0 and last_kn_3 != digitalRead(kn_3))
    if (mode < 2) mode++; else mode = 0;

  last_kn_3 = digitalRead(kn_3);

  Serial.print("M");
  Serial.print(mode);
  Serial.print(" | ");
  Serial.print(ch);
  Serial.print(" | ");
  Serial.print(ch, BIN);
  Serial.print(" | ");
  Serial.print(analogRead(pot_pin)/4);
  Serial.print(" | ");
  Serial.println(delay_time_2/100);

  delay(50);
}


