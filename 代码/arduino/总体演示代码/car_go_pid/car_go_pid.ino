// 设置对管状态
int white = 0;
int black = 1;

float err,last_err;   //pid 参数定义
float P,I,D;
//红外对管左->右引脚为PB3,PB4,PB5,PB6,PB7,PB8
const uint8_t infrared[6] = {PB3, PB4, PB5, PB6, PB7, PB8};
//L298N的in1->in4
const uint8_t moter[4] = {PA2, PA3, PA1, PA0};
//CH340连接,PA9接CH340的RXD,PA10接CH340的TXD,然后接上3.3V和GND
const uint8_t ch340[2] = {PA9, PA10};

// 设置整体速度，角速度，比例常数
int V_x = 90;
int W_z = 0;
int K = 1;

void setup() {
  // 串口设置初始化，注意PC13的灯只在低电平情况下点亮，所以用LOW
  Serial.begin(9600); 
  pinMode(PC13,OUTPUT);
  digitalWrite(PC13,LOW);

  //设置L298N输出引脚
  pinMode(PA0,OUTPUT);//PA0,PA1为右边的两个引脚
  pinMode(PA1,OUTPUT);
  pinMode(PA2,OUTPUT);//PA2,PA3为左边的两个引脚
  pinMode(PA3,OUTPUT);
  //写入状态,PA1,PA2为固定低电平，PA0和PA3为PWM输出
  digitalWrite(PA1,LOW);
  digitalWrite(PA2,LOW);

  //设置红外对管输入引脚，数字电平，设置引脚为输入上拉
  pinMode(infrared[0],INPUT_PULLUP);
  pinMode(infrared[1],INPUT_PULLUP);
  pinMode(infrared[2],INPUT_PULLUP);
  pinMode(infrared[3],INPUT_PULLUP);
  pinMode(infrared[4],INPUT_PULLUP);
  pinMode(infrared[5],INPUT_PULLUP);
}

// 红外对光管参数
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

//PID参数设置，提高P值能够使反应更快，但可能会过冲，设置i值，能够让整体轨迹更接近目标值，d值能够减小过冲（振荡）。
//调节顺序先kp后ki再kd，这里自己试一下，这里我只使用纯P调节
//I调节是有作用的，但是不要太大
//(由于是误差是离散的，不连续的，这里的D，只在数值突变的一刻，D才起调节作用，具体有没有用，理解代码，自己试一下)
int pid(){
  int output;
  float kp=45,ki=0,kd=0;
  P = err;
  I += err;
  D = err - last_err;
  output = kp*P + ki*I + kd*D;
  last_err = err;
  return output;
}

void loop() {
  sensor();
  //计算左右轮子的速度，逆向运动学
  int V_L = V_x + K*pid();
  int V_R = V_x - K*pid();

  //限幅，arduino的PWM输出为0-255，对应0-100的占空比
  if(V_L >255)V_L=255;
  if(V_L <=0)V_L=0;
  if(V_R >255)V_R=255;
  if(V_R <=0)V_R=0;

  //左右输出PWM
  analogWrite(PA0,V_R);//R
  analogWrite(PA3,V_L);//L

  Serial.print("v_L");
  Serial.println(V_L);  //测试代码
  Serial.print("v_R");
  Serial.println(V_R);  //测试代码
}

