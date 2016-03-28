//RGB LED pins
//Only red and green are used
const int redPin = 9;
const int greenPin = 10;

void setup() {
  // put your setup code here, to run once:
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);

}

void loop() {
  // put your main code here, to run repeatedly:
  
//  for (int i = 0; i < 255; i +=5) {
//    analogWrite(redPin, i);
//
//    delay(100);
//  }
//  
//  for (int i = 255; i >= 0; i -=5) {
//    analogWrite(redPin, i);
//
//    delay(100);
//  }

  analogWrite(redPin, 255);
  analogWrite(greenPin, 0);
  delay(1000);
  
  analogWrite(redPin, 0);
  analogWrite(greenPin, 0);
  delay(1000);
  
  analogWrite(greenPin, 255);
  analogWrite(redPin, 0);
  delay(1000);
  
  analogWrite(greenPin, 0);
  analogWrite(redPin, 0);
  delay(1000);
  
  
}
