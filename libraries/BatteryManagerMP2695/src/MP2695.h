#pragma once

#include "Arduino.h"
#include <Wire.h>

#define MP2695_ADDR 0x6B
#define MP2695_REGS_MAX 11

struct mp2695_reg_0_s
{
    uint8_t IINLIM : 3;
    uint8_t VINMIN : 3;
    uint8_t EN_TIMER : 1;
    uint8_t REG_RST : 1;
};

struct mp2695_reg_1_s
{
    uint8_t IPRE : 2;
    uint8_t EN_NTC : 1;
    uint8_t ICC : 5;
};

// Each bit 100ma, with 500ma offset.
enum
{
    ICC_500MA,
    ICC_600MA,
    ICC_700MA,
    ICC_800MA,
    ICC_900MA,
    ICC_1000MA,
    ICC_1100MA,
    ICC_1200MA,
};

struct mp2695_reg_2_s
{
    uint8_t CHG_EN : 1;
    uint8_t ITERM : 2;
    uint8_t JEITA_DIS : 1;
    uint8_t BATT_REG : 3;
    uint8_t BATT_OVP_DIS : 1;
};

struct mp2695_reg_5_s
{
    uint8_t res_0 : 1;
    uint8_t USB1_PLUG_EN : 1;
    uint8_t IPPM_STAT : 1;
    uint8_t VPPM_STAT : 1;
    uint8_t CHG_STAT : 2;
};

enum
{
    BATT_REG_3V6,
    BATT_REG_4V1,
    BATT_REG_4V2,
    BATT_REG_4V3,
    BATT_REG_4V35,
    BATT_REG_4V4,
    BATT_REG_4V45
};

enum
{
    CHG_EN_DISABLE,
    CHG_EN_ENABLE
};

enum
{
    CHG_STAT_NOT,
    CHG_STAT_PRE,
    CHG_STAT_CHG,
    CHG_STAT_DONE
};

class MP2695
{
public:
    // MP2695 Constructors
    MP2695();
    MP2695(int addr);
    MP2695(TwoWire *w, int addr);
    MP2695(TwoWire *w);

    // MP2695 Basic Settings
    bool init();
    bool reset();
    bool setChgEn(int val);
    bool setBattReg(int val);
    bool setIcc(int val);
    bool getReg(uint8_t i, int *val);
    bool readReg(uint8_t i, int *val);
    bool writeReg(uint8_t i, int *val);
    bool updateRegs();

private:
    TwoWire *_wire;
    int _address;
    int _registers[MP2695_REGS_MAX];
};