// Laser Modem
// (c) Nino Ivanov
// GNU Affero General Public License v3.0

// Operation is half-duplex, i.e. either information
// is sent, or it is received, but not both simultaneously.

// Wiring: Connect Laser Negative to Arduino Ground.
// Connect Laser positive to Arduino pin 2 (on an
// Arduino Nano, these two pins are adjacent).
// Connect one pin of the photo resistor to 5V.
// Connect the other pin of the photo resistor to a
// 10K-resistor, so, that there is a segment of wire
// where the photo-resistor connects to the 10K-resistor
// (segment A) and a segment of wire on the other end
// of the 10K-resistor. Connect segment A to Arduino
// pin A0. Connect segment B to Arduino Ground.


const int laserpin = 2;

int photoval = 0;          // 0-1023
int senseval = 0;
int prevsense = 0;
int pprevsense = 0;
int ppprevsense = 0;
int cmdsignal = 0;
int bitval = 0;
int startcomm = 0;
int prevstartcomm = 0;
int readerswitch = 0;
int lastlight = 0;
int lastdarkness = 0;
int bitcounter = 0;
int sensecounter = 0;

int THRESHLIGHT = 850; // overwritten below, but sensible...

void laserout(char c) {

  digitalWrite(laserpin, HIGH);
  delay(400);

  if ((c & 128) > 0) {
    digitalWrite(laserpin, HIGH); delay(90); digitalWrite(laserpin, LOW); delay(10);
    digitalWrite(laserpin, LOW); delay(100);
  } else {
    digitalWrite(laserpin, LOW); delay(100);
    digitalWrite(laserpin, HIGH); delay(90); digitalWrite(laserpin, LOW); delay(10);
  }

  if ((c & 64) > 0) {
    digitalWrite(laserpin, HIGH); delay(90); digitalWrite(laserpin, LOW); delay(10);
    digitalWrite(laserpin, LOW); delay(100);
  } else {
    digitalWrite(laserpin, LOW); delay(100);
    digitalWrite(laserpin, HIGH); delay(90); digitalWrite(laserpin, LOW); delay(10);
  }

  if ((c & 32) > 0) {
    digitalWrite(laserpin, HIGH); delay(90); digitalWrite(laserpin, LOW); delay(10);
    digitalWrite(laserpin, LOW); delay(100);
  } else {
    digitalWrite(laserpin, LOW); delay(100);
    digitalWrite(laserpin, HIGH); delay(90); digitalWrite(laserpin, LOW); delay(10);
  }

  if ((c & 16) > 0) {
    digitalWrite(laserpin, HIGH); delay(90); digitalWrite(laserpin, LOW); delay(10);
    digitalWrite(laserpin, LOW); delay(100);
  } else {
    digitalWrite(laserpin, LOW); delay(100);
    digitalWrite(laserpin, HIGH); delay(90); digitalWrite(laserpin, LOW); delay(10);
  }

  if ((c & 8) > 0) {
    digitalWrite(laserpin, HIGH); delay(90); digitalWrite(laserpin, LOW); delay(10);
    digitalWrite(laserpin, LOW); delay(100);
  } else {
    digitalWrite(laserpin, LOW); delay(100);
    digitalWrite(laserpin, HIGH); delay(90); digitalWrite(laserpin, LOW); delay(10);
  }

  if ((c & 4) > 0) {
    digitalWrite(laserpin, HIGH); delay(90); digitalWrite(laserpin, LOW); delay(10);
    digitalWrite(laserpin, LOW); delay(100);
  } else {
    digitalWrite(laserpin, LOW); delay(100);
    digitalWrite(laserpin, HIGH); delay(90); digitalWrite(laserpin, LOW); delay(10);
  }

  if ((c & 2) > 0) {
    digitalWrite(laserpin, HIGH); delay(90); digitalWrite(laserpin, LOW); delay(10);
    digitalWrite(laserpin, LOW); delay(100);
  } else {
    digitalWrite(laserpin, LOW); delay(100);
    digitalWrite(laserpin, HIGH); delay(90); digitalWrite(laserpin, LOW); delay(10);
  }

  if ((c & 1) > 0) {
    digitalWrite(laserpin, HIGH); delay(90); digitalWrite(laserpin, LOW); delay(10);
    digitalWrite(laserpin, LOW); delay(100);
  } else {
    digitalWrite(laserpin, LOW); delay(100);
    digitalWrite(laserpin, HIGH); delay(90); digitalWrite(laserpin, LOW); delay(10);
  }

  digitalWrite(laserpin, LOW);
  delay(800);

}

