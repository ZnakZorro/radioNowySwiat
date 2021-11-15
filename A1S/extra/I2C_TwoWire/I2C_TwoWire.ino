#include <Wire.h>
#define I2C_Freq 100000

// I2C GPIOs
#define IIC_CLK       32
#define IIC_DATA      33

#define IIC_CLK_2       23
#define IIC_DATA_2      18

TwoWire I2C1 = TwoWire(0); //I2C1 bus
TwoWire I2C2 = TwoWire(1); //I2C2 bus

 
void setup()
{
    // Init buses
    I2C1.begin(IIC_DATA,  IIC_CLK,  I2C_Freq); // Start I2C1
    I2C2.begin(IIC_DATA2, IIC_CLK2, I2C_Freq); // Start I2C2
    
}

void loop() 
{
    //Talk to device connected to I2C1
    I2C1.beginTransmission(0x3c);
    I2C1.write(0x40);
    I2C1.endTransmission();

    //Talk to device connected to I2C2
    I2C2.beginTransmission(0x30);
    I2C2.write(0x45);
    I2C2.endTransmission();
}
