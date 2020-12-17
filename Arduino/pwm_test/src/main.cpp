#include <ABSExperimentTable/ABSExperimentTable.h>
#include <Arduino.h>
#include <MsTimer2/MsTimer2.h>
// 制动器力矩输出端口 5  输出pwm信号 0-5v
#define BRAKINGINPUT 5

// 力矩检测端口  0 - 5v  对应  0 - 500NM
#define TORQUE 3

// 码盘中断端口号  用于测速
#define R_INTR 2 // 自带的外部中断0 ，优先级很高，端口在D2
#define L_INTR 4 // 用户设定外部中断， 端口在D4

// const double BIZHI = 1.875; // 640 个脉冲 40ms 对应 1200r/min
// 1.8725 * 59.8 / 94.8 得来的
const double SPEED_COEF = 1.1828;

ABSExperimentTable abs_table;

// 每5ms归零，实时变化的脉冲数
volatile int pulse = 0;

// 每5ms变化一次，记录当前周期总脉冲数
int current_cycle_total_pulse = 0;

// 速度环, 设置测速周期为40ms, 这里就是当speedcc== 8 时，计算速度
int speedcc = 0;

// 测试变量  电机 40ms 的脉冲数， 每40ms 变化一次
int speed_pulse = 0;

// 转速 r/min
double speed = 0;
// 转速  字符串
String str_speed;

// 力矩 N*m
double braking_torque = 0;

// 硬件中断调用函数
void pulse_plus()
{
    pulse++;
}

String dToStr(double raw_data)
{
    char char_data[7];
    dtostrf(raw_data, 3, 2, char_data);
    return (String)char_data;
}

void countpulse()
{
    // 每个周期 结束时统计当前周期总脉冲数量
    current_cycle_total_pulse = pulse;

    // 归零
    pulse = 0;
}
// 定时中断调用函数 每5ms
void inter()
{
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

void setup()
{

    /**
     * 测速部分 
     */
    // 测速码盘 端口号 设定
    pinMode(R_INTR, INPUT_PULLUP);

    // 开启串口
    Serial.begin(38400);

    delay(50);

    MsTimer2::set(5, inter);
    MsTimer2::start();

    // 设定外部中断判断信息  上升沿
    attachInterrupt(0, pulse_plus, CHANGE);

    /**
     * 力矩检测部分
     */
    pinMode(TORQUE, INPUT);
}
void loop()
{
    // 上传数据
    // speed 是 40ms 的脉冲数 乘以 转速系数得到
    speed = SPEED_COEF * (double)speed_pulse;
    str_speed = dToStr(speed);

    // 力矩 =  读数 * 500 / 255 = 读数 * 1.9608
    braking_torque = (double)analogRead(TORQUE) * 1.9608;

    Serial.println("Speed:r/min: " + str_speed);
    Serial.println("Torque:N*m" + dToStr(braking_torque));

    delay(1000);
}
