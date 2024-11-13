void setup() {
  // put your setup code here, to run once:
}

//PID参数设置，提高P值能够使反应更快，但可能会过冲，设置i值，能够让整体轨迹更接近目标值，d值能够减小过冲（振荡）。
//调节顺序先kp后ki再kd，这里自己试一下，这里我只使用纯P调节
//I调节是有作用的，但是不要太大
//(由于是误差是离散的，不连续的，这里的D，只在数值突变的一刻，D才起调节作用，具体有没有用，理解代码，自己试一下)
float err,last_err;   //pid 参数定义
float P,I,D;

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
  // put your main code here, to run repeatedly:
  
}
