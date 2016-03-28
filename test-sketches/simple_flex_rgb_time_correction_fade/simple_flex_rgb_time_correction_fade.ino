const int flexPin = 0;
const int redPin = 9;
const int greenPin = 10;
const int bluePin = 11;

int redIntensity = 0;
int greenIntensity = 0;

unsigned long badPostureTime;
unsigned long correctionTime;

boolean counting = false;
boolean correction = true;

//Alert if flexed for 5 seconds or more
const unsigned long requiredDuration = 5000;

void setup() {
  // put your setup code here, to run once:
  digitalWrite(bluePin, LOW);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  int flexReading = analogRead(flexPin);
  Serial.println(flexReading);
  delay(500);
  
  int mappedValue = map(flexReading, 780, 920, 0, 100);  
  
  //Green if no flex
  if (mappedValue <= 50) {
    if (!correction) {
      correctionTime = millis();
      correction = true;
    }
    else {
      long duration = millis() - correctionTime;
      if (duration >= requiredDuration) {
        counting = false;
        badPostureTime = 0;
        fadeToGreen();
        delay(100);
      }
    }
  }
  
  //Red if there is flex
  if (mappedValue > 50) {
    if (!counting) {
      badPostureTime = millis();
      counting = true;
    }
    else {
      long duration = millis() - badPostureTime;
      if (duration >= requiredDuration) {
        correction = false;
        correctionTime = 0;
        fadeToRed();
        delay(100);
      }
    }
  }

}

void fadeToRed () {
  if (redIntensity < 255) {
    redIntensity++;
  }
  if (greenIntensity > 0) {
    greenIntensity--;
  }
  analogWrite(redPin, redIntensity);
  analogWrite(greenPin, greenIntensity);  
}

void fadeToGreen () {
  if (greenIntensity < 255) {
    greenIntensity++;
  }
  if (redIntensity > 0) {
    redIntensity--;
  }
  analogWrite(redPin, redIntensity);
  analogWrite(greenPin, greenIntensity);
}
