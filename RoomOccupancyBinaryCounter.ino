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

  if (isProximityWithinRange()) {
    
    Serial.println("-- PROXIMITY DISTRUBED --");
    digitalWrite(proximityLedPin, HIGH);

    long inTimeout = millis();

    while (true) {
      if (isUltrasonicWithinRange()) {
        setLedCount(--ledCount);
        digitalWrite(ultrasonicLedPin, HIGH);
  
        while(isUltrasonicWithinRange()) {
          Serial.println("waiting to leave...");  
        }

        digitalWrite(buzzerPin, HIGH);
        delay(100);
        digitalWrite(buzzerPin, LOW);
        delay(100);
        digitalWrite(buzzerPin, HIGH);
        delay(100);
        digitalWrite(buzzerPin, LOW);

        delay(100);
        digitalWrite(proximityLedPin, LOW);
        digitalWrite(ultrasonicLedPin, LOW);
               
        break;
      }
      else if (isProximityWithinRange()) {
        inTimeout = millis();
      }
      else if (!isProximityWithinRange()) {
        Serial.print(millis());
        Serial.print(" : ");
        Serial.println(inTimeout);

        if (millis() - inTimeout > 700) {
          Serial.println("Timeout...");
          digitalWrite(proximityLedPin, LOW);
          break;
        }
      }
      delay(50);
    }
    delay(100);
  }

  else if (isUltrasonicWithinRange()) {
    
    Serial.println("-- ULTRASONIC DISTRUBED --");
    digitalWrite(ultrasonicLedPin, HIGH);

    long inTimeout = millis();

    while (true) {
      if (isProximityWithinRange()) {
        setLedCount(++ledCount);
        digitalWrite(proximityLedPin, HIGH);
  
        while(isProximityWithinRange()) {
          Serial.println("waiting to leave...");  
        }

        digitalWrite(buzzerPin, HIGH);
        delay(300);
        digitalWrite(buzzerPin, LOW);

        delay(100);
        digitalWrite(proximityLedPin, LOW);
        digitalWrite(ultrasonicLedPin, LOW);
               
        break;
      }
      else if (isUltrasonicWithinRange()) {
        inTimeout = millis();
      }
      else if (!isUltrasonicWithinRange()) {
        Serial.print(millis());
        Serial.print(" : ");
        Serial.println(inTimeout);

        if (millis() - inTimeout > 700) {
          Serial.println("Timeout...");
          digitalWrite(ultrasonicLedPin, LOW);
          break;
        }
      }
      delay(50);

    }

    delay(100);
  }
  
  delay(100);
}

bool isProximityWithinRange() {
  return !digitalRead(proximityPin);
}

bool isUltrasonicWithinRange() {
  const short near = 50;
  const short far = 400;
  const int distance = getUltrasonicDistance();
  return (distance <= far && distance >= near);
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