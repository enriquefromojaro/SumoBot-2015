// sumo bot edition 1
//import library
#include <Arduino.h>
#include <Wire.h>
#include <HMC5883L_Simple.h>

//create a compass
HMC5883L_Simple Compass;

//sonar array
int sonar[7];

//bin0, bin1 and bin2 for finding the responced sonar
volatile boolean ITR0 = false;
//motor pin:
const int ENA = 13;
const int Left_B = 12;
const int Left_A = 11;
const int Left_pwm = 10;
const int ENB = 8;
const int Right_A = 7;
const int Right_B = 6;
const int Right_pwm = 5;
int pwm = 250;

//reflectance
#define Front_L 2
#define Front_R 9
#define Back_L 3
#define Back_R 4

//interrupt pin
#define B0  24
#define B1  25
#define B2  26
int sonar_to_read = 0;
//sonar index
#define front  0
#define front_left  1
#define front_right  2
#define left  5
#define right  4
 
//direction define
#define LEFT 0
#define RIGHT 1
#define MID 2
//setup
void setup(){
  //compass configuration
  Serial.begin(9600);
  compass_config();
  //motor configuration
  motor_config();
  sonar_config();
  //No NEED for configuration of reflectance sensor !!!
  delay(4100);
  // A and B cannot be HIGH at the SAME TIME !!!

  
}
// main 
void loop(){

  Serial.println(reflectance(Front_L));
  Serial.println(reflectance(Back_R));
  Serial.println(reflectance(Back_L));
  Serial.println(reflectance(Front_R));
  sonar_to_read = digitalRead(B0) + digitalRead(B1)*2 + digitalRead(B2)*4 - 1;
  Serial.print(" sonar: ");
  Serial.println(sonar_to_read);
  /*digitalWrite(ENA,HIGH);
  digitalWrite(ENB,HIGH);
  // A and B cannot be HIGH at the SAME TIME !!!
  digitalWrite(Left_A,LOW);
  digitalWrite(Left_B,HIGH);
  digitalWrite(Right_A,HIGH);
  digitalWrite(Right_B,LOW);
  analogWrite(Left_pwm,180);
  analogWrite(Right_pwm,180);
  delay(150);
  Stop();
  delay(500);*/
  
  while ((reflectance(Front_L) > 2700)&& (reflectance(Front_R) > 2700)&&(reflectance(Back_L) > 2700) && (reflectance(Back_R) > 2700))
  {
    sonar_to_read = digitalRead(B0) + digitalRead(B1)*2 + digitalRead(B2)*4 - 1;
    Serial.println("Go");
    switch (sonar_to_read)
    {
      case (front):
      {
        Serial.println("Forward");
        trace();
        break;
      }
      case (front_left):
      {
        Serial.println("front left");
        digitalWrite(ENA,HIGH);
         digitalWrite(ENB,HIGH);
  // A and B cannot be HIGH at the SAME TIME !!!
        digitalWrite(Left_A,LOW);
        digitalWrite(Left_B,HIGH);
         digitalWrite(Right_A,HIGH);
        digitalWrite(Right_B,LOW);
        analogWrite(Left_pwm,180);
        analogWrite(Right_pwm,180);
        delay(150);
        break;
      }
      case (front_right):
      {
        Serial.println("front right");
        digitalWrite(ENA,HIGH);
         digitalWrite(ENB,HIGH);
  // A and B cannot be HIGH at the SAME TIME !!!
        digitalWrite(Left_A,HIGH);
        digitalWrite(Left_B,LOW);
         digitalWrite(Right_A,LOW);
        digitalWrite(Right_B,HIGH);
        analogWrite(Left_pwm,180);
        analogWrite(Right_pwm,180);
        delay(150);
        break;
      }
      case (left):
      {
        Serial.println("left");
        digitalWrite(ENA,HIGH);
         digitalWrite(ENB,HIGH);
  // A and B cannot be HIGH at the SAME TIME !!!
        digitalWrite(Left_A,LOW);
        digitalWrite(Left_B,HIGH);
         digitalWrite(Right_A,HIGH);
        digitalWrite(Right_B,LOW);
        analogWrite(Left_pwm,180);
        analogWrite(Right_pwm,180);
        delay(300);
        break;
      }
      case (right):
      {
        Serial.println("right");
        digitalWrite(ENA,HIGH);
         digitalWrite(ENB,HIGH);
  // A and B cannot be HIGH at the SAME TIME !!!
        digitalWrite(Left_A,HIGH);
        digitalWrite(Left_B,LOW);
         digitalWrite(Right_A,LOW);
        digitalWrite(Right_B,HIGH);
        analogWrite(Left_pwm,180);
        analogWrite(Right_pwm,180);
        delay(300);
        break;
      }
      default:
      {
        randomWalk();
        break;
      }
    }
  }
  Serial.println("dangeous");
  
  Stop();
  if (reflectance(Front_L) <=2700)
  {
    digitalWrite(ENA,HIGH);
    digitalWrite(ENB,HIGH);
    // A and B cannot be HIGH at the SAME TIME !!!
    digitalWrite(Left_B,LOW);
    digitalWrite(Left_A,HIGH);
    digitalWrite(Right_B,LOW);
    digitalWrite(Right_A,HIGH);
    analogWrite(Left_pwm,200);
    analogWrite(Right_pwm,200);
    delay(200);
    digitalWrite(Left_A,LOW);
    digitalWrite(Left_B,HIGH);
    digitalWrite(Right_A,HIGH);
    digitalWrite(Right_B,LOW);
    analogWrite(Left_pwm,200);
    analogWrite(Right_pwm,200);
    delay(650);
  }else if (reflectance(Front_R) <= 2700)
  {
    digitalWrite(ENA,HIGH);
    digitalWrite(ENB,HIGH);
    // A and B cannot be HIGH at the SAME TIME !!!
    digitalWrite(Left_B,LOW);
    digitalWrite(Left_A,HIGH);
    digitalWrite(Right_B,LOW);
    digitalWrite(Right_A,HIGH);
    analogWrite(Left_pwm,200);
    analogWrite(Right_pwm,200);
    delay(200);
    digitalWrite(Left_A,LOW);
    digitalWrite(Left_B,HIGH);
    digitalWrite(Right_A,HIGH);
    digitalWrite(Right_B,LOW);
    analogWrite(Left_pwm,200);
    analogWrite(Right_pwm,200);
    delay(650);
  }else if (reflectance(Back_L) <= 2700)
  {
    digitalWrite(ENA,HIGH);
    digitalWrite(ENB,HIGH);
    // A and B cannot be HIGH at the SAME TIME !!!
    digitalWrite(Left_A,LOW);
    digitalWrite(Left_B,HIGH);
    digitalWrite(Right_A,HIGH);
    digitalWrite(Right_B,LOW);
    analogWrite(Left_pwm,220);
    analogWrite(Right_pwm,220);
    delay(50);
  }else if (reflectance(Back_R) <=2700)
  {
    digitalWrite(ENA,HIGH);
    digitalWrite(ENB,HIGH);
    // A and B cannot be HIGH at the SAME TIME !!!
    digitalWrite(Left_A,HIGH);
    digitalWrite(Left_B,LOW);
    digitalWrite(Right_A,HIGH);
    digitalWrite(Right_B,LOW);
    analogWrite(Left_pwm,220);
    analogWrite(Right_pwm,220);
    delay(50);
  }
}


