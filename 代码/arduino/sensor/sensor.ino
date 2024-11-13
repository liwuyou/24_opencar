// 设置对管状态
int white = 0;
int black = 1;

float err;
//红外对管左->右引脚为PB3,PB4,PB5,PB6,PB7,PB8
const uint8_t infrared[6] = {PB3, PB4, PB5, PB6, PB7, PB8};

//CH340连接,PA9接CH340的RXD,PA10接CH340的TXD,然后接上3.3V和GND
const uint8_t ch340[2] = {PA9, PA10};


void setup() {
  // 串口设置初始化，注意PC13的灯只在低电平情况下点亮，所以用LOW
  Serial.begin(9600); 
  pinMode(PC13,OUTPUT);
  digitalWrite(PC13,LOW);

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

void loop() {
  sensor();
  Serial.println(err);  // 向电脑发送err值
}

