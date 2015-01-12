// sumo bot edition 1
//import library
#include <PID_v1.h>
#include <Arduino.h>
#include <Wire.h>
#include <HMC5883L_Simple.h>

//create a compass
HMC5883L_Simple Compass;

//sonar array
int sonar[7];
//sonar reading variable:
int state = 1;
int id,reading;
byte temp[24];  //24 bytes
boolean read_ready = false ;
int read_byte = 0;
//bin0, bin1 and bin2 for finding the responced sonar
volatile boolean ITR0 = false;
//motor pin:
const int ENA = 13;
const int Left_A = 12;
const int Left_B = 10;
const int Left_pwm = 11;
const int ENB = 8;
const int Right_A = 7;
const int Right_B = 6;
const int Right_pwm = 5;
int pwm = 250;

//interrupt pin
const int B0 = 1;
const int B1 = 3;
const int B2 = 4;
//sonar index
const int front = 0;
const int front_left = 1;
const int front_right = 2;
const int left = 3;
const int right = 4;
const int back_left = 5;
const int back_right = 6;
//PID Variable
double Desired_A,Measured_A,A_Error;
double Kp = 1, Ki = 0.05, Kd = 0;
PID A_PID(&Desired_A,&A_Error,&Measured_A, Kp,Ki , Kd,DIRECT);
 
//setup
void setup(){
  //serial communication
  Serial1.begin(9601);
  //sonar interrupt configuration
  interrupt_config();
  //compass configuration
  compass_config();
  //motor configuration
  motor_config();
  //set PID
  A_PID.SetMode(AUTOMATIC);
}
// main 
void loop(){
  
  //sonar read
}

void interrupt_config()
{
  //in mega, pin 18 for interrupt 5, pin 19 for interrupt 4, pin 20 for interrupt 3, pin 21 for interrupt 2
  attachInterrupt(2,interrupt_0,RISING);
}

//interrupt 0
void interrupt_0(){
  ITR0 = true;
}

