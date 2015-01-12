#include <SharpIR.h>

//motor pin:
const int ENA = 13;
const int Left_A = 12;
const int Left_B = 11;
const int Left_pwm = 10;
const int ENB = 8;
const int Right_A = 7;
const int Right_B = 6;
const int Right_pwm = 5;
int max_pwm = 255;
//sonar pin:
const int trig_right = 25;
const int echo_right = 26;
//IR pin:
const int IR_back_right = 1;
//IR
SharpIR IR_BR(IR_back_right,15,95, 1080);
//sonar index
#define front  0
#define front_left  1
#define front_right  2
#define left  3
#define right  4
#define back_left  5
#define back_right  6

//direction define
#define LEFT 0
#define RIGHT 1
#define MID 2
double sonar[7];
//setup
void setup(){
  //serial communication
  Serial.begin(9600);
  //sonar interrupt configuration
  sonar_config();
  //motor configuration
  motor_config();
  //begin when right sonar got 10cm
  sonar[right] = reading(right);
  /*while ((reading(right) > 10.50) || (reading(right) < 9.50))
  {
    Stop();
  }*/
}
// main 
void loop(){
  
  //sonar read
    Serial.print("back right:  ");n  
  Serial.println(IRreading(IR_back_right));
  sonar[right] = reading(right);
  sonar[back_right] = IRreading(IR_back_right);
  if (sonar[back_right] > 14.0)
  {
    Backward(160, out_curve(sonar[back_right],sonar[right]),LEFT);
  }else if (sonar[back_right] <14.0 )
  {
    Backward(160,in_curve(sonar[back_right],sonar[right]),RIGHT);
  }
  else{
    Backward(170,1,MID);
  }

}

void sonar_config()
{
  pinMode(trig_right,OUTPUT);
  pinMode(echo_right,INPUT);
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
double out_curve(double BR, double R){
  //find a, b
  double a = BR + 11.243;
  double b = R + 7.95;
  //cosine law
  double c = root_approxi( a*a+b*b - 2*a*b*0.7071);
  //curve radius
  double r = c/(2 * ( a*0.7071/c - 1.5708));
  //return speed ratio
  double ratio = (b - 7.95 + r)/(r + b +7.95); 
  // left : right
  return ratio;
}

double in_curve(double BR, double R){
  //find a,b
  double a = BR + 11.243;
  double b = R + 7.95;
  ////curve radius
  double r = ( a*a - b*b)/(2*b - 1.4142*a);
  //return speed ratio
  double ratio = ( r -7.95)/(r + 7.95);
  //left : right
  return ratio;
}

double reading(int index)
{
  double duration;
  double dist;
  switch (index)
  {
    case (right):
    {
      digitalWrite(trig_right,HIGH);
      delayMicroseconds(10);
      digitalWrite(trig_right,LOW);
      duration=pulseIn(echo_right,HIGH);
      dist =  duration / 58;
      break;
    }
    default:
    {
      break;
    }
  }
  return dist;
}

void Backward(int pwm, double ratio, int direct)
{
  digitalWrite(ENA,HIGH);
  digitalWrite(ENB,HIGH);
  // A and B cannot be HIGH at the SAME TIME !!!
  digitalWrite(Left_A,LOW);
  digitalWrite(Left_B,HIGH);
  digitalWrite(Right_A,LOW);
  digitalWrite(Right_B,HIGH);
  //switch mood
  switch (direct)
  {
    case (0):
    {
      analogWrite(Left_pwm,ratio *pwm);
      analogWrite(Right_pwm, pwm);
      break;
    }
    case (1):
    {
      analogWrite(Left_pwm,pwm);
      analogWrite(Right_pwm, ratio * pwm);
      break;
    }
    case(2):
    {
      analogWrite(Left_pwm,pwm);
      analogWrite(Right_pwm, pwm);
      break;
    }
    default:
    {
      break;
    }
  }
}

void IRconfig(){
  pinMode(IR_back_right,INPUT);
  
}
double IRreading(int sensorPin){
  double D;
  switch (sensorPin)
  {
    case(IR_back_right):
    {
      Serial.println("start reading");
      D= (IR_BR.distance() - 20);
      Serial.println("done reading");
      break;
    }
    default:
    {
      break;
    }
  }
  return D;
}
