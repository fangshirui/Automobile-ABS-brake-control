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
double speedOld = 0;
double speed = 0;
// 转速  字符串
String upload_string;

// 力矩 N*m
double braking_torque_old = 0;
double braking_torque = 0;



// 制动器工作模式 0 空闲模式  1 最大制动力  2 自由设定
int mark = 0;

// 软串口参数 mySerial  Rx = 8, Tx = 9
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
    mySerial.begin(19200);

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

bool isNewLine = false;
bool isBraking = false;
void loop() {

    while (mySerial.available()) {
        mark = mySerial.read();
        isNewLine = true;
    }

    if (isNewLine) {
        switch (mark) {
        case '0':
            // 关闭制动模式
            analogWrite(BRAKEOUTPUT, 0);
            isBraking = false;
            break;
        case '1':
            // 开启制动模式
            analogWrite(BRAKEOUTPUT, abs_table.pwm);
            isBraking = true;
            break;
        case '2':
            abs_table.pwm += 20;
            break;
        case '3':
            abs_table.pwm -= 20;
            break;
        default:
            break;
        }
        isNewLine = false;
    }


    // 上传数据 ======= 每隔 50ms  ==========
    // speed 是 40ms 的脉冲数 乘以 转速系数得到
    speed = SPEED_COEF * (double)speed_pulse;
    speed = speedOld * 0.7 + speed * 0.3;
    speedOld = speed;

    // 力矩 = x  - 500
    braking_torque = (double)analogRead(TORQUE) - 525;
    braking_torque = braking_torque_old * 0.7 + braking_torque * 0.3;
    braking_torque_old = braking_torque;

    // speed,braking_torque  单位  rpm, N*m
    upload_string = dToStr(speed) + "," + dToStr(braking_torque) + "," + dToStr(abs_table.pwm) + ",";
    mySerial.print(upload_string);
    mySerial.println(isBraking);
    // mySerial.println(millis());


    // 必须要有的延迟
    delay(87);
}
