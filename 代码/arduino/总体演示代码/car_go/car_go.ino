// 设置对管状态
int white = 0;
int black = 1;
float err,last_err;   //pid 参数定义

//红外对管左->右引脚为PB3,PB4,PB5,PB6,PB7,PB8
const uint8_t infrared[6] = {PB3, PB4, PB5, PB6, PB7, PB8};
//L298N的in1->in4
const uint8_t moter[6] = {PA2, PA3, PA1, PA0};

int V_x = 90;
int W_z = 0;
int K = 30;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600); 
  pinMode(PC13,OUTPUT);
  digitalWrite(PC13,LOW);

  //设置L298N输出引脚
  pinMode(PA0,OUTPUT);//PA0,PA1为右边的两个引脚
  pinMode(PA1,OUTPUT);
  pinMode(PA2,OUTPUT);//PA2,PA3为左边的两个引脚
  pinMode(PA3,OUTPUT);
  //写入状态
  //analogWrite(PA0,100);
  digitalWrite(PA1,LOW);
  //analogWrite(PA2,100);
  digitalWrite(PA2,LOW);
  //analogWrite(PA0,100);
  //设置红外对管输入引脚，数字电平
  pinMode(infrared[0],INPUT_PULLUP);
  pinMode(infrared[1],INPUT_PULLUP);
  pinMode(infrared[2],INPUT_PULLUP);
  pinMode(infrared[3],INPUT_PULLUP);
  pinMode(infrared[4],INPUT_PULLUP);
  pinMode(infrared[5],INPUT_PULLUP);
}


void sensor(){
  int a_1 = digitalRead(infrared[0]);
  int a_2 = digitalRead(infrared[1]);
  int a_3 = digitalRead(infrared[2]);
  int a_4 = digitalRead(infrared[3]);
  int a_5 = digitalRead(infrared[4]);
  int a_6 = digitalRead(infrared[5]);
  if(a_1==black && a_2==white && a_3==white && a_4==white && a_5==white && a_6==white){
      err = -2;
  }
  else if(a_1==white && a_2==black && a_3==white && a_4==white && a_5==white && a_6==white){
      err = -1;
  }
  else if(a_1==white && a_2==white && a_3==black && a_4==white && a_5==white && a_6==white){
      err = -0.5;
  }
  else if(a_1==white && a_2==white && a_3==black && a_4==black && a_5==white && a_6==white){
      err = 0;
  } 
  else if(a_1==white && a_2==white && a_3==white && a_4==black && a_5==white && a_6==white){
      err = 0.5;
  }
  else if(a_1==white && a_2==white && a_3==white && a_4==white && a_5==black && a_6==white){
      err = 1;
  }
  else if(a_1==white && a_2==white && a_3==white && a_4==white && a_5==white && a_6==black){
      err = 2;
  }
  
}

int pid(){
  int output;
  float kp=17,ki=0.001,kd=0;
  float P,I,D;
  P = err;
  I += err;
  D = err - last_err;
  output = kp*P + ki*I + kd*D;
  last_err = err;
  return output;
}

void loop() {
  sensor();
  int V_L = V_x + K*err;
  int V_R = V_x - K*err;
  analogWrite(PA0,V_R);//R
  analogWrite(PA3,V_L);//L
  Serial.print("v_L");
  Serial.println(V_L);  //测试代码
  Serial.print("v_R");
  Serial.println(V_R);  //测试代码
}

