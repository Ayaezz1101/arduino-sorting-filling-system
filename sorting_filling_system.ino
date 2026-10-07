#include <Servo.h>
const int trigPin = 9;
const int echoPin = 10;
const int pumpPin = 7;
#define TRIG_PIN1 11
#define ECHO_PIN1 12
#define SERVO_PIN 6

Servo myServo;

float distance;
float targetDistance = 10.0;
float error;
float previousError = 0;
float integral = 0;
float derivative;
float output;

const float Kp = 3.0;
const float Ki = 0.1;
const float Kd = 0.2;

//const float integralMax = 50; 
//const float integralMin = -50; 

long duration;

unsigned long currentTime;
unsigned long lastTime = 0;
float elapsedTime;

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(pumpPin, OUTPUT);
  pinMode(TRIG_PIN1, OUTPUT);
  pinMode(ECHO_PIN1, INPUT);
  myServo.attach(SERVO_PIN);
  Serial.begin(9600);
  myServo.write(50);
}

void loop() {
  currentTime = millis();
  elapsedTime = (currentTime - lastTime) / 1000.0;  
  
 
  float distance1 = measureDistance(TRIG_PIN1, ECHO_PIN1);
 // Serial.print("Distance1: ");
  //Serial.println(distance1);
  Serial.print("Distance: ");
  Serial.println(distance);


  if (distance1 > 0 && distance1 <= 10) {
    myServo.write(90);


    distance = measureDistance(trigPin, echoPin);

  
    error = targetDistance - distance;
    
   
    integral += error * elapsedTime;
    
   
   // if (integral > integralMax) integral = integralMax;
    //if (integral < integralMin) integral = integralMin;
    
   
    derivative = (error - previousError) / elapsedTime;
    
    
    output = Kp * error + Ki * integral + Kd * derivative;
    
    const int minPumpSpeed = 220;   
    const int maxPumpSpeed = 255; 
    
   
    if (output < minPumpSpeed) {
      output = minPumpSpeed; 
    } else if (output > maxPumpSpeed) {
      output = maxPumpSpeed;
    }

   
    if (distance <= targetDistance) {
      output = 0;
    }
    
  
    int pumpSpeed = map(output, -255, 255, 0, 255);
    
    
    analogWrite(pumpPin, pumpSpeed);

   
    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.print(" cm, Pump Speed: ");
    Serial.println(pumpSpeed);

    previousError = error;
    lastTime = currentTime;
  } else {
    myServo.write(180); 
  }
  
 
  delay(100);
}


float measureDistance(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  long duration = pulseIn(echoPin, HIGH);
  return duration * 0.034 / 2;
}