void setup(){
  Serial.begin(9600);
  pinMode(laserpin, OUTPUT); // laser attached
  pinMode(A0, INPUT); // photoresistor is on A0

  // Turn on the laser, expecting
  // the same from the other party.
  Serial.println("Calibrating light threshold. Input desired threshold,");
  Serial.println("by typing the (not displayed) digits of the number,");
  Serial.println("which should be about 10-20% lower than registered");
  Serial.println("laser intensity. After calibration, the continuous");
  Serial.println("laser tuns off and the system is ready for transmission.");

  digitalWrite(laserpin, HIGH);
  Serial.flush();
  while (!Serial.available()) {
    Serial.println(analogRead(A0));
    delay(200);
  }
  THRESHLIGHT = Serial.parseInt();
  Serial.print(THRESHLIGHT);
  Serial.println("");
  Serial.flush();
  digitalWrite(laserpin, LOW);
  delay(1000);
}

void loop(){

  // if (THRESHLIGHT < 350) {
  //  THRESHLIGHT = 350;
  // }

//  digitalWrite(laserpin, LOW);
//  while ((analogRead(A0) <= THRESHLIGHT) && (startcomm == 0)) {
//    if (Serial.available()) {
//      break;
//    }
//  }


  if ((Serial.available()) && (startcomm == 0)) {
    char j = Serial.read();
    if (j == '#') {
      Serial.println("");
      Serial.print("registered intensity: ");
      Serial.println(analogRead(A0));
      Serial.println("laserbeam for 4 seconds");
      digitalWrite(laserpin, HIGH);
      delay(4000);
      digitalWrite(laserpin, LOW);
    } else {
      Serial.print(j);
      laserout(j);
    }
  }


  if(analogRead(A0) > THRESHLIGHT) {
    bitcounter = 0;
    sensecounter = 0;
    while (1) {

      prevstartcomm = startcomm;

      ppprevsense = pprevsense;
      pprevsense = prevsense;
      prevsense = senseval;

      
      photoval = analogRead(A0);
      if (photoval > THRESHLIGHT) {
        senseval = 1;
        readerswitch++;
        lastlight = photoval;
      } else {
        senseval = 0;
        readerswitch++;
        lastdarkness = photoval;
      }
      delay(50);

  // Serial.print(" photo=");
       // Serial.print("    "); Serial.println(photoval);
  // Serial.print(" startcomm=");
  // Serial.print(startcomm);
  // Serial.print(" ppprevsense=");
  // Serial.print(ppprevsense);
  // Serial.print(" pprevsense=");
  // Serial.print(pprevsense);
  // Serial.print(" prevsense=");
  // Serial.print(prevsense);
  // Serial.print(" senseval=");
  // Serial.print(senseval);
  // Serial.print(" readerswitch =");
  // Serial.println(readerswitch);
      if ((ppprevsense > 0) && (pprevsense > 0) && (prevsense > 0) && (senseval > 0) && (readerswitch == 2)) {
        startcomm = 1;
        // Serial.println("startcomm becomes 1");
        readerswitch = 0;
      } else if ((ppprevsense == 0) && (pprevsense == 0) && (prevsense == 0) && (senseval == 0)) {
        startcomm = 0;
        // Serial.println("startcomm becomes 0");
        // THRESHLIGHT = (lastlight + lastdarkness) / 2; // automatic recalibration
      }

      // if byte construction has started, and a value is sensed,
      // then decide is it a "dash" or a "dot"; TWO sensings are
      // needed for ONE bit: 1, 0 for 1; and 0, 1 for 0; to avoid
      // interpreting "natural" laser states as a bit signal (00 or 11).
      if (startcomm == 1) {
        if ((prevsense == 1) && (senseval == 0) & (readerswitch == 2)) {
          bitval = bitval * 2;
          bitval = bitval + 1;
          bitcounter++; readerswitch = 0;
          // Serial.println("bit = 1");
        } else if ((prevsense == 0) && (senseval == 1) & (readerswitch == 2)) {
          bitval = bitval * 2;
          bitcounter++; readerswitch = 0;
          // Serial.println("bit = 0");
        }
      }

      if (readerswitch == 2) {
          bitcounter++; readerswitch = 0;
          // but that actually means a DEFECT, and maybe in future, I
          // should mark somehow the entire byte as "wrongly received".
          // Imprecise, but easy: I could turn bitval negative, assuming
          // no two mistakes will likely hit the same byte. Or set a flag.
      }

      delay(50);

      sensecounter++;
  
      if (((startcomm == 0) & (prevstartcomm == 1)) || (bitcounter > 7) || (sensecounter > 25) ||
          ((ppprevsense == 0) && (pprevsense == 0) && (prevsense == 0) && (senseval == 0))){
        // Serial.println("----------------");
        // Serial.print("BYTE READ: ");
        // Serial.println(bitval);
        // Serial.println("----------------");
        Serial.print((char) bitval);
        bitval = 0;
        prevstartcomm = 0;
        senseval = 0;
        prevsense = 0;
        pprevsense = 0;
        ppprevsense = 0;
        bitcounter = 0;
        sensecounter = 0;
        break;
      }
    }
  }
}
