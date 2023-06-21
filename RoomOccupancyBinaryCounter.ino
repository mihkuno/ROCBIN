const byte buzzerPin        = 11;
const byte ultrasonicLedPin = 10;
const byte sonicEchoPin     = 9;
const byte sonicTrigPin     = 8;
const byte regInputPin      = 7;
const byte regLatchPin      = 6;
const byte regClockPin      = 5;
const byte proximityPin     = 4;
const byte proximityLedPin  = 3;

byte ledCount = 0;

void setup() {
  pinMode(buzzerPin, OUTPUT);
  pinMode(regInputPin, OUTPUT);
  pinMode(regLatchPin, OUTPUT);
  pinMode(regClockPin, OUTPUT);
  pinMode(sonicTrigPin, OUTPUT);
  pinMode(sonicEchoPin, INPUT);
  pinMode(proximityLedPin, OUTPUT);
  pinMode(ultrasonicLedPin, OUTPUT);
  pinMode(proximityPin, INPUT);

  setLedCount(ledCount);
  Serial.begin(9600);
}

void loop() {

  bool enteringFlag = isUltrasonicWithinRange();
  bool exitingFlag = isProximityWithinRange();


   while (enteringFlag) {
    digitalWrite(ultrasonicLedPin, HIGH);

    delay(200);
    bool nextFlag = false;
    if (isProximityWithinRange() && nextFlag == false) {  
      if (nextFlag == false) {
        setLedCount(++ledCount);
        nextFlag = true;
      }
    }

    if (nextFlag == true) {
      digitalWrite(proximityLedPin, HIGH);
      digitalWrite(ultrasonicLedPin, LOW);
      

      while (isProximityWithinRange()) {
        delay(200);
      }

      digitalWrite(buzzerPin, HIGH);
      delay(120);
      digitalWrite(buzzerPin, LOW);
      delay(120);
    
      digitalWrite(proximityLedPin, LOW);
      enteringFlag = false;
      delay(80);
    }
  }
  

  while (exitingFlag) {
    digitalWrite(proximityLedPin, HIGH);

    delay(200);
    bool nextFlag = false;
    if (isUltrasonicWithinRange() && nextFlag == false) {  
      if (nextFlag == false) {
        setLedCount(--ledCount);
        nextFlag = true;
      }
    }

    if (nextFlag == true) {
      digitalWrite(ultrasonicLedPin, HIGH);
      digitalWrite(proximityLedPin, LOW);

      while (isUltrasonicWithinRange()) {
        delay(200);
      }

      digitalWrite(buzzerPin, HIGH);
      delay(80);
      digitalWrite(buzzerPin, LOW);
      delay(80);
      digitalWrite(buzzerPin, HIGH);
      delay(80);
      digitalWrite(buzzerPin, LOW);
    
      digitalWrite(ultrasonicLedPin, LOW);
      exitingFlag = false;
      delay(80);
    }
  }
  



  delay(150);
}

bool isProximityWithinRange() {
  return !digitalRead(proximityPin);
}

bool isUltrasonicWithinRange() {
  short distance = getUltrasonicDistance();
  bool inrange = distance >= 0 && distance <= 500;
  Serial.print(distance);
  Serial.print(" : ");
  Serial.println(inrange);

  return inrange;
}

short getUltrasonicDistance() {
  digitalWrite(sonicTrigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(sonicTrigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(sonicTrigPin, LOW);

  return pulseIn(sonicEchoPin, HIGH);
}

void setLedCount(byte value) {
  digitalWrite(regLatchPin, LOW);
  shiftOut(regInputPin, regClockPin, MSBFIRST, value);
  digitalWrite(regLatchPin, HIGH);
}