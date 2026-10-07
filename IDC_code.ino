#include <Servo.h>
#include <SoftwareSerial.h>

//Pins for QTI connections on board
#define lineSensor1 47  // Left
#define lineSensor2 51  //middle
#define lineSensor3 52  // Right

//define onboard RGB pins
#define redpin 45
#define greenpin 46
#define bluepin 44

#define ex_redpin 4
#define ex_greenpin 3
#define ex_bluepin 2

#define TxPin 14
SoftwareSerial mySerial = SoftwareSerial(255, TxPin); 

//============================================= WEEK 2 CODE =========================================================

//define team
const int team_int = 80;

//define servo
Servo servoLeft;
Servo servoRight;

//define threshold and counts
int static count = 0;
int threshold = 300;

//number of dinosaur sightings
int static dinosaur_count = 0;

//define hall sensor constants
const int Hall_In = 0;
const float VCC = 5.0;
const float Hall_sensitivity = 0.005;  // 5 mV/G
const float hall_upper_threshold = -5;
const float hall_lower_threshold = -20;

//scanning function
void scan_hall(){
int upper_count = 0;
int lower_count = 0;
long start_time = millis();
while (millis()-start_time < 200){
  float Hall_Reading = analogRead(Hall_In);
  float Hall_Voltage = Hall_Reading * 5.0 / 1023.0;
  float Hall_Gauss = (Hall_Voltage - (VCC / 2)) / 0.005;
  Serial.println(Hall_Gauss);
  if (Hall_Gauss > hall_upper_threshold){
    upper_count++;
    //Serial.println(1);
  }
  else if (Hall_Gauss < hall_lower_threshold){
    lower_count++;
    //Serial.println(-1);
  }
  else{
    //Serial.println(0);
  }
}
  if(upper_count > 5 || lower_count > 5){
    dinosaur_count++;
   set_ex_RGB(0,1,0);
  }
  else{
   set_ex_RGB(1,0,0);
  }
  Serial.println(dinosaur_count);
  delay(500);
  set_ex_RGB(0,0,0);
}

int report_values(){
  set_ex_RGB(0,0,1);
  delay(1000);
int val = team_int + dinosaur_count;
long start_time = millis();
Serial2.flush();
 Serial2.print(char(val));
 Serial.println(char(val));
 set_ex_RGB(0,0,0);
process_incoming(val);
return 0;
}


///===================================================== WEEK 3 CODE ==========================================================
int score_arr[5] = {-1,-1,-1,-1,-1}; 

void display(){
  
  String text = "[";
  for (int i = 0; i < 5; i++){
    text += score_arr[i];
    if (i < 4) text += " ";
  }
  text += "]";
  mySerial.write(12);
  delay(10);
  mySerial.write(22);
  delay(10);
  mySerial.print(text);
}

bool scoresFilled(){
  for(int i  = 0; i < 5; i++){
    if(score_arr[i] <0) return false;
  }
  return true;
}

void process_incoming(char in){
  char code = in;
  if (code < 50 || code > 99) return;   // ignore garbage
  int team_num = code / 10 - 5;
  int team_score = code % 10;
  score_arr[team_num] = team_score;
}

void read_xbee(){
  while (Serial2.available()){
    char incoming = Serial2.read();
    set_ex_RGB(255,255,255);
    delay(250);
    set_ex_RGB(0,0,0);
    Serial.print("incoming: ");
    Serial.println((char)incoming);
    process_incoming(incoming);
  }
}

//==================================================== WEEK 1 CODE ======================================================
//function for when needing to wait for a certain amount of time not moving
void wait(int time) {
servoLeft.writeMicroseconds(1500);
servoRight.writeMicroseconds(1500);
delay(time);
}

//funcition called for when unlikely scenarios happen
void broken() {
servoLeft.detach();
servoRight.detach();
}

