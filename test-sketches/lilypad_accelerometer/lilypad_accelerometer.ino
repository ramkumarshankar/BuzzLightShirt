int xPin = 0;
int yPin = 1;
int zPin = 2;
int option = -1;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

}

void loop() {
  // put your main code here, to run repeatedly:
  float xValue = analogRead(xPin);
  float yValue = analogRead(yPin);
  float zValue = analogRead(zPin);
  float zero_G = 500;
  float scale = 102.3;
  
//  while (Serial.available() > 0) {
//    option = Serial.parseInt();
//  }
//    
//  if (option == 0) {
//    Serial.print("x: ");
////    Serial.println(xValue);
//    Serial.println(xValue/scale);
//  }
//  if (option == 1) {
//    Serial.print("y: ");
////    Serial.println(yValue);
//    Serial.println(yValue/scale);
//  }
//  if (option == 2) {
//    Serial.print("z: ");
//    Serial.println(zValue);
////    Serial.println(zValue/scale);
//  }

  //Serial.print(((float)xValue - zero_G)/scale);
  Serial.print(xValue);
  Serial.print("\t");
  //Serial.print(((float)yValue - zero_G)/scale);
  Serial.print(yValue);
  Serial.print("\t");
  //Serial.print(((float)zValue - zero_G)/scale);
  Serial.print(zValue);
  Serial.print("\n");
  delay(500);
}

