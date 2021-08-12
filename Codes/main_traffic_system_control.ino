#include <TimerOne.h>
#include<Wire.h>
#include <LiquidCrystal_I2C.h>

#define R1 23
#define Y1 25
#define G1 27

#define R2 46
#define Y2 48
#define G2 50

#define R3 13
#define Y3 12
#define G3 11

#define R4 10
#define Y4 9
#define G4 8

LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display

int signal1[] = {23, 25, 27};
int signal2[] = {46, 48, 50};
int signal3[] = {13, 12, 11};
int signal4[] = {10, 9, 8};
int redDelay = 5000;
int yellowDelay = 2000;
int signalONdelay;
                              //ultra-sonic sensor control variables
volatile int triggerpin1 = 31;    
volatile int echopin1 = 29;       
volatile int triggerpin2 = 44;     
volatile int echopin2 = 42;        
volatile int triggerpin3 = 7;    
volatile int echopin3 = 6;       
volatile int triggerpin4 = 5;    
volatile int echopin4 = 4;       
volatile long time;             //Variable for lane operation flow time
volatile int S1, S2, S3, S4;    //Variables for storing the distance covered
int t = 60;                     //distance(in cm) under which the sensor will detect vehicles.
int IR1counter = 0;             //counts vehicles from IR sensor 1
int IR2counter = 0;             //counts vehicles from IR sensor 2
int IR3counter = 0;             //counts vehicles from IR sensor 3
int IR4counter = 0;             //counts vehicles from IR sensor 4
int count = 0;                  //counts elapsed time for counter timer
long duration, inches, cm, m;   //timer controls

    //-------IR sensor pins dfn---------
int IRSensor1 = 22;
int IRSensor2 = 24;
int IRSensor3 = 26;
int IRSensor4 = 28;

int IR1,IR2,IR3,IR4;

int carCounter1;
String laneValue;
long interval;

void setup(){
  Serial.begin(9600);
  pinMode(R1,OUTPUT);
  pinMode(Y1,OUTPUT);
  pinMode(G1,OUTPUT);
  
  pinMode(R2,OUTPUT);
  pinMode(Y2,OUTPUT);
  pinMode(G2,OUTPUT);
  
  pinMode(R3,OUTPUT);
  pinMode(Y3,OUTPUT);
  pinMode(G3,OUTPUT);
  
  pinMode(R4,OUTPUT);
  pinMode(Y4,OUTPUT);
  pinMode(G4,OUTPUT);
                                // Sets the trigPin as an OUTPUT
                                // Sets the echoPin as an INPUT
  pinMode(triggerpin1, OUTPUT); 
  pinMode(echopin1, INPUT); 
  pinMode(triggerpin2, OUTPUT); 
  pinMode(echopin2, INPUT); 
  pinMode(triggerpin3, OUTPUT);
  pinMode(echopin3, INPUT);
  pinMode(triggerpin4, OUTPUT);
  pinMode(echopin4, INPUT); 
  
  lcd.begin(16,2);
  lcd.init();     // initialize the lcd 
  lcd.backlight();

}
void readSensor(){
  IR1=digitalRead(22);
  IR2=digitalRead(24);
  IR3=digitalRead(26);
  IR4=digitalRead(28);

  Serial.print(IR1);
  Serial.print("\t");
  Serial.print(IR2);
  Serial.print("\t");
  Serial.print(IR3);
  Serial.print("\t");
  Serial.print(IR4);
  Serial.print("\t");

}

void arc(){
  digitalWrite(R1,LOW);
  digitalWrite(R2,LOW);
  digitalWrite(R3,LOW);
  digitalWrite(R4,LOW);
  digitalWrite(Y1,HIGH);
  digitalWrite(Y2,HIGH);
  digitalWrite(Y3,HIGH);
  digitalWrite(Y4,HIGH);

  delay(800);
  
  digitalWrite(Y1,LOW);
  digitalWrite(Y2,LOW);
  digitalWrite(Y3,LOW);
  digitalWrite(Y4,LOW);

  delay(500);
  readSensor();
 }
