int led [] = {4, 5, 6, 7, 8, A0, A1, A2, 12, 13};
int lcd = 11;
int mosfet = 9;
int pulseO = 10, pulseI = A3;
int serialLedIn = 2, serialLedOut = 3;

boolean onOffMosfet = false, onOffLcd = false;
int count = 0;

int pulseDelay = 100;

unsigned long nextBlackout = millis();
int mosFreq = 100, mosPower = 255, lcdFreq = 100;

void setup() {

  pinMode (serialLedIn, OUTPUT);
  pinMode (serialLedOut, OUTPUT);

  for (int i = 0; i < 10; i++) {
    pinMode (led[i], OUTPUT);
    digitalWrite(led[i], HIGH);
  }
  digitalWrite(serialLedIn, HIGH);
  digitalWrite(serialLedOut, HIGH);
  delay(250);
  for (int i = 0; i < 10; i++) {
    digitalWrite(led[i], LOW);
  }
  digitalWrite(serialLedIn, LOW);
  digitalWrite(serialLedOut, LOW);
  delay(250);

  pinMode (lcd, OUTPUT);
  pinMode (mosfet, OUTPUT);
  pinMode (pulseO, OUTPUT);

  Serial.begin(115200);
}

void loop() {

  int ran = random(100);
  if (ran < 10) {
    for (int j = 0; j < random(20) + 3; j++) {
      for (int i = 0; i < 10; i++) {
        int r = random(100);
        if (r < 33) {
          digitalWrite(led[i], HIGH);
        } else {
          digitalWrite(led[i], LOW);
        }
        delay(1);
      }
    }
    nextBlackout = millis() + map(pulseDelay, 0, 200, 3000, 15000);
    //nextBlackout = millis() + pulseDelay * 10;
    delay(pulseDelay);
    Serial.print('A');
  }
  if (count % mosFreq == 0) {
    if (onOffMosfet == true) {
      analogWrite(mosfet, mosPower);
      Serial.print("mosfet on ");
      Serial.println(mosPower);
    } else {
      analogWrite(mosfet, 0);
      Serial.println("mosfet off");
    }
    onOffMosfet = !onOffMosfet;
  }
  if (count % lcdFreq == 0) {
    digitalWrite(lcd, onOffLcd);
    onOffLcd = !onOffLcd;
  }
  count++;

  if (millis() > nextBlackout) {
    for (int i = 0; i < 10; i++) {
      digitalWrite(led[i], LOW);
    }
  }


  delay(random(200));
}

void serialEvent() {

  digitalWrite(serialLedIn, HIGH);
  char incoming = Serial.read();
  if (incoming == 'A') {
    for (int j = 0; j < random(20) + 3; j++) {
      for (int i = 0; i < 10; i++) {
        int r = random(100);
        if (r < 33) {
          digitalWrite(led[i], HIGH);
        } else {
          digitalWrite(led[i], LOW);
        }
        delay(1);
      }
    }
    nextBlackout = millis() + map(pulseDelay, 0, 200, 3000, 15000);
    //nextBlackout = millis() + pulseDelay * 10;
    delay(pulseDelay);
    Serial.print('A');
  }
  if (incoming == 'M') {
    // set delay
    delay(5);
    incoming = Serial.read();
    int val = constrain(incoming, 48, 57); // ascii characters 0-9
    mosFreq = map(val, 48, 57, 2, 10000);
    Serial.print('M');
    Serial.print(incoming);
    delay(5);
    incoming = Serial.read();
    val = constrain(incoming, 48, 57); // ascii characters 0-9
    mosPower = map(val, 48, 57, 0, 255);
    Serial.print(incoming);
  }
  if (incoming == 'L') {
    // set delay
    delay(5);
    incoming = Serial.read();
    int val = constrain(incoming, 48, 57); // ascii characters 0-9
    lcdFreq = map(val, 48, 57, 2, 200);
    Serial.print('L');
    Serial.print(incoming);
  }
  if (incoming == 'D') {
    // set delay
    delay(5);
    incoming = Serial.read();
    int val = constrain(incoming, 48, 57); // ascii characters 0-9
    pulseDelay = map(val, 48, 57, 0, 200);
    // pass it on
    Serial.print('D');
    Serial.print(incoming);
  }
  if (incoming == 'B') {
    for (int i = 0; i < 10; i++) {
      digitalWrite(led[i], HIGH);

    }
    delay(200);
  }

  delay(10);
  digitalWrite(serialLedIn, LOW);
}


