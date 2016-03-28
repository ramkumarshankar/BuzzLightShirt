int vibePin = A2;
int amplitude = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(vibePin, OUTPUT);
  analogWrite(vibePin, 0);
}

void loop() {
  // put your main code here, to run repeatedly:
  for (int i = 78; i <=255; i++) {
    analogWrite(vibePin, i);
    delay(20);
  }

  analogWrite(vibePin, 255);
  delay(100);
  
  analogWrite(vibePin, 0);
  delay(2000);
  
  for (int i = 255; i >= 78; i--) {
    analogWrite(vibePin, i);
    delay(20);
  }
  
  analogWrite(vibePin, 0);
  delay(1000);
}