void loop(){
  
  readSensor();   //calls IR read fn
  //softInterr();   //calls interrupt control fn
  
  if(IR1==0 && IR2==0 && IR3==0 && IR4==0){
    arc();
  }
  else if(IR1==1 && IR2==0 && IR3==0 && IR4==0){
    signal1Function();
  }
  else if(IR1==0 && IR2==1 && IR3==0 && IR4==0){
    signal2Function();
  }
   else if(IR1==0 && IR2==0 && IR3==1 && IR4==0){
    signal3Function();
  }
   else if(IR1==0 && IR2==0 && IR3==0 && IR4==1){
    signal4Function();
  }
   else if(IR1==1 && IR2==1 && IR3==0 && IR4==0){
    signal1Function();
    signal2Function();
  }
   else if(IR1==1 && IR2==0 && IR3==1 && IR4==0){
    signal1Function();
    signal3Function();
  }
   else if(IR1==1 && IR2==0 && IR3==0 && IR4==1){
    signal1Function();
    signal4Function();
  }
   else if(IR1==1 && IR2==1 && IR3==1 && IR4==0){
    signal1Function();
    signal2Function();
    signal3Function();
  }
   else if(IR1==1 && IR2==1 && IR3==0 && IR4==1){
    signal1Function();
    signal2Function();
    signal4Function();
  }
   else if(IR1==1 && IR2==0 && IR3==1 && IR4==1){
    signal1Function();
    signal3Function();
    signal4Function();
  }
   else if(IR1==0 && IR2==1 && IR3==1 && IR4==0){
    signal2Function();
    signal3Function();
  }
   else if(IR1==0 && IR2==1 && IR3==0 && IR4==1){
    signal2Function();
    signal3Function();
  }
   else if(IR1==0 && IR2==1 && IR3==1 && IR4==1){
    signal2Function();
    signal3Function();
    signal4Function();
  }
   else if(IR1==0 && IR2==0 && IR3==1 && IR4==1){
    signal3Function();
    signal4Function();
  }
   else if(IR1==1 && IR2==1 && IR3==1 && IR4==1){
    signal1Function();
    signal2Function();
    signal3Function();
    signal4Function();
  }
 }
void unitDisplay(int countValue){
  lcd.clear();
  lcd.setCursor(0,1);
  lcd.print("Traffic System");
  lcd.setCursor(1,0);
  lcd.print(laneValue);
  lcd.print(":V-");
  lcd.print(countValue); //number of vehicles
  lcd.print(":");
  lcd.print("T-");
  lcd.print(signalONdelay / 1000); //time allocated(s)
  lcd.print("(s)");
 }
