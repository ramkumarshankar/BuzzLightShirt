const int flexPin = 0;
const int redPin = 9;
const int greenPin = 10;
const int bluePin = 11;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  int flexReading = analogRead(flexPin);
  Serial.println(flexReading);
  delay(500);
  
  int mappedValue = map(flexReading, 780, 920, 0, 100);
  Serial.print("mapped: ");
  Serial.println(mappedValue);  
  
  //Green if no flex
  if (mappedValue <= 50) {
    digitalWrite(redPin, LOW);
    digitalWrite(greenPin, HIGH);
    digitalWrite(bluePin, LOW);
  }
  
  //Red if there is flex
  if (mappedValue > 50) {
    digitalWrite(redPin, HIGH);
    digitalWrite(greenPin, LOW);
    digitalWrite(bluePin, LOW);
  }

}