//sonar config
void sonar_config()
{
  pinMode(B0, INPUT);
  pinMode(B1, INPUT);
  pinMode(B2, INPUT);
}
//motor configuration 
void motor_config(){
  pinMode(ENA,OUTPUT);
  //input INPUTA, INPUTB control
  pinMode(Left_A,OUTPUT);
  pinMode(Left_B,OUTPUT);
  pinMode(Left_pwm,OUTPUT);
  pinMode(Right_A,OUTPUT);
  pinMode(Right_B,OUTPUT);
  pinMode(Right_pwm,OUTPUT);
  pinMode(ENB,OUTPUT);
}
/*
//move forward, for direction, 0 means turn left, 1 means right
void Forward(int pwm,double ratio,int direct){
  digitalWrite(ENA,HIGH);
  digitalWrite(ENB,HIGH);
  // A and B cannot be HIGH at the SAME TIME !!!
  digitalWrite(Left_A,HIGH);
  digitalWrite(Left_B,LOW);
  digitalWrite(Right_A,HIGH);
  digitalWrite(Right_B,LOW);
  int Speed = ratio*pwm;
  //switch mood
  switch (direct)
  {
    case (LEFT):
    {
      analogWrite(Left_pwm,Speed);
      analogWrite(Right_pwm, pwm);
    }
    case (RIGHT):
    {
      analogWrite(Left_pwm,pwm);
      analogWrite(Right_pwm, Speed);
    }
    case (MID):
    {
      analogWrite(Left_pwm,pwm);
      analogWrite(Right_pwm, pwm);
    }
  }
  delay(250);
}*/
//randome walk
void randomWalk(){
    digitalWrite(ENA,HIGH);
    digitalWrite(ENB,HIGH);
    // A and B cannot be HIGH at the SAME TIME !!!
    digitalWrite(Left_B,HIGH);
    digitalWrite(Left_A,LOW);
    digitalWrite(Right_B,HIGH);
    digitalWrite(Right_A,LOW);
    analogWrite(Left_pwm,70);
    analogWrite(Right_pwm,70);
    delay(30);
}