// This is interrupt function and it will run each time the timer period finishes. The timer period is set at 100 milli seconds.
void softInterr(){
  // Reading from first ultrasonic sensor
  digitalWrite(triggerpin1, LOW);  
  delayMicroseconds(2);
  digitalWrite(triggerpin1, HIGH); 
  delayMicroseconds(10);
  digitalWrite(triggerpin1, LOW);
  time = pulseIn(echopin1, HIGH); 
  S1= time*0.034/2;
  // Reading from second ultrasonic sensor
  digitalWrite(triggerpin2, LOW);  
  delayMicroseconds(2);
  digitalWrite(triggerpin2, HIGH); 
  delayMicroseconds(10);
  digitalWrite(triggerpin2, LOW);
  time = pulseIn(echopin2, HIGH); 
  S2= time*0.034/2;
  // Reading from third ultrasonic sensor
  digitalWrite(triggerpin3, LOW);  
  delayMicroseconds(2);
  digitalWrite(triggerpin3, HIGH); 
  delayMicroseconds(10);
  digitalWrite(triggerpin3, LOW);
  time = pulseIn(echopin3, HIGH); 
  S3= time*0.034/2;
  // Reading from fourth ultrasonic sensor
  digitalWrite(triggerpin4, LOW);  
  delayMicroseconds(2);
  digitalWrite(triggerpin4, HIGH); 
  delayMicroseconds(10);
  digitalWrite(triggerpin4, LOW);
  time = pulseIn(echopin4, HIGH); 
  S4= time*0.034/2;
  // Print distance values on serial monitor for debugging

}
void signal1Function(){
  softInterr();   //calls interrupt control fn
  Serial.println("1");
  low();
  
  lane1Counter();           //calibrates lane 1 counter
  delayHandler1();           //calls delay function
 
  laneValue = "L1";
      
                            // Make RED LED LOW and make Green HIGH for 5 seconds
  digitalWrite(signal1[0], LOW);
  digitalWrite(signal1[2], HIGH);
  unitDisplay(IR1counter);             //calls lcd display
  if(S2<t || S3<t || S4<t)
  {
                             // Make Green LED LOW and make yellow LED HIGH for 2 seconds
    if(signalONdelay != 0){
      digitalWrite(signal1[2], LOW);
      digitalWrite(signal1[1], HIGH);
      delay(yellowDelay);
      }
  }
}
void signal2Function(){
  softInterr();   //calls interrupt control fn
  Serial.println("2");
  low();
  
  lane2Counter(); //calibrates lane 2 counter
  delayHandler2(); //calls delay function
  
  laneValue = "L2";
  
  digitalWrite(signal2[0], LOW);
  digitalWrite(signal2[2], HIGH);
  unitDisplay(IR2counter);
      //calls lcd display
  
  if(S1<t || S3<t || S4<t)
  {
                             // Make Green LED LOW and make yellow LED HIGH for 2 seconds
    if(signalONdelay != 0){
      digitalWrite(signal1[2], LOW);
      digitalWrite(signal1[1], HIGH);
      delay(yellowDelay);
      } 
  }
}
void signal3Function(){
  softInterr();   //calls interrupt control fn
  Serial.println("3");
  low();
  
  lane3Counter(); //calibrates lane 3 counter
  delayHandler3(); //calls delay function
  
  laneValue = "L3";
  
  digitalWrite(signal3[0], LOW);
  digitalWrite(signal3[2], HIGH);
  unitDisplay(IR3counter);
      //calls lcd display
  
  if(S1<t || S2<t || S4<t)
  {
                             // Make Green LED LOW and make yellow LED HIGH for 2 seconds
    if(signalONdelay != 0){
      digitalWrite(signal1[2], LOW);
      digitalWrite(signal1[1], HIGH);
      delay(yellowDelay);
      } 
  }  
}
void signal4Function(){
  softInterr();   //calls interrupt control fn
  Serial.println("4");
  low();
  
  lane4Counter(); //calibrates lane 1 counter
  delayHandler4(); //calls delay function
  
  laneValue = "L4";
  
  digitalWrite(signal4[0], LOW);
  digitalWrite(signal4[2], HIGH);
  unitDisplay(IR4counter);
      //calls lcd display
  
  if(S1<t || S2<t || S3<t)
  {
                             // Make Green LED LOW and make yellow LED HIGH for 2 seconds
    if(signalONdelay != 0){
      digitalWrite(signal1[2], LOW);
      digitalWrite(signal1[1], HIGH);
      delay(yellowDelay);
      } 
  }
}
// Function to make all LED's LOW except RED one's.
void low(){
  for(int i=1; i<3; i++)
  {
    digitalWrite(signal1[i], LOW);
    digitalWrite(signal2[i], LOW);
    digitalWrite(signal3[i], LOW);
    digitalWrite(signal4[i], LOW);
  }
  for(int i=0; i<1; i++)
  {
    digitalWrite(signal1[i], HIGH);
    digitalWrite(signal2[i], HIGH);
    digitalWrite(signal3[i], HIGH);
    digitalWrite(signal4[i], HIGH);
  }
}
void lane1Counter(){
    count++;
    digitalWrite(triggerpin1, LOW);  
    delayMicroseconds(2);
    digitalWrite(triggerpin1, HIGH); 
    delayMicroseconds(10);
    digitalWrite(triggerpin1, LOW);
    duration = pulseIn(echopin1, HIGH);
    inches = microsecondsToInches(duration);
    cm = microsecondsToCentimeters(duration);
    m = cm / 100;
    
    Serial.print("L1 Distance: ");
    Serial.println(cm);
    if(cm < t){
      count = 0;
      IR1counter++;
    }//clears count
    else 
      IR1counter = 0;
}
void lane2Counter(){
    count++;
    digitalWrite(triggerpin2, LOW);  
    delayMicroseconds(2);
    digitalWrite(triggerpin2, HIGH); 
    delayMicroseconds(10);
    digitalWrite(triggerpin2, LOW);
    duration = pulseIn(echopin2, HIGH);
    inches = microsecondsToInches(duration);
    cm = microsecondsToCentimeters(duration);
    m = cm / 100;
  
    Serial.print("L2 Distance: ");
    Serial.println(cm);
    if(cm < t){
      count = 0;
      IR2counter++;
    }//clears count
    else 
      IR2counter = 0;
    //delay(1000);
}
void lane3Counter(){
    count++;
    digitalWrite(triggerpin3, LOW);  
    delayMicroseconds(2);
    digitalWrite(triggerpin3, HIGH); 
    delayMicroseconds(10);
    digitalWrite(triggerpin3, LOW);
    duration = pulseIn(echopin3, HIGH);
    inches = microsecondsToInches(duration);
    cm = microsecondsToCentimeters(duration);
    m = cm / 100;
  
    Serial.print("L3 Distance: ");
    Serial.println(cm);
    if(cm < t){
      count = 0;
      IR3counter++;
    }//clears count
    else 
      IR3counter = 0;
    //delay(1000);
}
void lane4Counter(){
    count++;
    digitalWrite(triggerpin4, LOW);  
    delayMicroseconds(2);
    digitalWrite(triggerpin4, HIGH); 
    delayMicroseconds(10);
    digitalWrite(triggerpin4, LOW);
    duration = pulseIn(echopin4, HIGH);
    inches = microsecondsToInches(duration);
    cm = microsecondsToCentimeters(duration);
    m = cm / 100;
  
    Serial.print("L4 Distance: ");
    Serial.println(cm);
    if(cm < t){
      count = 0;
      IR4counter++;
    }//clears count
    else  
      IR4counter = 0;
    //delay(1000);
}
void delayHandler1(){//delay calibration function
  if(IR1counter == 0){//skip lane
    signalONdelay = 0;
  }
  if((IR1counter > 0) && (IR1counter <= 5)){//low density(4s)
    signalONdelay = 4000;
  }
  else if((IR1counter > 5) && (IR1counter <= 9)){//medium density(8s)
    signalONdelay = 8000;
  }
  if(IR1counter > 9){//high density(12s)
    signalONdelay = 12000;
  }
}
void delayHandler2(){//delay calibration function
  if(IR2counter == 0){//skip lane
    signalONdelay = 0;
  }
  if((IR2counter > 0) && (IR2counter <= 5)){//low density(4s)
    signalONdelay = 4000;
  }
  else if((IR2counter > 5) && (IR2counter <= 9)){//medium density(8s)
    signalONdelay = 8000;
  }
  if(IR2counter > 9){//high density(12s)
    signalONdelay = 12000;
  }
}
void delayHandler3(){//delay calibration function
  if(IR3counter == 0){//skip lane
    signalONdelay = 0;
  }
  if((IR3counter > 0) && (IR3counter <= 5)){//low density(4s)
    signalONdelay = 4000;
  }
  else if((IR3counter > 5) && (IR3counter <= 9)){//medium density(8s)
    signalONdelay = 8000;
  }
  if(IR3counter > 9){//high density(12s)
    signalONdelay = 12000;
  }
}
void delayHandler4(){//delay calibration function
  if(IR4counter == 0){//skip lane
    signalONdelay = 0;
  }
  if((IR4counter > 0) && (IR4counter <= 5)){//low density(4s)
    signalONdelay = 4000;
  }
  else if((IR4counter > 5) && (IR4counter <= 9)){//medium density(8s)
    signalONdelay = 8000;
  }
  if(IR4counter > 9){//high density(12s)
    signalONdelay = 12000;
  }
}
long microsecondsToInches(long microseconds) {
   return microseconds / 74 / 2;
}
long microsecondsToCentimeters(long microseconds) {
   return microseconds / 29 / 2;
}