void finish_state() {
  servoLeft.detach();
  servoRight.detach();
  set_RGBi(255, 255, 255);
  scan_hall();

  int error_status = report_values();
  delay(250);
  set_RGBi(0,0,0);
  Serial.println("Error status: ");
  Serial.println(error_status);
  servoLeft.detach();
  servoRight.detach();
  display();
  while(!scoresFilled()){
    read_xbee();
    display();
  }
  while(true){
    delay(1000); 
  }
}


//function called for when meeting a hash
void stop() {
//increase count of hash everytime reaching one
count++;
wait(1000);
//call switch statement to define light color depending on the hash mark you are on

switch (count) {
  case 1:
    set_RGBi(255, 0, 255);
    scan_hall();
    break;
  case 2:
    set_RGBi(0, 255, 0);
    scan_hall();
    break;
  case 3:
    set_RGBi(0, 0, 255);
    scan_hall();
    break;
  case 4:
    set_RGBi(255, 0, 0);
    scan_hall();
    break;
  case 5:
    finish_state();
}
delay(1000);
//return RGB to having no color
set_RGBi(0, 0, 0);
//move bot forward off of the hash first before returning to scanning to prevent getting stuck on hash
servoLeft.writeMicroseconds(1550);
servoRight.writeMicroseconds(1450);
delay(300);
}

//MOVEMENT CONDITIONS
//function for turning left a little
void left_min() {
servoLeft.writeMicroseconds(1500);
servoRight.writeMicroseconds(1350);
}
//function for turning left a lot
void left_max() {
servoLeft.writeMicroseconds(1500);
servoRight.writeMicroseconds(1300);
}
//function for turning right a little
void right_min() {
servoLeft.writeMicroseconds(1650);
servoRight.writeMicroseconds(1500);
}
//function for turning right a lot
void right_max() {
servoLeft.writeMicroseconds(1700);
servoRight.writeMicroseconds(1500);
}
//function for going forward
void forward() {
servoLeft.writeMicroseconds(1550);
servoRight.writeMicroseconds(1450);
}

//setup block
void setup() {

Serial.begin(9600);
Serial2.begin(9600);
mySerial.begin(9600);
mySerial.write(12);
mySerial.write(18);
mySerial.write(22);
servoLeft.attach(12);
servoRight.attach(11);

//set pinmodes for external RGB
pinMode(ex_redpin, OUTPUT);
pinMode(ex_greenpin, OUTPUT);
pinMode(ex_bluepin, OUTPUT);

//define RGB modes
pinMode(redpin, OUTPUT);
pinMode(greenpin, OUTPUT);
pinMode(bluepin, OUTPUT);
//set default RGB to
set_RGBi(0, 0, 0);
set_ex_RGB(0,0,0);
}

void loop() {
//read from qti sensor
//scan_hall();
int qti1 = rcTime(lineSensor1);
int qti2 = rcTime(lineSensor2);
int qti3 = rcTime(lineSensor3);
//define binary variables for recording qti input
int sLeft;
int sMiddle;
int sRight;
//evaluate qti and translate to binary variables
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
//define state as sum of binary variables
int state = 7 - (4 * sLeft + 2 * sMiddle + sRight);

//switch statement for different cases  of state
switch (state) {
  case 0:  //DDD
    stop();
    break;
  case 1:  //DDL
    left_min();
    break;
  case 2:  //DLD
    broken();
    break;
  case 3:  //DLL
    left_max();
    break;
  case 4:  //LDD
    right_min();
    break;
  case 5:  //LDL
    forward();
    break;
  case 6:  //LLD
    right_max();
    break;
  case 7:  //LLL
    broken();
    break;
}
read_xbee();

//if(!scoresFilled()) read_xbee();

}


//function for changing RGB light vaues
void set_RGBi(int r, int g, int b) {
// Set RGB LED pins based on low=bright (default)
analogWrite(redpin, 255-r);
analogWrite(greenpin, 255-g);
analogWrite(bluepin, 255-b);
}
//wrapper function for inverting RGB input values.
void set_ex_RGB(int r, int g, int b) {
// Set RGB LED pins based on high=bright
digitalWrite(ex_redpin, 0==r);
digitalWrite(ex_greenpin, 0==g);
digitalWrite(ex_bluepin, 0==b);
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
