
    ///////////////////IR Sensor////////////////////////

const int pingPin = 31; // Trigger Pin of Ultrasonic Sensor
const int echoPin = 29; // Echo Pin of Ultrasonic Sensor

int IRSensor = 22; // connect ir sensor to arduino pin 2
int LED = 23; // conect Led to arduino pin 13

int counter = 0;
int count = 0;
long duration, inches, cm, m;


void setup() 
{
  Serial.begin(9600);
  pinMode (IRSensor, INPUT); // sensor pin INPUT
  pinMode (LED, OUTPUT); // Led pin OUTPUT
}

void loop()
{
   count++;
   duration = pulseIn(echoPin, HIGH);
   inches = microsecondsToInches(duration);
   cm = microsecondsToCentimeters(duration);
   m = cm / 100;

  int statusSensor = digitalRead(IRSensor);
  
   if((cm > 500) && count > 3){
      counter = 0;
    }//clears main counter
    if(cm < 500){
      count = 0;
      counter++;
    }//clears count
    delay(1000);
}
long microsecondsToInches(long microseconds) {
   return microseconds / 74 / 2;
}

long microsecondsToCentimeters(long microseconds) {
   return microseconds / 29 / 2;
}
