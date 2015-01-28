#include <SoftwareSerial.h>
#include <Servo.h> 
SoftwareSerial mySerial(7, 8);
// Switch to control on/off .
const int switchPin = 3;
// Direction.
int d1, d2;
// Speed.
int s1, s2;
// Quadrant (not used).
int qA, qB;

int x = 0;
int y = 0;
int oldX = 0;
int oldY = 0;

// Pin for gending/recieving ping from proximity sensor.
const int pingPin = 6;
// Establish variables for duration of the ping,
// and the distance result in inches and centimeters.
long duration, cm;
// Servo for ping mount.
Servo myservo; 
// Servo postion.  
int cpos = 90;
int prevpos = 0;
void setup()
{
  pinMode(switchPin, INPUT); 
  
  Serial.begin(9600);
  
  mySerial.begin(19200);
  // Attaches the servo on pin 9 to the servo object.
  myservo.attach(9);
  // Turn to center.
  myservo.write(cpos);
  delay(200);
  myservo.detach();
}
/////////////////////////////////////////////////////////////////////
///// Function List ///////
/////////////////////////////////////////////////////////////////////
//   rc motor control
// readAndGo  -- Gets RC motor data request trough serial
//            -- Reads back the RC data in to variables
//            -- Sends stored RC motor variables to serial motor control
//  ultrasonic sensor 
// ping();
// microsecondsToCentimeters(long microseconds);
// avoidCollision(); -- reacts to sensor data input and moves motor depending on servo location. 
//  servo 
// moveDatServo(); -- swings the servo left and right
// goBackCheck(); -- When backing up, look both ways first and choose a way to turn
//  Generic DC motor cmds
// go(); -- tells bot to go forward
// turnLeft(); -- tells bot to spin left
// turnRight(); -- tells bot to spin right
// goBack(); -- tells bot to go backward
// goBackLeft(); -- backing Left
// goBackRight(); -- backing right
// noGOGO(); -- sets bot to stop
// rcGOGO(); -- Accepts go commands from other places
//   old stuff
// whoDatIs(); -- old servo scan
// noGO(); -- old nogo
// backUp(); -- tells bot to go backward
/////////////////////////////////////////////////////////////////////
//// Main Loop
/////////////////////////////////////////////////////////////////////
void loop() {
  int switchState = digitalRead(switchPin);
  if (switchState == HIGH) {
    
    xyGoGo();
    
    // myservo.attach(9);
    // moveDatServo();
    // avoidCollision();
    // readAndGo();
    // go();
    // myservo.detach();
  } else {
    noGOGO();
  }
}


void xyGoGo() {
  int incomingByte = 0;
  
  // send data only when you receive data:
  while (Serial.available()) {
    // read the incoming byte:
    incomingByte = Serial.read();
    Serial.println(incomingByte);
    
    if(incomingByte == 119) {
      
      // up
      y = y + 5;
    } else if(incomingByte == 97) {
      
      // left
      x = x - 5;
    } else if(incomingByte == 115) {
      
      // down
      y = y - 5;
    } else if(incomingByte == 100) {
      
      //right
      x = x + 5;
    }
    
    x = getRestrained(x);
    y = getRestrained(y);
  }
  
  if (y != oldY || x != oldX) {
    
    Serial.println("================================");
    
    Serial.print("x: ");
    Serial.print(x, DEC);
    Serial.print(", ");
    Serial.print("y: ");
    Serial.println(y, DEC);
    
    if (y > 0) {
      
      // forward
      d1 = 0xC5;
      d2 = 0xCE;
    } else {
      
      // back
      d2 = 0xC6;
      d1 = 0xCD;
    }
    
    if (x > -32 && x < 32) {
      
      // center lane
      s1 = abs(y);
      s2 = abs(y);
    } else {
      
      double ratio =  1 / (abs(x) * 0.03149);
      
      if (x < 0) {
        
        s1 = (abs(y) * ratio);
        s2 = (abs(y));
      } else {
        
        s1 = (abs(y));
        s2 = (abs(y) * ratio);
      }
    }
    
    Serial.println(s1);
    Serial.println(s2);
    Serial.println("================================");

    s1 = getRestrained((int) s1);
    s2 = getRestrained((int) s2);
    
    rcGOGO(d1, s1, d2, s2);
  }
  
  oldX = x;
  oldY = y;
}


int getRestrained(int value) {
  
    if(value < -125) {
      return -125;
    }
    
    if(x > 125) {
      return 125;
    }
    
    return value;
}

