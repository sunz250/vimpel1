#include <Servo.h>
#include <Ultrasonic.h>
#include <Pixy.h>
#include <SPI.h>
#define ENA 6
#define IN1 4
#define IN2 5
#define my_period 1700
Ultrasonic ultrasonic_left(8, 7);
Ultrasonic ultrasonic_right(10,9);
Servo myservo;
Pixy pixy;
uint8_t power = 120;
uint16_t blocks; 
static int f;
char buf[32]; 
int number = 0;
uint16_t my_timer = 500;
int dist_left, dist_right, i;
void setup() {
  Serial.begin(9600);
  myservo.attach(A0);
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pixy.init();
  myservo.write(82);
}
void PID();
void MOVER();
void MOVEL();
void Time();
void loop() {  
  analogWrite(ENA, power);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);   
  blocks = pixy.getBlocks();   
  PID();
  for (f = 0; f < blocks; f++) {
    if (pixy.blocks[f].signature == 1) {
      MOVER();
    }
    if (pixy.blocks[f].signature == 2) {
      MOVEL();
    }
  }
  Time();
  delay(70);
}
void PID() {
  float err, ust, err_old, u, propor_part, difference_part, kp = 0.2, kd = 0.02;
  millis();
  dist_right = ultrasonic_right.distanceRead();
  dist_left = ultrasonic_left.distanceRead();
  ust = (dist_right + dist_left) / 2;
  err = dist_right - ust;
  propor_part = err * kp;
  difference_part = kd * (err - err_old);
  u = propor_part + difference_part;
  u = -u;
  err_old = err;
  if (u > 30) {
    u = 30;  
  }
  if (u < -40) {
    u = -40;
  }
  Serial.println(u); 
  myservo.write(u + 82);
}
void Time(){
  if (millis() - my_timer >= my_period) {
    if (dist_right > 125 || dist_left > 125) {
      number++; 
      my_timer = millis();
      if (number == 11){
        while (millis() - my_timer < 1000){
         PID();
        }
        delay(150);
        power = 0;
      }
    }
  }
}
void MOVEL() {
  if (pixy.blocks[f].height > 50) {
    myservo.write(95);
  }
  else {
    return;
   }
  }
void MOVER() {
  if (pixy.blocks[f].height > 50) {
    myservo.write(55);
    }
  else {
    return;
    }
}