//stop
void Stop(){
  digitalWrite(ENA,LOW);
  digitalWrite(ENB,LOW);
  digitalWrite(Left_A,LOW);
  digitalWrite(Left_B,LOW);
  analogWrite(Left_pwm,0);
  digitalWrite(Right_A,LOW);
  digitalWrite(Right_B,LOW);
  analogWrite(Right_pwm,0);
}

//compass configuration
void compass_config()
{
  Wire.begin();
  Compass.SetDeclination(23, 35, 'E');
  Compass.SetSamplingMode(COMPASS_SINGLE);
  Compass.SetScale(COMPASS_SCALE_130);
  Compass.SetOrientation(COMPASS_HORIZONTAL_X_NORTH);
}

//get compass reading
double heading(){
  return Compass.GetHeadingDegrees();
}

//refletance sensor reading
int reflectance(int sensor)
{
  pinMode(sensor,OUTPUT);
  digitalWrite(sensor,HIGH);
  delayMicroseconds(100);                  //  Testing !!!  40 may not be the best fit
  pinMode(sensor,INPUT);
  
  long time = micros();

  //time how long the input is HIGH, but quit after 3ms as nothing happens after that
  while (digitalRead(sensor) == HIGH && micros() - time < 3000); 
  int diff = micros() - time;

  return diff;
}


void trace()
{
  boolean trace_success = true;
  while ((reflectance(Front_L) > 2700)&& (reflectance(Front_R) > 2700)&&(reflectance(Back_L) > 2700) && (reflectance(Back_R) > 2700)&&(trace_success))
  {
    sonar_to_read = digitalRead(B0) + digitalRead(B1)*2 + digitalRead(B2)*4 - 1;
    switch (sonar_to_read)
    {
         
      case (front):
      {
        Serial.println("Forward");
         digitalWrite(ENA,HIGH);
        digitalWrite(ENB,HIGH);
  // A and B cannot be HIGH at the SAME TIME !!!
        digitalWrite(Left_B,HIGH);
          digitalWrite(Left_A,LOW);
        digitalWrite(Right_B,HIGH);
        digitalWrite(Right_A,LOW);
        analogWrite(Left_pwm,250);
        analogWrite(Right_pwm,250);
        delay(50);
        break;
      }
      case (front_left):
      {
        Serial.println("trace, front left");
      digitalWrite(ENA,HIGH);
         digitalWrite(ENB,HIGH);
  // A and B cannot be HIGH at the SAME TIME !!!
        digitalWrite(Left_B,HIGH);
          digitalWrite(Left_A,LOW);
        digitalWrite(Right_B,HIGH);
        digitalWrite(Right_A,LOW);
        analogWrite(Left_pwm,130);
        analogWrite(Right_pwm,180);
        delay(150);
        break;
      }
      case (front_right):
      {
        Serial.println("trace, front right");
        digitalWrite(ENA,HIGH);
         digitalWrite(ENB,HIGH);
  // A and B cannot be HIGH at the SAME TIME !!!
         digitalWrite(Left_B,HIGH);
          digitalWrite(Left_A,LOW);
        digitalWrite(Right_B,HIGH);
        digitalWrite(Right_A,LOW);
        analogWrite(Left_pwm,180);
        analogWrite(Right_pwm,130);
        delay(150);
        break;
      }
      default:
      {
        trace_success = false;
        break;
      }
    }
  }
}

