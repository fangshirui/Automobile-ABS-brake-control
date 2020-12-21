#include <ABSExperimentTable.h>
#include <Arduino.h>
#include <MsTimer2.h>
#include <SoftwareSerial.h>
// 制动器力矩输出端口 5  输出pwm信号 0-5v
#define BRAKEOUTPUT 5

// 力矩检测端口  0v  -> 2.5v  -> 5v  对应  -500NM  -> 0 -> 500NM
#define TORQUE A3

// 码盘中断端口号  用于测速
#define R_INTR 2 // 自带的外部中断0 ，优先级很高，端口在D2
#define L_INTR 4 // 用户设定外部中断， 端口在D4

// 16个脉冲 40ms 对应 60 r/min
// 比值为 3.75 1个脉冲对应 3.75 r/min
// 3.75 * 60 / 95 得来的
const double SPEED_COEF = 2.3685;

ABSExperimentTable abs_table;

// 每5ms归零，实时变化的脉冲数
volatile int pulse = 0;

// 每5ms变化一次，记录当前周期总脉冲数
volatile int current_cycle_total_pulse = 0;

// 速度环, 设置测速周期为40ms, 这里就是当speedcc== 8 时，计算速度
volatile int speedcc = 0;

// 测试变量  电机 40ms 的脉冲数， 每40ms 变化一次
volatile int speed_pulse = 0;

// 转速 r/min
double speed = 0;
// 转速  字符串
String upload_string;

// 力矩 N*m
double braking_torque = 0;

// 制动器工作模式 0 空闲模式
int mark = 0;

// 软串口参数 mySerial  Rx, Tx
SoftwareSerial mySerial(8, 9);

// 硬件中断调用函数
void pulse_plus() {
    pulse++;
}

String dToStr(double raw_data) {
    char char_data[7];
    dtostrf(raw_data, 3, 2, char_data);
    return (String)char_data;
}

void countpulse() {
    // 每个周期 结束时统计当前周期总脉冲数量
    current_cycle_total_pulse = pulse;

    // 归零
    pulse = 0;
}
// 定时中断调用函数 每5ms
void inter() {
    sei();
    countpulse();

    speedcc++;
    abs_table.pulse_motor += current_cycle_total_pulse;

    if (speedcc >= 8) {
        // 此时已经过了40ms
        speedcc = 0;
        speed_pulse = abs_table.pulse_motor;
        abs_table.pulse_motor = 0;
    }
}

void setup() {

    /**
     * 测速部分
     */
     // 测速码盘 端口号 设定
    pinMode(R_INTR, INPUT_PULLUP);

    // 开启串口
    Serial.begin(9600);
    mySerial.begin(9600);

    delay(50);

    MsTimer2::set(5, inter);
    MsTimer2::start();

    // 设定外部中断判断信息  上升沿
    attachInterrupt(0, pulse_plus, RISING);

    // 设置pwm端口 输出 0 - 5v 对应 0 - 48v
    pinMode(BRAKEOUTPUT, OUTPUT);
}

/**
 * 接收指令代码
 */

int incomingByte;


void loop() {
    if (mySerial.available()) {
        incomingByte = mySerial.read();

        if (incomingByte == '0') {
            mark = 0;
        }
        else if (incomingByte == '1') {
            mark = 1;
        }
        else {
            mark = 2;
        }
    }

    if (mark == 0) {
        // 制动器关闭模式 "0"
        analogWrite(BRAKEOUTPUT, 0);
    }
    else if (mark == 1) {
        // 制动器开启最大制动力 48v
        //  "1"
        analogWrite(BRAKEOUTPUT, 255);
    }
    else {
    }


    // 上传数据 ======= 每隔 50ms  ==========
    // speed 是 40ms 的脉冲数 乘以 转速系数得到
    speed = SPEED_COEF * (double)speed_pulse;

    // 力矩 = x  - 500
    braking_torque = (double)analogRead(TORQUE) - 510;

    // speed,braking_torque  单位  rpm, N*m
    upload_string = dToStr(speed) + "," + dToStr(braking_torque);
    mySerial.println(upload_string);

    // 必须要有的延迟
    delay(1000);
}