//clear interrupt reading
void clear()
{
  ITR0 = false;
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

//move forward, for direction, 0 means turn left, 1 means right
void Forward(int pwm,double ratio,int direct){
  digitalWrite(ENA,HIGH);
  digitalWrite(ENB,HIGH);
  // A and B cannot be HIGH at the SAME TIME !!!
  digitalWrite(Left_A,HIGH);
  digitalWrite(Left_B,LOW);
  digitalWrite(Right_A,HIGH);
  digitalWrite(Right_B,LOW);
  //switch mood
  switch (direct)
  {
    case (0):
    {
      analogWrite(Left_pwm,ratio *pwm);
      analogWrite(Right_pwm, pwm);
    }
    case (1):
    {
      analogWrite(Left_pwm,pwm);
      analogWrite(Right_pwm, ratio * pwm);
    }
  }
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

//robot turning
void turn(int sonar_index)
{
  digitalWrite(ENA,HIGH);
  digitalWrite(ENB,HIGH);
  switch (sonar_index)
  {
    case (front_left):
    {
      //assigning speed, needs to be cailberated
      pwm = 150;
      if (heading() > 45)
      {
        Desired_A = heading() - 45.0;
      }else
      {
        Desired_A = heading() + 315.0;
      }
      Measured_A = heading();
      A_PID.Compute();
      //break when angle is correct
      while (( A_Error >10.0) && (A_Error <=45.0))
      {
        //PID adjustment
        Measured_A = heading();
        A_PID.Compute();
        if (heading() < Desired_A)
        {
          digitalWrite(Left_A,HIGH);
          digitalWrite(Left_B,LOW);
          digitalWrite(Right_A,LOW);
          digitalWrite(Right_B,HIGH);
          //pwm value needs to be cailberated
          pwm = pwm* A_Error/45;
          analogWrite(Left_pwm, pwm );
          analogWrite(Right_pwm, pwm);
        }else{
          digitalWrite(Left_A,LOW);
          digitalWrite(Left_B,HIGH);
          digitalWrite(Right_A,HIGH);
          digitalWrite(Right_B,LOW);
          //pwm value needs to be cailberated
          pwm = pwm* A_Error/45;
          analogWrite(Left_pwm, pwm );
          analogWrite(Right_pwm, pwm);
        }
      }
      break;
    }
    case (front_right):
    {
      //assigning speed ,needs to be cailerbrated
      pwm = 150;
      if (heading() >315.0)
      {
        Desired_A = heading() - 315.0;
      }else
      {
        Desired_A = heading() + 45.0 ;
      }
      Measured_A = heading();
      A_PID.Compute();
      //break when angle is correct
      while (( A_Error >10.0) && (A_Error <=45.0))
      {
        //PID adjustment
        Measured_A = heading();
        A_PID.Compute();
        if (heading() < Desired_A)
        {
          digitalWrite(Left_A,HIGH);
          digitalWrite(Left_B,LOW);
          digitalWrite(Right_A,LOW);
          digitalWrite(Right_B,HIGH);
          //pwm value needs to be cailberated
          pwm = pwm* A_Error/45;
          analogWrite(Left_pwm, pwm );
          analogWrite(Right_pwm, pwm);
        }else{
          digitalWrite(Left_A,LOW);
          digitalWrite(Left_B,HIGH);
          digitalWrite(Right_A,HIGH);
          digitalWrite(Right_B,LOW);
          //pwm value needs to be cailberated
          pwm = pwm* A_Error/45;
          analogWrite(Left_pwm, pwm );
          analogWrite(Right_pwm, pwm);
        }
      }
      break;
    }
    case (left):
    {
      //assigning speed ,needs to be cailerbrated
      pwm = 180;
      if (heading() >90.0)
      {
        Desired_A = heading() - 90.0;
      }else
      {
        Desired_A = heading() + 270.0;
      }
      Measured_A = heading();
      A_PID.Compute();
      //break when angle is correct
      while (( A_Error >10.0) && (A_Error <=90.0))
      {
        //PID adjustment
        Measured_A = heading();
        A_PID.Compute();
        if (heading() < Desired_A)
        {
          digitalWrite(Left_A,HIGH);
          digitalWrite(Left_B,LOW);
          digitalWrite(Right_A,LOW);
          digitalWrite(Right_B,HIGH);
          //pwm value needs to be cailberated
          pwm = pwm* A_Error/90;
          analogWrite(Left_pwm, pwm );
          analogWrite(Right_pwm, pwm);
        }else{
          digitalWrite(Left_A,LOW);
          digitalWrite(Left_B,HIGH);
          digitalWrite(Right_A,HIGH);
          digitalWrite(Right_B,LOW);
          //pwm value needs to be cailberated
          pwm = pwm* A_Error/90;
          analogWrite(Left_pwm, pwm );
          analogWrite(Right_pwm, pwm);
        }
      }
      break;
    }
    case (right):
    {
            //assigning speed ,needs to be cailerbrated
      pwm = 180;
      if (heading() > 270.0)
      {
        Desired_A = heading() - 270.0;
      }else
      {
        Desired_A = heading() + 90.0;
      }
      Measured_A = heading();
      A_PID.Compute();
      //break when angle is correct
      while (( A_Error >10.0) && (A_Error <=90.0))
      {
        //PID adjustment
        Measured_A = heading();
        A_PID.Compute();
        if (heading() < Desired_A)
        {
          digitalWrite(Left_A,HIGH);
          digitalWrite(Left_B,LOW);
          digitalWrite(Right_A,LOW);
          digitalWrite(Right_B,HIGH);
          //pwm value needs to be cailberated
          pwm = pwm* A_Error/90;
          analogWrite(Left_pwm, pwm );
          analogWrite(Right_pwm, pwm);
        }else{
          digitalWrite(Left_A,LOW);
          digitalWrite(Left_B,HIGH);
          digitalWrite(Right_A,HIGH);
          digitalWrite(Right_B,LOW);
          //pwm value needs to be cailberated
          pwm = pwm* A_Error/90;
          analogWrite(Left_pwm, pwm );
          analogWrite(Right_pwm, pwm);
        }
      }
      break;
    }
    case (back_left):
    {
      //assigning speed ,needs to be cailerbrated
      pwm = 220;
      if (heading() > 135.0)
      {
        Desired_A = heading() - 135.0;
      }else
      {
        Desired_A = heading() + 225.0;
      }
      Measured_A = heading();
      A_PID.Compute();
      //break when angle is correct
      while (( A_Error >10.0) && (A_Error <=135.0))
      {
        //PID adjustment
        Measured_A = heading();
        A_PID.Compute();
        if (heading() < Desired_A)
        {
          digitalWrite(Left_A,HIGH);
          digitalWrite(Left_B,LOW);
          digitalWrite(Right_A,LOW);
          digitalWrite(Right_B,HIGH);
          //pwm value needs to be cailberated
          pwm = pwm* A_Error/135;
          analogWrite(Left_pwm, pwm );
          analogWrite(Right_pwm, pwm);
        }else{
          digitalWrite(Left_A,LOW);
          digitalWrite(Left_B,HIGH);
          digitalWrite(Right_A,HIGH);
          digitalWrite(Right_B,LOW);
          //pwm value needs to be cailberated
          pwm = pwm* A_Error/135;
          analogWrite(Left_pwm, pwm );
          analogWrite(Right_pwm, pwm);
        }
      }
      break;
    }
    case (back_right):
    {
      //assigning speed ,needs to be cailerbrated
      pwm = 220;
      if (heading() > 225.0)
      {
        Desired_A = heading() - 225.0;
      }else
      {
        Desired_A = heading() + 135.0;
      }
      Measured_A = heading();
      A_PID.Compute();
      //break when angle is correct
      while (( A_Error >10.0) && (A_Error <=135.0))
      {
        //PID adjustment
        Measured_A = heading();
        A_PID.Compute();
        if (heading() < Desired_A)
        {
          digitalWrite(Left_A,HIGH);
          digitalWrite(Left_B,LOW);
          digitalWrite(Right_A,LOW);
          digitalWrite(Right_B,HIGH);
          //pwm value needs to be cailberated
          pwm = pwm* A_Error/135;
          analogWrite(Left_pwm, pwm );
          analogWrite(Right_pwm, pwm);
        }else{
          digitalWrite(Left_A,LOW);
          digitalWrite(Left_B,HIGH);
          digitalWrite(Right_A,HIGH);
          digitalWrite(Right_B,LOW);
          //pwm value needs to be cailberated
          pwm = pwm* A_Error/135;
          analogWrite(Left_pwm, pwm );
          analogWrite(Right_pwm, pwm);
        }
      }
      break
    }
    default:
    {
      Stop();
      break;
    }
  } 
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
int refletance(int sensor)
{
  pinMode(sensor,OUTPUT);
  digitalWrite(sensor,HIGH);
  delayMicroseconds(11); //10us for rechargin its capacitor
  pinMode(sensor,INPUT);
  
  long time = micros();
  //time how long the input  is HIGH, but quite after 0.5ms as nothing happens after that
  while (digitalRead(sensor) == HIGH && micros() - time < 500);
  //return reading
  return (micros() - time);
}
//low pass filter
// filterVal determines smoothness. 1~ 0.0001, 0 is off
// smoothedVal holds the last loop value just use a unique variable, this is needed to be declared as GOLOBAL variable
int smooth(int data, double filterVal, double smoothedVal){


  if (filterVal > 1){      // check to make sure param's are within range
    filterVal = .99;
  }
  else if (filterVal <= 0){
    filterVal = 0;
  }

  smoothedVal = (data * (1 - filterVal)) + (smoothedVal  *  filterVal);

  return (int)smoothedVal;
}
 //Serial interrupt 
void serialEvent1()
{
  //changed from ==28 to >= 28
  if(Serial1.available()>=28)
  {
     for (int i=0; i < 28;i++)
     {
      //read 8 bit
      temp[i] = Serial1.read();
      }
     read_ready = true;
  }
}
//decoding reading from FPGA
void Decode()
{
  //-------------------------------
  //arrange data
  for (int j=0; j < read_byte;j++){
    switch(state){
        //start
       case (1):
       {
         if (temp[j] == 1)
         {
           state = 2;
        }
         break;
        }
        //id
        case (2):
        {
          if (temp[j] > 7){  
            state = 1;
          }else
          {
            id = temp[j];
            state = 3;
          }
          break;
        }
        //reading
        case (3):
        {
          if (temp[j] <2){
            state =1;
          }else
          {
             reading = temp[j];
             state = 4;
          }
           break;
        }
        //break
        case (4):
        {
          if (temp[j] == 255)
          {
            sonar[id] = reading;
          }
          state = 1;
          break;
        }
        //default
        {
          state = 1;
          break;
        }
      }
  }
}

//functions for path follower
//root approxi
double root_approxi(double num){
  int sifig = 0;
  double temp_num = num;
  double x;
  while (num > 0)
  {
    num = num/10;
    sifig++;
  }
  //rough approximation
  x = sifig*100;
  for (int i =1;i < 10; i++)
  {
    x = 0.5*(x + temp_num/x);
  }
  return x;
}
//return ratio
double out_curve(int front_left, int left){
  //find a, b
  double a = front_left + 11.243;
  double b = left + 7.95;
  //cosine law
  double c = root_approxi( a*a+b*b - 2*a*b*0.7071);
  //curve radius
  double r = c/(2 * ( a*0.7071/c - 1.5708));
  //return speed ratio
  double ratio = (b - 7.95 + r)/(r + b +7.95); 
  // left : right
  return ratio;
}

double in_curve(int front_left, int left){
  //find a,b
  double a = front_left + 11.243;
  double b = left + 7.95;
  ////curve radius
  double r = ( a*a - b*b)/(2*b - 1.4142*a);
  //return speed ratio
  double ratio = ( r -7.95)/(r + 7.95);
  //left : right
  return ratio;
}

