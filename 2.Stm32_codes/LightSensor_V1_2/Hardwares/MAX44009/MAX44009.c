#include "MAX44009.h"

#define CLK_ENABLE __HAL_RCC_GPIOB_CLK_ENABLE();
iic_bus_t MAX44009_bus = 
{
	.IIC_SDA_PORT = GPIOB,
	.IIC_SCL_PORT = GPIOB,
	.IIC_SDA_PIN  = GPIO_PIN_11,
	.IIC_SCL_PIN  = GPIO_PIN_10,
};

uint8_t MAX_ReadOneReg(unsigned char RegAddr)
{
	unsigned char dat;
	dat = IIC_Read_One_Byte(&MAX44009_bus, MAX44009_ADDR, RegAddr);
	return dat;
}

void MAX_WriteOneReg(unsigned char RegAddr, unsigned char dat)
{
	IIC_Write_One_Byte(&MAX44009_bus, MAX44009_ADDR, RegAddr, dat);
}

uint8_t MAX44009_Init(void)
{
	uint8_t i = 5;
	
	CLK_ENABLE;
	IICInit(&MAX44009_bus);
	
	while(MAX_ReadOneReg(INT_STATUS_REG) != 0x00 && i)
	{
		HAL_Delay(100);
		i--;
	}
	if(!i)
	{return 1;}

	MAX_WriteOneReg(INT_ENABLE_REG,0x00);//关闭中断使能
	HAL_Delay(100);
	MAX_WriteOneReg(CONFIGURATION_REG,0x80);//连续转换、自动模式、积分时间为100/200/400/800
	HAL_Delay(100);
	
	if((MAX_ReadOneReg(CONFIGURATION_REG)&0x80) != 0x80)
	{return 2;}
	return 0;
}

void MAX_Measure(float * Lux_value)
{
	uint8_t lux[2], lux_exponent;
	uint32_t mLux_value;

    lux[0] = MAX_ReadOneReg(LUX_HIGH_REG);
    lux[1] = MAX_ReadOneReg(LUX_LOW_REG);

    lux_exponent    = ((lux[0] >> 4) & 0x0F);
    lux[0]          = ((lux[0] << 4) & 0xF0);
    lux[1]         &= 0x0F;

    mLux_value = ( lux[0] | lux[1] ) * (1<< lux_exponent);
    *Lux_value = 0.045*mLux_value;
}

