HELLLOOOOO my goat


#include <Servo.h>

//Pins for QTI connections on board
#define lineSensor1 47  // Left
#define lineSensor2 51  //middle
#define lineSensor3 52  // Right

#define redpin 45
#define greenpin 46
#define bluepin 44

Servo servoLeft;
Servo servoRight;

int static count = 0;
int threshold = 300;
static int light = 0;

void wait(int time) {
 servoLeft.writeMicroseconds(1500);
 servoRight.writeMicroseconds(1500);
 delay(time);
}

void broken() {
 servoLeft.detach();
 servoRight.detach();
 Serial.println("AHHHHHH");
}
void stop() {
 count++;
 wait(1000);
 switch(count){
   case 1:
     set_RGBi(255,0,255);
     break;
   case 2:
     set_RGBi(0,255,0);
     break;
   case 3:
     set_RGBi(0,0,255);
     break;
   case 4:
     set_RGBi(255,0,0);
     break;
   case 5:
     set_RGBi(255,255,255);
     servoLeft.detach();
     servoRight.detach();
 }
 delay(1000);
 set_RGBi(0,0,0);
 servoLeft.writeMicroseconds(1550);
 servoRight.writeMicroseconds(1450);
 delay(300);
}
void left_min() {
 servoLeft.writeMicroseconds(1500);
 servoRight.writeMicroseconds(1350);
}
void left_max() {
 servoLeft.writeMicroseconds(1500);
 servoRight.writeMicroseconds(1300);
}
void right_min() {
 servoLeft.writeMicroseconds(1650);
 servoRight.writeMicroseconds(1500);
}
void right_max() {
 servoLeft.writeMicroseconds(1700);
 servoRight.writeMicroseconds(1500);
}
void forward() {
 servoLeft.writeMicroseconds(1550);
 servoRight.writeMicroseconds(1450);
}

void setup() {
 Serial.begin(9600);
 servoLeft.attach(12);
 servoRight.attach(11);

 pinMode(redpin, OUTPUT);
 pinMode(greenpin, OUTPUT);
 pinMode(bluepin, OUTPUT);
 set_RGBi(0,0,0);
}

void loop() {
 int qti1 = rcTime(lineSensor1);
 int qti2 = rcTime(lineSensor2);
 int qti3 = rcTime(lineSensor3);
 int sLeft;
 int sMiddle;
 int sRight;
 //delay(50);
 // Serial.println(qti1);
 // Serial.println(qti2);
 // Serial.println(qti3);
 if (qti1 < threshold) {
   sLeft = 0;
 } else {
   sLeft = 1;
 }
 if (qti2 < threshold) {
   sMiddle = 0;
 } else {
   sMiddle = 1;
 }
 if (qti3 < threshold) {
   sRight = 0;
 } else {
   sRight = 1;
 }
 int state = 7 - (4 * sLeft + 2 * sMiddle + sRight);
 //Serial.println(state);

 switch (state) {
   case 0:
     stop();
     break;
   case 1:
     left_min();
     break;
   case 2:
     broken();
     break;
   case 3:
     left_max();
     break;
   case 4:
     right_min();
     break;
   case 5:
     forward();
     break;
   case 6:
     right_max();
     break;
   case 7:
     broken();
     break;
 }
 //delay(100);
}

void set_RGB(int r, int g, int b) {
// Set RGB LED pins based on low=bright (default)
 analogWrite(redpin, r);
 analogWrite(greenpin, g) ;
 analogWrite(bluepin, b);
}
void set_RGBi(int r, int g, int b) {
// Set RGB LED pins based on high=bright
set_RGB(255 - r, 255 - g, 255 - b);
}

//Defines funtion 'rcTime' to read value from QTI sensor
// From Ch. 6 Activity 2 of Robotics with the BOE Shield for Arduino
long rcTime(int pin) {
 pinMode(pin, OUTPUT);     // Sets pin as OUTPUT
 digitalWrite(pin, HIGH);  // Pin HIGH
 delay(1);                 // Waits for 1 millisecond
 pinMode(pin, INPUT);      // Sets pin as INPUT
 digitalWrite(pin, LOW);   // Pin LOW
 long time = micros();     // Tracks starting time
 while (digitalRead(pin))
   ;                      // Loops while voltage is high
 time = micros() - time;  // Calculate decay time
 return time;             // Return decay time
}