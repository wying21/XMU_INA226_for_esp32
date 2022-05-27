#line 1 "d:\\Github_Projects\\INA226_for_esp32\\main\\XMU_ina226.h"
#ifndef XMU_ina226_H //预编译指令，防止重复定义类
#define XMU_ina226_H
#include "Wire.h"

const uint16_t INA226_CONFIGURATION = 0x4427; // Device configuration - 0x4427 corresponds to:
const uint8_t INA226_CONFIG_REG = 0;          // Register locations
const uint8_t INA226_SHUNT_VOLTAGE_REG = 1;
const uint8_t INA226_VOLTAGE_REG = 2;
const uint8_t INA226_POWER_REG = 3;
const uint8_t INA226_CURRENT_REG = 4;
const uint8_t INA226_CALIBRATION_REG = 5;

class PowerManager
{
public:
    PowerManager(uint8_t addr);
    void init(uint8_t addr);
    void setWire(int sda, int scl);
    void writeRegister(uint8_t reg, uint16_t value);
    void setInaConfig(float maxAmps, float resistorVal);
    uint16_t readRegister(uint8_t reg);
    float getVoltage();
    float getCurrent();
    float getPower();

private:
    int addr;
    float power_lsb = 0.0025;
    float voltage_lsb = 0.00125;
    float current_lsb = 0.0001;
    float shunt_mohm = 2;
    float max_amps = 3.6;
};

PowerManager ::PowerManager(uint8_t addr)
{
    this->addr = addr;
    // this->setInaConfig(max_amps, shunt_mohm);
}

//实例化一个INA226芯片，传入INA226芯片地址。
void PowerManager ::init(uint8_t addr)
{
    this->addr = addr;
}

// 设置i2c管脚号
void PowerManager ::setWire(int sda, int scl)
{
    Wire.begin(sda, scl);
    this->setInaConfig(max_amps, shunt_mohm);
}

// 初始化INA226芯片
void PowerManager ::setInaConfig(float maxAmps, float resistorVal)
{
    writeRegister(INA226_CONFIG_REG, INA226_CONFIGURATION);
    uint16_t calibration = 0.00512 / (current_lsb * resistorVal / 1000);
    writeRegister(INA226_CALIBRATION_REG, calibration);
}

// 将一个值通过i2c总线传输到指定的寄存器中
void PowerManager ::writeRegister(uint8_t reg, uint16_t value)
{
    Wire.beginTransmission(addr);
    Wire.write(reg);
    Wire.write((value >> 8) & 0xFF);
    Wire.write(value & 0xFF);
    Wire.endTransmission();
}

// 读出指定寄存器的值
uint16_t PowerManager ::readRegister(uint8_t reg)
{
    uint16_t res = 0x0000;
    Wire.beginTransmission(addr);
    Wire.write(reg);
    if (Wire.endTransmission() == 0)
    {
        if (Wire.requestFrom(addr, 2) >= 2)
        {
            res = Wire.read() * 256;
            res += Wire.read();
        }
    }
    return res;
}

// 读出电压值
float PowerManager ::getVoltage()
{
    return readRegister(INA226_VOLTAGE_REG) * voltage_lsb * 0.9461;
}

// 读出电流值
float PowerManager ::getCurrent()
{
    return readRegister(INA226_CURRENT_REG) * current_lsb * 1.2051; 
}

// 读出功率值
float PowerManager ::getPower()
{
    return readRegister(INA226_POWER_REG) * power_lsb * 1.1378;
}

#endif