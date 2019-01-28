volatile long count = 0;
boolean A,B;
unsigned long timep, time, etime;
byte state, statep;

#define m11 9
#define m12 10 

#define D0 19
#define D1 18
#define D2 17
#define D3 16


int speed_actual = 0;
double Kp = 0.5;
double Kd = 1;
int val= 0;
int mode;
unsigned long lastTime = 0;
unsigned long lastTime_print = 0;
#define LOOPTIME 100

void setup() {
  Serial.begin(115200);
  pinMode(2, INPUT);
  pinMode(3, INPUT);
  attachInterrupt(0,Achange,CHANGE);
  attachInterrupt(1,Bchange,CHANGE);
  timep = millis(); 
  A = digitalRead(2);
  B = digitalRead(3);
  if ((A==HIGH)&&(B==HIGH)) statep = 1;
  if ((A==HIGH)&&(B==LOW)) statep = 2; 
  if ((A==LOW)&&(B==LOW)) statep = 3;
  if((A==LOW)&&(B==HIGH)) statep = 4;
  pinMode(m11, OUTPUT);
  pinMode(m12, OUTPUT);


 pinMode(D0, INPUT);
 pinMode(D1, INPUT);
 pinMode(D2, INPUT);
 pinMode(D3, INPUT);

 pinMode(m11, OUTPUT);
 pinMode(m12, OUTPUT);

}

void loop() {
   int temp1=digitalRead(D0);
   int temp2=digitalRead(D1);
   int temp3=digitalRead(D2);
   int temp4=digitalRead(D3);
 
        if(temp1==0 && temp2==0 && temp3==0 && temp4==1)
   mode=0;//stop
   else if(temp1==0 && temp2==0 && temp3==1 && temp4==0)
   mode=1;//forward speed
   else if(temp1==0 && temp2==1 && temp3==0 && temp4==1)
   mode=2;//forward slow
   else if(temp1==0 && temp2==0 && temp3==1 && temp4==1)
   mode=3;//backward fast
   else if(temp1==0 && temp2==1 && temp3==1 && temp4==0)
   mode=4;//backward slow
  if((millis() - lastTime) >= LOOPTIME) {
    lastTime = millis(); 

    if(mode==0){
    analogWrite(m11, LOW);
    analogWrite(m12, LOW);
      }
    else if(mode==1){
     findMotorData(255);
    analogWrite(m11, LOW);
    analogWrite(m12, val);
      }
    else if(mode==2){
    findMotorData(100);  
    analogWrite(m11, LOW);
    analogWrite(m12, val);
      }
    else if(mode==3){
     findMotorData(255);
    analogWrite(m11, val);
    analogWrite(m12, LOW);
      }
    else if(mode==4){
    findMotorData(100);  
    analogWrite(m11, val);
    analogWrite(m12, LOW);
      }
      else{
    analogWrite(m11, LOW);
    analogWrite(m12, LOW);}

     }

}

void findMotorData(int speed_req) {
  static long count_prev = 0;
  speed_actual = ((count - count_prev)*(60*(1000/LOOPTIME)))/4000;
  count_prev = count;
  
  float pidTerm = 0;
  int error = 0;
  int last_error = 0;
  error = abs(speed_req) - abs(speed_actual);
  pidTerm = (Kp * error) + (Kd * (error - last_error));
  last_error = error;
  val = constrain(pidTerm, 0, 255);
}



void Achange() 
{
  A = digitalRead(2);
  B = digitalRead(3);

  if ((A==HIGH)&&(B==HIGH)) state = 1;
  if ((A==HIGH)&&(B==LOW)) state = 2;
  if ((A==LOW)&&(B==LOW)) state = 3;
  if((A==LOW)&&(B==HIGH)) state = 4;
  switch (state)
  {
    case 1:
    {
      if (statep == 2) count++;
      if (statep == 4) count--;
      break;
    }
    case 2:
    {
      if (statep == 1) count--;
      if (statep == 3) count++;
      break;
    }
    case 3:
    {
      if (statep == 2) count --;
      if (statep == 4) count ++;
      break;
    }
    default:
    {
      if (statep == 1) count++;
      if (statep == 3) count--;
    }
  }
  statep = state;
}

void Bchange()
{
  A = digitalRead(2);
  B = digitalRead(3);

  if ((A==HIGH)&&(B==HIGH)) state = 1;
  if ((A==HIGH)&&(B==LOW)) state = 2;
  if ((A==LOW)&&(B==LOW)) state = 3;
  if((A==LOW)&&(B==HIGH)) state = 4;
  switch (state)
  {
    case 1:
    {
      if (statep == 2) count++;
      if (statep == 4) count--;
      break;
    }
    case 2:
    {
      if (statep == 1) count--;
      if (statep == 3) count++;
      break;
    }
    case 3:
    {
      if (statep == 2) count --;
      if (statep == 4) count ++;
      break;
    }
    default:
    {
      if (statep == 1) count++;
      if (statep == 3) count--;
    }
  }
  statep = state;
}
