//Accelerometer pins
const int xPin = A0;
const int yPin = A1;

//Change
float threshhold=65.0;

float xval[100]={0};
float yval[100]={0};
float zval[100]={0};
float xavg;
float yavg;
float zavg;
  
int flag=0;

void setup() {
  // put your setup code here, to run once:
  
  //Calibrate Accelerometer
  calibrate();
  
  Serial.begin(9600);
}

void loop() {

    readAccelerometer();

}

//Taken and modified from
//http://www.instructables.com/id/Simple-Easy-and-Cheap-DIY-Pedometer-with-Arduino/?ALLSTEPS

void calibrate() {
  digitalWrite(13,HIGH);
  
  float sum=0;
  float sum1=0;
  float sum2=0;
  for (int i=0;i<100;i++) {
    xval[i]=float(analogRead(xPin));
    sum=xval[i]+sum;
  }
  xavg=sum/100.0;
  Serial.println(xavg);
  delay(100);

  for (int j=0;j<100;j++) {
    yval[j]=float(analogRead(yPin));
    sum1=yval[j]+sum1;
  }
  yavg=sum1/100.0;
  Serial.println(yavg);
  delay(100);

  digitalWrite(13,LOW);
}

//Taken and modified from
//http://www.instructables.com/id/Simple-Easy-and-Cheap-DIY-Pedometer-with-Arduino/?ALLSTEPS

void readAccelerometer () {
  int acc=0;
  float totvect[100]={0};
  float totave[100]={0};
  float xaccl[100]={0};
  float yaccl[100]={0};
  
  for (int i=0;i<100;i++) {
    xaccl[i]=float(analogRead(xPin));
    delay(1);


    yaccl[i]=float(analogRead(yPin));
    delay(1);

    totvect[i] = sqrt(((xaccl[i]-xavg)* (xaccl[i]-xavg))+ ((yaccl[i] - yavg)*(yaccl[i] - yavg)));
    totave[i] = (totvect[i] + totvect[i-1]) / 2 ;
 
    Serial.println(totave[i]);
    delay(200);

    if (totave[i]>threshhold && flag==0) {
      //Light up LED
      Serial.println("Turn the LED on");
      digitalWrite(13, HIGH);
      delay(1000);
      digitalWrite(13, LOW);
      flag = 1;
    }
    else if (totave[i] > threshhold && flag==1) {
      //do nothing 
    }
    if (totave[i]<threshhold  && flag==1) {
      flag=0;
    }

}

 delay(1000);

}
