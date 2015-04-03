/*
  Robot 02
  Uses the IR obstacle Sensors to prevent the car hitting obstacles
  Uses PWM to set the speed
 
  This example code is in the public domain.
 */
 
//int L1 = 7, L2 = 6, R1 = 8, R2 = 9, LED = 13; // Pins connected to Motor driver H-Bridge
int L1 = 6, L2 = 7, R1 = 9, R2 = 8, LED = 13; // Pins connected to Motor driver H-Bridge
int lDetect = 12, rDetect = 13;             // Pins for obstacle sensors (remove rDetect connection when downloading sketch to Arduino)
int wait = 10;  // ms to wait between moves - set to short time exept for testing
int count = 0;
int flag = 0;
int reverseAmount = 50; // Default was originally 255

// Ultrasound Definitions
#include <Servo.h> 
 
int panPin = 10;     // Pan servo is on Digital Pin 8
int tiltPin = 11;    // Tilt servo is on Digital Pin 10
Servo panServo, tiltServo;  // create servo object to control a servo 

int trigPin = A4;   // Analog pin 0 used for the Ping Pin (Trig)
int echoPin = A5;   // Analog pin 1 used for the Echo Pin (Echo)
unsigned long duration, inches;
int indec, cmdec;
int inchconv = 147; // ratio between pulse width and inches
int cmconv = 59;    // ratio between pulse width and cm
String s1, s2, s3;
int panCentre = 110;

// the setup routine runs once when you press reset:
void setup()
{                
  // initialize the digital pins we will use as an output.
  pinMode(L1, OUTPUT);     
  pinMode(L2, OUTPUT);     
  pinMode(R1, OUTPUT);     
  pinMode(R2, OUTPUT);
  pinMode (lDetect, INPUT);
  pinMode (rDetect, INPUT);
//  panServo.attach(panPin);      // attaches the pan servo to the servo object 
//  tiltServo.attach(tiltPin);    // attaches the tilt servo to the servo object
//  panServo.write(panPosition);    // Initialise Pan servo
//  tiltServo.write(tiltPosition);  // initialise Tilt servo
  Serial.begin(115200);
  
  // initialize the ultrasonic sensor pins and centre the servos
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  panServo.attach(panPin, 1000, 2000);
  tiltServo.attach(tiltPin, 1000, 2000);
  pointCentre();
  tiltServo.write(90);
}

void loop()
{
  Serial.println("Starting...");
  forward(255);
  if (count>5){ // Trapped in a corner more than 5 times ?
    count=0;
    reverse(reverseAmount); // Escape from corner
    delay(3000);
    rSpin(255);
    delay(2000);
  }
  if (digitalRead(lDetect)==0) // Test left sensor
  {
    Serial.println(String("lDetect") + ":" + String(count));
    if(flag == 1)                  // Check previous state
      count++;
    else
      count=0;
    flag = 0;
    reverse(reverseAmount); // Normal operate
    delay(2000);
    rSpin(255);
    delay(1000);
  }
  if (digitalRead(rDetect)==0)         // Test right switch
  {
    Serial.println(String("rDetect") + ":" + String(count));
    if(flag == 0)                  // Check previous state
      count++;
    else
      count=0;
    flag = 1;
    reverse(reverseAmount); // Normal operate
    delay(2000);
    lSpin(200);
    delay(1000);
  }
}

// Ultrasound functions
void pointLeft()
{
  panServo.write(panCentre - 60);
  delay(150); // wait for servo to get there
}

void pointRight()
{
  panServo.write(panCentre + 60);
  delay(300); // wait for servo to get there
}

void pointCentre()
{
  panServo.write(panCentre);
  delay(150); // wait for servo to get there
}
int getDistance()
{
  int rval;
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH, 38000L);  // Set timeout to 38mS
  rval = microsecondsToCentimeters(duration);
  Serial.println(rval);
  return rval;
}

long microsecondsToCentimeters(long microseconds)
{
  return microseconds / cmconv;
}

// robMove sets the H-Bridge inputs appropriately for the direction and sets the PWM values
void robMove(int lSpeed, int rSpeed)
{
  String s2 = String(lSpeed) + ":" + String(rSpeed);
  Serial.println(s2);
  if(lSpeed == 0 && rSpeed == 0)
  {
    digitalWrite(L1, LOW);
    digitalWrite(L2, LOW);
    digitalWrite(R1, LOW);
    digitalWrite(R2, LOW);  
  }
  else
  {
    if(lSpeed < 0)
    {
      digitalWrite(L1, LOW);
      analogWrite(L2, abs(lSpeed));
    }
    else
    {
      digitalWrite(L1, HIGH);
      analogWrite(L2, 255 - lSpeed);
    }
    if(rSpeed < 0)
    {
      digitalWrite(R1, LOW);
      analogWrite(R2, abs(rSpeed));
    }
    else
    {
      digitalWrite(R1, HIGH);
      analogWrite(R2, 255 - rSpeed);
    }
  }
}

void forward(int rate)
{
  robMove(rate, rate);
  delay(wait);
}

void reverse(int rate)
{
  robMove(-rate, -rate);
  delay(wait);
}

void left(int rate)
{
  robMove(rate/2, rate);
  delay(wait);
}

void right(int rate)
{
  robMove(rate, rate/2);
  delay(wait);
}

void halt()
{
  robMove(0, 0);
  delay(wait);
}

void lSpin(int rate)
{
  robMove(-rate, rate);
  delay(wait);
}

void rSpin(int rate)
{
  robMove(rate, -rate);
  delay(wait);
}
