#define dir1_pin 15
#define step1_pin 14
#define dir2_pin 13
#define step2_pin 12

const long r_steps = 3000;
const int step_delay_us = 100;

long cur_x = r_steps;
long cur_y = 0;


void setup() {
  Serial.begin(115200);
  pinMode(dir1_pin, OUTPUT);
  pinMode(step1_pin, OUTPUT);
  pinMode(dir2_pin, OUTPUT);
  pinMode(step2_pin, OUTPUT);
  digitalWrite(step1_pin, LOW);
  digitalWrite(step2_pin, LOW);
}


void pulse(int pin) {
  digitalWrite(pin, HIGH);
  delayMicroseconds(step_delay_us);
  digitalWrite(pin, LOW);
  delayMicroseconds(step_delay_us);
}


// перемещение на (dx, dy) шагов, оси двигаются одновременно (Брезенхэм)
void move_rel(long dx, long dy) {
  digitalWrite(dir1_pin, dx >= 0 ? HIGH : LOW);
  digitalWrite(dir2_pin, dy >= 0 ? HIGH : LOW);
  long ax = labs(dx);
  long ay = labs(dy);
  long n = max(ax, ay);
  long ex = 0, ey = 0;
  for (long i = 0; i < n; i++) {
    ex += ax;
    ey += ay;
    if (ex >= n) { ex -= n; pulse(step1_pin); }
    if (ey >= n) { ey -= n; pulse(step2_pin); }
  }
}


void loop() {
  for (int angle = 1; angle <= 360; angle++) {
    float a = angle * PI / 180.0;
    long target_x = lround(cos(a) * r_steps);
    long target_y = lround(sin(a) * r_steps);
    move_rel(target_x - cur_x, target_y - cur_y);
    cur_x = target_x;
    cur_y = target_y;
  }
}
