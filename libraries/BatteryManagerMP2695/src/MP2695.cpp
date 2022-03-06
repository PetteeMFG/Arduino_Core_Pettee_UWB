#include "MP2695.h"

// MP2695 Constructors
MP2695::MP2695()
{
    _wire = &Wire;
    _address = MP2695_ADDR;
}

MP2695::MP2695(int addr)
{
    _wire = &Wire;
    _address = addr;
}

MP2695::MP2695(TwoWire *w)
{
    _wire = w;
    _address = MP2695_ADDR;
}

MP2695::MP2695(TwoWire *w, int addr)
{
    _wire = w;
    _address = addr;
}

// MP2695 Basic Settings
bool MP2695::init()
{
    _wire->begin();
    reset();
    int regValue = 0x63;
    if (!writeReg(0, &regValue))
        return false;
    regValue = 0x2d;
    if (!writeReg(1, &regValue))
        return false;
    regValue = 0x23;
    if (!writeReg(2, &regValue))
        return false;
    regValue = 0xe2;
    if (!writeReg(8, &regValue))
        return false;
    regValue = 0x80;
    if (!writeReg(10, &regValue))
        return false;
    if (!updateRegs())
        return false;
    return true;
}

bool MP2695::reset()
{
    mp2695_reg_0_s buf;
    buf.REG_RST = 1;
    if (!writeReg(0, (int *)&buf))
        return false;
    return updateRegs();
}

bool MP2695::setChgEn(int val)
{
    mp2695_reg_2_s buf;
    if (getReg(2, (int *)&buf))
    {
        buf.CHG_EN = val;
        if (writeReg(2, (int *)&buf))
            return true;
    }
    return false;
}

bool MP2695::setBattReg(int val)
{
    mp2695_reg_2_s buf;
    if (getReg(2, (int *)&buf))
    {
        buf.BATT_REG = val;
        if (writeReg(2, (int *)&buf))
            return true;
    }
    return false;
}

bool MP2695::setIcc(int val)
{
    mp2695_reg_1_s buf;
    if (getReg(1, (int *)&buf))
    {
        buf.ICC = val;
        if (writeReg(1, (int *)&buf))
            return true;
    }
    return false;
}

// MP2695 Private Functions
bool MP2695::readReg(uint8_t i, int *val)
{
    if (i < MP2695_REGS_MAX)
    {
        _wire->beginTransmission(MP2695_ADDR);
        _wire->write(i);
        if (_wire->endTransmission() != 0)
            return false;
        _wire->requestFrom(MP2695_ADDR, 1);
        if (_wire->available())
        {
            *val = _wire->read();
            return true;
        }
    }
    return false;
}

bool MP2695::writeReg(uint8_t i, int *val)
{
    if (i < MP2695_REGS_MAX)
    {
        _wire->beginTransmission(MP2695_ADDR);
        _wire->write(i);
        _wire->write(*val);
        if (_wire->endTransmission() == 0)
            return true;
    }
    return false;
}

bool MP2695::getReg(uint8_t i, int *val)
{
    if (i < MP2695_REGS_MAX)
    {
        *val = _registers[i];
        return true;
    }
    return false;
}

bool MP2695::updateRegs()
{
    for (int i = 0; i < MP2695_REGS_MAX; i++)
    {
        int buf;
        if (readReg(i, &buf))
            _registers[i] = buf;
        else
            return false;
    }
    return true;
}