//Flex sensor input
const int flexPin = A3;

float flexAvg;

//RGB LED pins
//Only red and green are used
const int redPin = 9;
const int greenPin = 10;

//Vibration motor pin
const int vibePin = A2;

//Push button switch pin
const int switchPin = 7;
int switchValue;
int buttonState = HIGH;
long debounceDelay = 50;

//Accelerometer pins and variables
const int xPin = A0;
const int yPin = A1;

//Change
float threshold=70.0;

float xval[100]={0};
float yval[100]={0};
float xavg;
float yavg;

int flag=0;

//RGB LED
int redIntensity = 0;
int greenIntensity = 0;

//Activity tracking variables
int breathingExercisesComplete = 0;
int badPostureCount = 0;
const int allowedBadPostureMaximum = 10;
const int requiredBreathingExercises = 40;

unsigned long badPostureTime;
unsigned long correctionTime;

//Breathing exercise interval
//Usually every hour (3600000), but set to a shorter value for testing
unsigned long breathingInterval = 60000;
unsigned long nextBreathingTime  = 0;

boolean counting = false;
boolean correction = false;

//Alert if flexed for a preset amount of time
//At least greater than 5 minutes (300000), but set to a shorter value for testing
const unsigned long requiredDuration = 30000;

void setup() {
  // put your setup code here, to run once:

  Serial.begin(9600);

  //Setup our input and output pins
  pinMode(vibePin, OUTPUT);
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);

  pinMode(xPin, INPUT);
  pinMode(yPin, INPUT);
  pinMode(switchPin, INPUT);

  //Set initial unpressed value to HIGH
  digitalWrite(switchPin, HIGH);

  //Turn off RGB LED
  resetLight();


  //Small vibration to indicate circuit is on
  pinMode(vibePin, OUTPUT);
  analogWrite(vibePin, 200);
  delay(500);
  analogWrite(vibePin, 0);
  delay(500);

  //Calibrate Flex Sensor
  calibrateFlexSensor();

  //Calibrate Accelerometer
  calibrateAccelerometer();

  //Initialise variables for breathing exercise
  nextBreathingTime += breathingInterval;

}

void loop() {
  // put your main code here, to run repeatedly

  readAccelerometer();
  delay(50);
  checkPosture();
  delay(50);

  readSwitchPin();
  if (switchValue == LOW) {
    //initiate a deep breathing exercise manually
    alertBreathing();
    for (int i = 0; i < 2; i++) {
      breathingExercise();
    }
    nextBreathingTime = millis() + breathingInterval;
  }

  delay(50);
  readAccelerometer();
  delay(50);
  checkBreathing();
  delay(50);
  readAccelerometer();
  delay(50);
}

//This function checks the wearer's posture
//and alerts them if correction is needed

void checkPosture() {
  int flexReading = analogRead(flexPin);

  //Compare reading with calibrated value on startup
  int difference = abs(flexReading-flexAvg);

  //Posture is healthy
  if (difference <= 10) {
    if (counting) {
      if (!correction) {
        correctionTime = millis();
        correction = true;
      }
      else {
        long duration = millis() - correctionTime;
        if (duration >= requiredDuration) {
          counting = false;
          badPostureTime = 0;
          Serial.println("Posture corrected!");
          delay(100);
          vibeCorrectionComplete();
        }
      }
    }
  }

  //Posture is unhealthy
  if (difference > 10) {
    if (!counting) {
      badPostureTime = millis();
      counting = true;
    }
    else {
      long duration = millis() - badPostureTime;
      if (duration >= requiredDuration) {
        correction = false;
        correctionTime = 0;
        Serial.println("Unhealthy posture detected!");
        delay(100);
        vibeCorrectionNeeded();
        if (duration >= 2*requiredDuration) {
          badPostureCount++;
          badPostureTime = millis();
          Serial.print("Bad Posture Count: ");
          Serial.println(badPostureCount);
          delay(100);
        }
      }
    }
  }
}

//Vibration pattern for deep breathing
//Called when breathing exercise interval is reached
void breathingExercise() {
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

  //Keep track of number of completed exercises
  breathingExercisesComplete++;
  Serial.print("Breathing Exercises Done: ");
  Serial.println(breathingExercisesComplete);
  delay(100);
}

//Vibration pattern to alert user that it's time for a breathing exercise
//Wearer can cancel the exercise by pressing the push button at this point
void alertBreathing() {
  analogWrite(vibePin, 200);
  delay(500);

  analogWrite(vibePin, 0);
  delay(500);

  analogWrite(vibePin, 200);
  delay(500);

  analogWrite(vibePin, 0);
  delay(500);

  analogWrite(vibePin, 200);
  delay(500);

  analogWrite(vibePin, 0);
  delay(500);
}