/////////////////////////////////////////////////////////////////////
////// RC motor Controls ///////
/////////////////////////////////////////////////////////////////////
void readAndGo () {
  // Data from RC reciever.
  byte rcData[2];
  // Get remapped channel input values.
  mySerial.write(0x87);
  // Ask for first 2 channels (Motor 1 and 2).
  mySerial.write(0x03);
  delayMicroseconds(250);
  for (int j = 0; j < 2; j++ ) {
    int x = mySerial.read();
    rcData[j] = x;
  }
  if (rcData[0] > 127) {
    d1 = 0xC5;    //motor 1 backward acc
    //d1 = 0xC1;  //motor 1 backward set
    s1 = rcData[0] - 127;
  } else {
    d1 = 0xC6;    //motor 1 forward acc function
    //d1 = 0xC2;  //motor 1 forward 'set' function
    s1 = rcData[0];
  }
  
  if (rcData[1] > 127) {
    d2 = 0xCE;    //M2 forward acc function
    //d2 = 0xCA;  //M2 forward 'set' function
    s2 = rcData[1] -127;
  } else {
    d2 = 0xCD;    // M2 backward acc
    //d2 = 0xC9;  // M2 backward set
    s2 = rcData[1];
  }
  rcGOGO(d1, s1, d2, s2);
}
void rcGOGO(int d1, int s1, int d2, int s2) {
  Serial.print("Direction 1: ");
  Serial.println(d1);
  Serial.print("Speed 1: ");
  Serial.println(s1);
  Serial.print("Direction 2: ");
  Serial.println(d2);
  Serial.print("Speed 2: ");
  Serial.println(s2);
  
  //motor 1
  mySerial.write(d1);
  mySerial.write(s1);
  //motor2
  mySerial.write(d2);
  mySerial.write(s2);
}
/////////////////////////////////////////////////////////////////////
////// Ultra Sonic Sensor cmds ///////
/////////////////////////////////////////////////////////////////////
long microsecondsToCentimeters(long microseconds) {
  
  return microseconds / 29 / 2;
}
void ping() {
  pinMode(pingPin, OUTPUT);
  digitalWrite(pingPin, LOW);
  delayMicroseconds(2);
  digitalWrite(pingPin, HIGH);
  delayMicroseconds(5);
  digitalWrite(pingPin, LOW);
  pinMode(pingPin, INPUT);
  duration = pulseIn(pingPin, HIGH);
  cm = microsecondsToCentimeters(duration);
}
/////////////////////////////////////////////////////////////////////
///// servo comands /////////
/////////////////////////////////////////////////////////////////////
void moveDatServo() {
  
  if (cpos == 180) {
    // Left most edge.
    prevpos = 181;
    cpos = 150;
    myservo.write(cpos);  
  } else if (cpos == 0) {
    // Right most edge.
    prevpos = -1;
    cpos = 30;
    myservo.write(cpos);
  } else if (prevpos < cpos && cpos < 180) {
    // We're moving counter clockwise toward 140.
    prevpos = cpos;
    cpos = cpos + 30;
    myservo.write(cpos);
  } else if (prevpos > cpos && cpos > 0) {
    // We're moving clockwise toward 20.
    prevpos = cpos;
    cpos = cpos - 30;  
    myservo.write(cpos);
  }
  
  delay(90);
}
void avoidCollision() {
  if (cpos == 90) {
    
    ping();
    if (cm < 60) {
       //Hazard ahead!
       goBackCheck();
       delay(600);
      //goRoundThatBitch();
    }  
  } else if (cpos == 120) {  
    
    ping();
    if (cm < 70) {
      // Hazard to left!
      turnRight();
      delay(140);
    }
  } else if (cpos == 60) {
    ping();
    if (cm < 70) {
      // Hazard to right!
      turnLeft();
      delay(140);
    }
  } else if (cpos == 180) {  
    ping();
    if (cm < 55) {
      // Hazard to far left!
      turnRight();
      delay(200);
    }
  } else if (cpos == 0) {
    ping();
    if (cm < 55) {
      // Hazard to far right!
      turnLeft();
      delay(200);
    }
  }
}
/////////////////////////////////////////////////////////////////////
///////// Generic Move Commands //////////
/////////////////////////////////////////////////////////////////////
void go(){
  
d1= 0xC5;
s1= 45;
d2= 0xCE;
s2= 45;
rcGOGO(d1, s1, d2, s2);
}
/////////////////////////////////////////////////////////////////////
void turnRight(){
    if (s1 < 63) {
     s1= s1 * 2;
    } else {
     s1= 127;
    } 
    
    s2= s2/4;
 
d1= 0xC5;
s1= s1;
d2= 0xCE;
s2= s2;
//old style (delay time also changed, used to be half)     
//d1= 0xC5;
//s1= 45;
//d2= 0xCD;
//s2= 45;
rcGOGO(d1, s1, d2, s2);
}
/////////////////////////////////////////////////////////////////////
void turnLeft(){
    s1= s1/4;
    
    if (s2 < 63) {
     s2= s2 * 2;
    } else {
     s2= 127;
    }     
d1= 0xC5;
s1= s1;
d2= 0xCE;
s2= s2;
//old style (delay time also changed, used to be half)      
//d1= 0xC6;
//s1= 45;
//d2= 0xCE;
//s2= 45;
rcGOGO(d1, s1, d2, s2);
}
/////////////////////////////////////////////////////////////////////
void goBack(){
  
d1= 0xC6;
s1= 45;
d2= 0xCD;
s2= 45;
rcGOGO(d1, s1, d2, s2);
}
/////////////////////////////////////////////////////////////////////
void goBackLeft(){
      
d1= 0xC6;
s1= 60;
d2= 0xCE;
s2= 60;
  
//d1= 0xC6;
//s1= 70;
//d2= 0xCD;
//s2= 30;
rcGOGO(d1, s1, d2, s2);
}
/////////////////////////////////////////////////////////////////////
void goBackRight(){
    
d1= 0xC5;
s1= 60;
d2= 0xCD;
s2= 60;
  
//d1= 0xC6;
//s1= 30;
//d2= 0xCD;
//s2= 70;
rcGOGO(d1, s1, d2, s2);
}
/////////////////////////////////////////////////////////////////////
void noGOGO(){
  
d1= 0xC6;
s1= 0;
d2= 0xCE;
s2= 0;
rcGOGO(d1, s1, d2, s2);
}
/////////////////////////////////////////////////////////////////////
////////Generic Servo Moves //////////
/////////////////////////////////////////////////////////////////////
void goToZero(){
   myservo.write(0);              // tell servo to go to position in variable 'pos' 
      delay(400); 
}
void goToOneEighty(){
   myservo.write(180);              // tell servo to go to position in variable 'pos' 
      delay(400); 
}
/////////////////////////////////////////////////////////////////////
void goBackCheck(){
  
noGOGO();
int cmL = 0;
int cmR = 0;
myservo.write(160);
delay(250);
ping();
cmR = cm;
myservo.write(20);
delay(250);
ping();
cmL = cm;
  if ( cmR > cmL){
      goBackLeft();      
  }else{
      goBackRight();  
  }
}
/////////////////////////////////////////////////////////////////////
void goRoundThatBitch() { 
  turnLeft();
  delay(950);
  noGOGO();
  myservo.attach(9);
  myservo.write(0);
  delay(200);
  myservo.detach();
  ping();
  while(cm < 60){
    ping();
    go();
    delay(100);
  }
  go();
  delay(1000);
  turnRight();
  delay(950);
  noGOGO();
  myservo.attach(9);
  myservo.write(90);
  delay(200); 
  myservo.detach();   
}
//////// Old Stuff ///////
/////////////////////////////////////////////////////////////////////
void whoDatIs()
{
int pos = 0;
Serial.print("cm = ");
Serial.print(cm);
Serial.print('\n');  
delay(1000);
ping();
myservo.attach(9);
Serial.print("Distance at 90 = ");
Serial.print(cm);
Serial.print('\n');
      if (pos >= 20){
        delay(10);
        myservo.write(135);
        delay(400);
        pos = 135;
    }
        
     if (pos >= 135){
        ping();       
        qA = cm;
        Serial.print("QA = ");
        Serial.print(qA);
        Serial.print('\n');       
        myservo.write(20); 
        delay(400);       
        ping();
        //delay(10);        
        qB = cm;
        Serial.print("QB = ");        
        Serial.print(qB);
        Serial.print('\n');
        pos = 20;
        //delay(200);
        myservo.write(90);
        pos = 90;
        delay(200);        
    }  
 Serial.print('\n');   
 myservo.detach();
}
void noGo(){
mySerial.write(0xC4);
mySerial.write(0xCC);
}
void backUp(){
mySerial.write(0xC6);
mySerial.write(0x20);
mySerial.write(0xCD);
mySerial.write(0x20);
}
/////////////////////////////////////////////////////////////////////

