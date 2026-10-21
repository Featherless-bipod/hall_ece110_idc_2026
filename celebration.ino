// ---------- Celebration functions ----------

// --- LIGHT SHOW ---
void lightShow(int er, int eg, int eb, int ir, int ig,  int ib) {
  void set_ex_RGB(int r, int g, int b) {
    // Set RGB LED pins based on high=bright
    digitalWrite(er, 0==r);
    digitalWrite(eg, 0==g);
    digitalWrite(eb, 0==b);
  }
  void set_RGBi(int r, int g, int b) {
    // Set RGB LED pins based on low=bright (default)
    analogWrite(ir, 255-r);
    analogWrite(ig, 255-g);
    analogWrite(ib, 255-b);
  }
  for (int i = 0; i < 5; i++) {  //flash for 5 * 1.5 seconds
    set_ex_RGB(255, 0, 0);  //red
    set_RGBi(255, 0, 0);
    delay(100);

    set_ex_RGB(0, 255, 0);  //green
    set_RGBi(0, 255, 0);
    delay(100);

    set_ex_RGB(0, 0, 255);  //blue
    set_RGBi(0, 0, 255);
    delay(100);     //1.5 seconds
  }
  set_ex_RGB(0, 0, 0);
  set_RGBi(0, 0, 0);
}



// --- DANCE ---
void dance() {
  for (int i = 0; i < 5; i++) {
    servoLeft.writeMicroseconds(1300);
    servoRight.writeMicroseconds(1300);
    delay(200);
    servoLeft.writeMicroseconds(1700);
    servoRight.writeMicroseconds(1700);
    delay(200);

    servoLeft.writeMicroseconds(1300);
    servoRight.writeMicroseconds(1700);
    delay(200);
    servoLeft.writeMicroseconds(1700);
    servoRight.writeMicroseconds(1300);
    delay(200);
  }
  servoLeft.writeMicroseconds(1500);
  servoRight.writeMicroseconds(1500);
}



void playTwinkle() {
  const byte twinkleNotes[14] = {
    223, 223, 230, 230, 220, 220, 230,  // C C G G A A G
    228, 228, 227, 227, 225, 225, 223,  // F F E E D D C
  };
  for (int k = 0; k < 14; k++) {
    bool isLong = (k % 7 == 6);                       // last note of each line is held
    byte dur = isLong ? 212 : 211;                    // quarter note : eighth note
    byte oct = (twinkleNotes[k] == 220) ? 216 : 215;  // the LCD's octaves start at A, so A is one octave up

    mySerial.write(dur);
    mySerial.write(oct);
    mySerial.write(twinkleNotes[k]);

    delay(isLong ? 550 : 275);  // note length x 1.1 so each note finishes before the next
  }
}
