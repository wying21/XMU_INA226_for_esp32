# 1 "d:\\Github_Projects\\INA226_for_esp32\\main\\main.ino"
# 2 "d:\\Github_Projects\\INA226_for_esp32\\main\\main.ino" 2




String req = "";


PowerManager PM1(0x44);
PowerManager PM2(0x45);

void setup()
{
    Serial.begin(115200);
    Serial.setTimeout(5);
    PM1.setWire(27, 26);
    PM2.setWire(27, 26);
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