//Check if it's time for a breathing exercise
void checkBreathing() {
  long runTime = millis();
  if (runTime >= nextBreathingTime) {
    //Time for a breathing exercise
    alertBreathing();
    //Do it 5 times
    for (int i = 0; i < 2; i++) {
      readSwitchPin();
      if (switchValue == LOW) {
        break;
      }
      breathingExercise();
    }
    nextBreathingTime = millis() + breathingInterval;
  }
}

//Calculate the intensity and colour of the RGB LED based on tracked activity
void statusLight() {
  int badPosturesAllowed = allowedBadPostureMaximum - badPostureCount;

  greenIntensity = 255 * (float)breathingExercisesComplete/(float)requiredBreathingExercises * (float)badPosturesAllowed/(float)allowedBadPostureMaximum;
  redIntensity = 255 - greenIntensity;

  outputLED(redIntensity, greenIntensity);
  delay(2000);

  outputLED(0, 0);
  delay(2000);

}

//Utility function to turn ligth off
void resetLight() {
  outputLED(0, 0);
}

//Function to calibrate the accelerometer
//Taken and modified from
//http://www.instructables.com/id/Simple-Easy-and-Cheap-DIY-Pedometer-with-Arduino/?ALLSTEPS

void calibrateAccelerometer() {
  digitalWrite(13,HIGH);

  float sum=0;
  float sum1=0;
  float sum2=0;
  for (int i=0;i<100;i++) {
    xval[i]=float(analogRead(xPin));
    sum=xval[i]+sum;
  }
  xavg=sum/100.0;
  Serial.print("x average: ");
  Serial.println(xavg);
  delay(100);

  for (int j=0;j<100;j++) {
    yval[j]=float(analogRead(yPin));
    sum1=yval[j]+sum1;
  }
  yavg=sum1/100.0;
  Serial.print("y average: ");
  Serial.println(yavg);
  delay(100);

  digitalWrite(13,LOW);
}

//Function to calibrate flex sensor on startup
//I wrote this based on the accelerometer calibration method above

void calibrateFlexSensor() {
  float flexSum = 0;
  int flexInit[10] = {0};

  for (int i=0; i < 10; i++) {
    flexInit[i] = analogRead(flexPin);
    flexSum = float(flexInit[i])+ flexSum;
    delay(10);
  }
  flexAvg = flexSum/10;
  Serial.print("Flex Sensor Average: ");
  Serial.println(flexAvg);
  delay(100);
}

//Read the accelerometer. Turn on status light if there is a significang enough orientation change
//Taken and modified from
//http://www.instructables.com/id/Simple-Easy-and-Cheap-DIY-Pedometer-with-Arduino/?ALLSTEPS

void readAccelerometer () {
  int acc=0;
  float totvect[100]={0};
  float totave[100]={0};
  float xaccl[100]={0};
  float yaccl[100]={0};
  //flag = 1;

  for (int i=0;i<100;i++) {
    xaccl[i]=float(analogRead(xPin));
    delay(1);


    yaccl[i]=float(analogRead(yPin));
    delay(1);

    totvect[i] = sqrt(((xaccl[i]-xavg)* (xaccl[i]-xavg))+ ((yaccl[i] - yavg)*(yaccl[i] - yavg)));
    if (i != 0)
    {
      totave[i] = (totvect[i] + totvect[i-1]) / 2 ;
    }
    else {
      totave[i] = 0;
    }

    //Serial.println(totave[i]);
    //delay(200);

    if (totave[i]>threshold && flag==0) {
      //Serial.println(totave[i]);
      //Light up LED
      delay(100);
      statusLight();
      flag = 1;
      break;
    }
    else if (totave[i] > threshold && flag==1) {
      //do nothing
    }
    if (totave[i]<threshold  && flag==1) {
      flag=0;
    }

  }

}

//Vibration pattern to alert user of unhealthy posture
void vibeCorrectionNeeded() {
  analogWrite(vibePin, 200);
  delay(1000);
  analogWrite(vibePin, 0);
  delay(1000);
}

//Vibration pattern to alert user that posture has been corrected
void vibeCorrectionComplete() {
  analogWrite(vibePin, 200);
  delay(500);
  analogWrite(vibePin, 0);
  delay(500);
  analogWrite(vibePin, 200);
  delay(500);
  analogWrite(vibePin, 0);
  delay(500);
}

//Color generating function
//Adapted from http://lilypadarduino.org/?page_id=702
//I removed the parameter for the blue pin, since I don't need it

void outputLED (unsigned char red, unsigned char green)	//the color generating function
{
  analogWrite(redPin, 255-red);
  analogWrite(greenPin, 255-green);
}

//Function to read push button switch
//Uses a debounce delay to avoid detecting multiple presses
//Code adapted from IDEA9101 lab exercises

void readSwitchPin() {

  buttonState = HIGH;

  switchValue = digitalRead(switchPin);
  if (switchValue != buttonState) {
    delay(debounceDelay);
    switchValue = digitalRead(switchPin);
    if (switchValue != buttonState) {
      buttonState = switchValue;
    }
  }

}
