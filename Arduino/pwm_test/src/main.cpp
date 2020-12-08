#include <Arduino.h>
// 输出端口 5
#define BRAKINGINPUT 5

// 制动器控制pwm值  0-255 对应 0 - 5v
int brakingPwm = 0;

void setup()
{
    // put your setup code here, to run once:
    pinMode(BRAKINGINPUT, OUTPUT);
    analogWrite(BRAKINGINPUT, 255);
}

void loop()
{
}
