#include <Arduino.h>
#line 1 "d:\\Github_Projects\\INA226_for_esp32\\main\\main.ino"
#include "XMU_ina226.h"
#define SDA 27
#define SCL 26
#define addr_1 0x44
#define addr_2 0x45
String req = "";


PowerManager PM1(addr_1);
PowerManager PM2(addr_2);

#line 12 "d:\\Github_Projects\\INA226_for_esp32\\main\\main.ino"
void setup();
#line 20 "d:\\Github_Projects\\INA226_for_esp32\\main\\main.ino"
void loop();
#line 35 "d:\\Github_Projects\\INA226_for_esp32\\main\\main.ino"
void req_deal(String req);
#line 12 "d:\\Github_Projects\\INA226_for_esp32\\main\\main.ino"
void setup()
{
    Serial.begin(115200);
    Serial.setTimeout(5);
    PM1.setWire(SDA, SCL);
    PM2.setWire(SDA, SCL);
}

void loop()
{
    if (Serial.available()) {
        req = Serial.readStringUntil('\n');
        Serial.println(req);    
    }

    if (req != ""){
        req_deal(req);
    }
    req = "";

}

// 查询指定电机的功率
void req_deal(String req)
{
    float volt=0, current=0, power=0;
    if (req.substring(0, 1) == "?")
    {
        if (req.substring(1, 2) == "1")
        {
            volt = PM1.getVoltage();
            current = PM1.getCurrent();
            power = PM1.getPower();
        }
        else if (req.substring(1, 2) == "2")
        {
            volt = PM2.getVoltage();
            current = PM2.getCurrent();
            power = PM2.getPower();
        }
        Serial.printf("volt:  %.2f V  current:  %.4f A  power:  %.3f W\n ", volt, current, power);
    }
}
