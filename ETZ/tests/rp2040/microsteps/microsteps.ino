#define dir_pin 15
#define step_pin 14
#define ms1_pin 13
#define ms2_pin 12
int t = 0;
int mode = 0;
void setup(){
  Serial.begin(921600);
  pinMode(dir_pin, OUTPUT);
  pinMode(step_pin, OUTPUT);
  pinMode(ms1_pin, OUTPUT);
  pinMode(ms2_pin, OUTPUT);
  digitalWrite(dir_pin, 0);
}

void loop(){
  t++;
  digitalWrite(step_pin, 1);
  delayMicroseconds(70);
  digitalWrite(step_pin, 0);
  delayMicroseconds(70);
  if (t > 5 * 1000000/300){
    t = 0;
    switch (mode) {
      case 0:
        digitalWrite(ms1_pin, 0);
        digitalWrite(ms2_pin, 0);
        mode = 1;
        break;
      case 1:
        digitalWrite(ms1_pin, 1);
        digitalWrite(ms2_pin, 0);
        mode = 2;
        break;
      case 2:
        digitalWrite(ms1_pin, 0);
        digitalWrite(ms2_pin, 1);
        mode = 3;
        break;
      case 3:
        digitalWrite(ms1_pin, 1);
        digitalWrite(ms2_pin, 1);
        mode = 0;
        break;
    }
    Serial.print("1/");
    Serial.println(pow(2, (mode + 3)));
  }
}