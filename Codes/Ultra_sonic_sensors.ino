//sample for ultra-sonic sensors control handler

const int pingPin = 31; // Trigger Pin of Ultrasonic Sensor
const int echoPin = 29; // Echo Pin of Ultrasonic Sensor

int LED = 23; // conect Led to arduino pin 23
int count = 0;
void setup() {
   Serial.begin(9600); // Starting Serial Terminal
   pinMode (LED, OUTPUT); // Led pin OUTPUT
}

void loop() {
   long duration, inches, cm, m;
   pinMode(pingPin, OUTPUT);
   
   digitalWrite(pingPin, LOW);
   delayMicroseconds(2);
   digitalWrite(pingPin, HIGH);
   delayMicroseconds(10);
   digitalWrite(pingPin, LOW);
   pinMode(echoPin, INPUT);
   
   duration = pulseIn(echoPin, HIGH);
   inches = microsecondsToInches(duration);
   cm = microsecondsToCentimeters(duration);
   m = cm / 100;

   if((cm > 500) && count >= 3){
      digitalWrite(LED, HIGH); // LED High
      if(cm < 500){
        count =0;
      }
    }
   else
      digitalWrite(LED, LOW); // LED High
      
   Serial.print(inches);
   Serial.print("in, ");
   Serial.print(cm);
   Serial.print("cm");
   Serial.print(m);
   Serial.print("m");
   Serial.println();
   Serial.print("Counter: ");
   Serial.print(count);
   Serial.println();
   count++;
   delay(1000);
}

long microsecondsToInches(long microseconds) {
   return microseconds / 74 / 2;
}

long microsecondsToCentimeters(long microseconds) {
   return microseconds / 29 / 2;
}
