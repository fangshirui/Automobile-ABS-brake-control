#include <Arduino.h>
#include <MsTimer2.h>
#include <PinChangeInt.h>
// 码盘中断端口号  用于测速
#define R_INTR 2 // 自带的外部中断0 ，优先级很高，端口在D2 轮速
#define L_INTR 4 // 用户设定外部中断， 端口在D4 车速  飞轮转速

#define OUTPUT_PORT 5

// 每5ms 归零 实时变化的脉冲数  
volatile int pulse_wheel = 0;

// 每5ms 归零 实时变化的脉冲数
volatile int pulse_car = 0;


double kp = 10;
double ki = 0.1;
double kd = 0;


// 磁粉离合器 输出 电压 0-5v 对应  0 - 255
double out = 0;

// 当前滑移率
double slip_rate = 0;
const double target_slip_rate = 0.2;

// 当前滑移率偏差
double slip_rate_bias = 0;
double slip_rate_bias_old = 0;

// 滑移率偏差 积分
double integration_bias_of_slip = 0;

// 滑移率偏差 微分
double dif_bias_of_slip = 0;

void pwm_out(double kp, double ki, double kd) {

    // 当前滑移率
    slip_rate = (pulse_car - pulse_wheel) / pulse_car;

    // 当前滑移率偏差
    slip_rate_bias = slip_rate - target_slip_rate;

    // 滑移率偏差积分
    integration_bias_of_slip += slip_rate_bias;

    // 滑移率偏差变化量  微分
    dif_bias_of_slip = slip_rate_bias - slip_rate_bias_old;

    slip_rate_bias_old = slip_rate_bias;

    out = kp * slip_rate_bias + ki * integration_bias_of_slip + kd * dif_bias_of_slip;

    // 幅值限制
    out = constrain(out, 0, 255);

    analogWrite(OUTPUT_PORT, out);

}

void inter() {
    // 中断初始化
    sei();

    // 根据 脉冲数 计算 PID 输出 
    pwm_out(kp, ki, kd);

    // 脉冲归零
    pulse_car = 0;
    pulse_wheel = 0;

}

void pulse_plus_wheel() {
    pulse_wheel++;
}

void pulse_plus_car() {
    pulse_car++;
}
void setup() {

    /**
     * 测速部分
     */
     // 测速码盘 端口号 设定
    pinMode(R_INTR, INPUT_PULLUP);
    pinMode(L_INTR, INPUT_PULLUP);

    // 开启串口
    Serial.begin(19200);

    delay(50);

    MsTimer2::set(10, inter);
    MsTimer2::start();

    // 设定外部中断判断信息  上升沿
    attachInterrupt(0, pulse_plus_wheel, CHANGE);
    attachPinChangeInterrupt(L_INTR, pulse_plus_car, CHANGE);


    // 设置pwm端口 输出 0 - 5v 对磁粉离合器输出 0-800NM  1.6Nm / 0.01v
    pinMode(OUTPUT_PORT, OUTPUT);
}

void loop() {

}