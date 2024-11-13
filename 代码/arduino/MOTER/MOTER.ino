//L298N的in1->in4
const uint8_t moter[4] = {PA2, PA3, PA1, PA0};

void setup() {
  // put your setup code here, to run once:

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
}

void loop() {

  //左右输出PWM，改变数值能够改变速度
  analogWrite(PA0,0);//R
  analogWrite(PA3,150);//L

}
