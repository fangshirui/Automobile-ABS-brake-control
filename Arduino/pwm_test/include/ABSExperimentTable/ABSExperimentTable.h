/*
 * @Author: FangShiRui
 * @Date: 2020-07-16 10:01:43
 * @LastEditors: FangShiRui
 * @LastEditTime: 2020-07-16 10:38:35
 * @Description: file content
 * 
 */

#ifndef ABS_Table_
#define ABS_Table_

#include <Arduino.h>

class ABSExperimentTable {

public:
    // 电机40ms的脉冲数,每5ms变化一次，40ms后会归零。重新变化。
    int pulse_motor;
};

#endif