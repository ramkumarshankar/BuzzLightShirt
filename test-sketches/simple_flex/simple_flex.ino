const int flexpin = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  int flexReading = analogRead(flexpin);
  Serial.println(flexReading);
  delay(500);

}
