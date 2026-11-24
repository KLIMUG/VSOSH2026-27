#include <Servo.h>
Servo srv;
#define dir1_pin 15
#define step1_pin 14
#define dir2_pin 13
#define step2_pin 12

// листок А4 8500 * 10500

void move_ab(int a, int b, int step_delay){
  const unsigned int steps_a = a < 0 ? 0u - static_cast<unsigned int>(a) : a;
  const unsigned int steps_b = b < 0 ? 0u - static_cast<unsigned int>(b) : b;
  const unsigned int ticks = steps_a > steps_b ? steps_a : steps_b;
  if (ticks == 0) return;

  if (steps_a != 0) digitalWrite(dir1_pin, a > 0 ? HIGH : LOW);
  if (steps_b != 0) digitalWrite(dir2_pin, b > 0 ? HIGH : LOW);
  delayMicroseconds(5);

  unsigned int acc_a = ticks / 2;
  unsigned int acc_b = ticks / 2;
  for (unsigned int tick = 0; tick < ticks; ++tick){
    acc_a += steps_a;
    acc_b += steps_b;
    const bool pulse_a = acc_a >= ticks;
    const bool pulse_b = acc_b >= ticks;
    if (pulse_a){
      acc_a -= ticks;
      digitalWrite(step1_pin, HIGH);
    }
    if (pulse_b){
      acc_b -= ticks;
      digitalWrite(step2_pin, HIGH);
    }
    delayMicroseconds(step_delay);
    if (pulse_a) digitalWrite(step1_pin, LOW);
    if (pulse_b) digitalWrite(step2_pin, LOW);
    delayMicroseconds(step_delay);
  }
}


void sound(int time){
  for(int i = 0; i < time*50; i++){
    digitalWrite(step1_pin, 1);
    digitalWrite(step2_pin, 1);
    delayMicroseconds(20);
    digitalWrite(step1_pin, 0);
    digitalWrite(step2_pin, 0);
    delayMicroseconds(20);
  }
}

void setup() {
  Serial.begin(921600);
  srv.attach(16);
  srv.write(90);
  pinMode(dir1_pin, OUTPUT);
  pinMode(step1_pin, OUTPUT);
  pinMode(dir2_pin, OUTPUT);
  pinMode(step2_pin, OUTPUT);
  int move = 10;
  while (move != 0){
    if (Serial.available()){
      move = Serial.parseInt();
      if (move > 10000) move = 10000;
      if (move < -10000) move = -10000;
      move_ab(move, move, 90);
      while (Serial.available()) {
        Serial.read();
      }
    }
  }

  sound(250);

  move = 10;
  while (move != 0){
    if (Serial.available()){
      move = Serial.parseInt();
      if (move > 11000) move = 10000;
      if (move < -11000) move = -10000;
      move_ab(-move, move, 90);
      while (Serial.available()) {
        Serial.read();
      }
    }
  }

  sound(250);

  srv.write(0);
  move_ab(8000, -3000, 100);
  srv.write(90);
  move_ab(-3000, 3000, 100);
  srv.write(0);
  move_ab(-8000, 3000, 100);
  srv.write(90);
  move_ab(500, -500, 100);
  srv.write(0);
  move_ab(8000, -3000, 100);
  srv.write(90);
  move_ab(-4000, 1500, 100);
  srv.write(0);
  move_ab(-1500, -3000, 100);
}

void loop() {

}

