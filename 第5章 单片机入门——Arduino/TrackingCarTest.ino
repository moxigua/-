
const int M_L_IN1 = 5;   // 电机A1
const int M_L_IN2 = 6;   // 电机A2
const int M_R_IN1 = 10;   // 电机B1
const int M_R_IN2 = 11;   // 电机B2
const int EN_PIN = 8;


const int IR_PIN[] = {A0, A1, A2, A3, A4}; //红外对管引脚定义

bool is_running = true; // 正在运动标志位


static int initial_motor_speed = 80; // 期望速度
int left_motor_speed = 0;
int right_motor_speed = 0;
int irs = 0;


float Kp = 10, Ki = 0.002, Kd = 1;

//float Kp = 50, Ki = 0.002, Kd = 10;

//float Kp = 3, Ki = 0.002, Kd = 20;
float error = 0, P = 0, I = 0, D = 0, PID_value = 0;
float last_input_error = 0;
float previous_error = 0;


void read_ir_values()
{
  irs = 0;    // 初始化传感器的值
  for (int i = 0 ; i<5;i++){
    // irs转换为二进制，每一位代表一个传感器的值
    irs |= digitalRead(IR_PIN[i]) << i; 
  }

    
  switch (irs) {
    case B00000:
        if (error < 0) {
            if (last_input_error < -1)     
              error = -9;   // 向右偏离出了轨道
//            Serial.println(-9);
        } else {
            if (last_input_error > 1)
              error = 9;    // 向左偏离除了轨道
//            Serial.println(9);
        }
        break;
    case B00001: error = -7;
//      Serial.println(-7);
      break;  // 右偏
    case B00011: error = -5;
//      Serial.println(-5); 
      break;  // 右偏
    case B00010: error = -3; 
//      Serial.println(-3);
      break;  // 右偏
    case B00110: error = -1;
//      Serial.println(-1); 
      break;  // 右偏
    case B00100: error = 0;
//      Serial.println(-0); 
      break;  // 无偏差，如图5-50(a)
    case B01100: error = 1;
//      Serial.println(1); 
      break;  // 左偏，如图5-50 (e)
    case B01000: error = 3;
//      Serial.println(3); 
      break;  // 左偏，如图5-50 (d)
    case B11000: error = 5;
//      Serial.println(5); 
      break;  // 左偏，如图5-50 (d)
    case B10000: error = 7;
//      Serial.println(-7); 
      break;  // 左偏，如图5-50 (b)
    case B11111: 
      is_running = false;    
          ;
      break;
  }

  last_input_error = error;
}


void calculate_pid()
{
    P = error;
    I = I + error;
    D = error - previous_error;

    PID_value = (Kp * P) + (Ki * I) + (Kd * D);
    PID_value = constrain(PID_value, -100, 100);  // 限制输出范围在[-100,100]之间

    previous_error = error;
}

/* 左侧的电机控制 */
void left_forward_run(int m_speed)   // 左侧电机正转，可根据实际接线修改
{
  analogWrite(M_L_IN1, m_speed);     
  analogWrite(M_L_IN2, 0);
}

void left_reversal_run(int m_speed) // 左侧电机反转，可根据实际接线修改
{
   analogWrite(M_L_IN1, 0);  
   analogWrite(M_L_IN2, m_speed);
}

/* 右侧的电机控制 */
void right_forward_run(int m_speed) // 右侧电机正转，可根据实际接线修改
{
  analogWrite(M_R_IN1, m_speed);     
  analogWrite(M_R_IN2, 0);
}

void right_reversal_run(int m_speed) // 左侧电机反转，可根据实际接线修改
{
   analogWrite(M_R_IN1, 0);  
   analogWrite(M_R_IN2, m_speed);
}

void motor_control()
{
    //计算每个电机的速度
//    left_motor_speed = initial_motor_speed + PID_value;  
//    right_motor_speed = initial_motor_speed - PID_value; // 是否相反需要实际调试

  left_motor_speed = initial_motor_speed - PID_value;  
    right_motor_speed = initial_motor_speed + PID_value; // 是否相反需要实际调试
    
    constrain(left_motor_speed, -155, 155);   // 速度限定在(-155，155)
    constrain(right_motor_speed, -155, 155);
    if (left_motor_speed > 0) { /** 左侧 **/
        left_forward_run(left_motor_speed);   // 正转
} else {
        left_reversal_run(abs(left_motor_speed));  // 反转
    }
    if (right_motor_speed > 0) {/** 右侧 **/
        right_forward_run(right_motor_speed);  // 正转
    } else {
        right_reversal_run(abs(right_motor_speed));  //反转
    }
}


/* 电机的使能与失能 */
void enable_run()
{
  digitalWrite(EN_PIN, HIGH);
}

void disenable_run()
{
  digitalWrite(EN_PIN, LOW);
  analogWrite(M_L_IN1, 0);    
  analogWrite(M_L_IN2, 0);
  analogWrite(M_R_IN1, 0);  
  analogWrite(M_R_IN2, 0);
  delay(1);
}


//速度设定范围(-255,255)
void setup()
{
  Serial.begin(9600);
  pinMode(EN_PIN, OUTPUT);
  disenable_run();
  
  // 初始化
  pinMode(M_L_IN1, OUTPUT);
  pinMode(M_L_IN2, OUTPUT);
  pinMode(M_R_IN1, OUTPUT);
  pinMode(M_R_IN2, OUTPUT);
  for (int i = 0; i < 5; i++) {
    pinMode(IR_PIN[i], INPUT);
  }
  
  
  right_forward_run(initial_motor_speed);  // 正转
  left_forward_run(initial_motor_speed);  // 正转
  delay(3000);
  enable_run();
  delay(500);
}

void loop()
{
  read_ir_values();
  if (is_running){
    calculate_pid();  // 计算PID值
    motor_control();  // 根据PID控制电机
  }else{
      analogWrite(M_L_IN1, 0);    
      analogWrite(M_L_IN2, 0);
      analogWrite(M_R_IN1, 0);    
      analogWrite(M_R_IN2, 0);
      while(1);
  }
}
