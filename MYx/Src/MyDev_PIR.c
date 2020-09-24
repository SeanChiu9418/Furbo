

#include "MyMCU_GPIO.h"
#include "My_Func.h"
#include "MyDev_LED.h"
#include "MyDev_PIR.h"

int	 PIRval    = 0;  // PIR signal
unsigned long	statcfg   = 0;  // status and configuration register

void MyPIR_Init()
{

	MyGPIO_PortB_Enable();

	/*Configure PIR GPIO pin to Output & Low Level*/
	// GPIOB, GPIO_PIN_6 -- PIR_SI_Pin
	// GPIOB, GPIO_PIN_7 -- PIR_WU_Pin

	MyGPIO_WritePin(PIR_SI_Port, PIR_SI_Pin|PIR_WU_Pin, GPIOPIN_RESET);
	MyGPIO_Conf2Output(PIR_SI_Port, PIR_SI_Pin|PIR_WU_Pin);

	/*Configure PIR Wake Up pin to RISING interrupt*/
	MyGPIO_Conf2IT_RISING(PIR_WU_Port,PIR_WU_Pin);

	/* EXTI interrupt init*/
	MyNVIC_SetPriority(NVIC_GPIO4_15,0,0);
	MyNVIC_EnableIRQ(NVIC_GPIO4_15);
}

void MyPIR_WriterRegisterValue()
{
	uint32_t Setting = 0;

	Setting = Setting | PIR_Reg_b17t24;
	Setting = (Setting << 4) | PIR_Reg_b13t16;
	Setting = (Setting << 2) | PIR_Reg_b11t12;
	Setting = (Setting << 2) | PIR_Reg_b9t10;
	Setting = (Setting << 2) | PIR_Reg_b7t8;
	Setting = (Setting << 2) | PIR_Reg_b5t6;
	Setting = (Setting << 2) | PIR_Reg_b3t4;
	Setting = (Setting << 1) | PIR_Reg_b2;
	Setting = (Setting << 1) | PIR_Reg_b1;
	Setting = (Setting << 1) | PIR_Reg_b0;

	MyPIR_Write(Setting);
}

void MyPIR_Write(uint32_t data)
{
	uint8_t i;
	data<<=7;
	for(i=0;i<25;i++)
	{
		MyGPIO_PinBitReset(PIR_SI_Port,PIR_SI_Pin);	//SERIN_L();

		MyGPIO_PinBitSet(PIR_SI_Port,PIR_SI_Pin);	//SERIN_H();

		if(data&0x80000000)
			MyGPIO_PinBitSet(PIR_SI_Port,PIR_SI_Pin);	//SERIN_H();
		else
			MyGPIO_PinBitReset(PIR_SI_Port,PIR_SI_Pin);	//SERIN_L();

		for(int j = 0; j <=150; j++)	// delay_us(140);
			asm("NOP");

		data<<=1;
	}
	MyGPIO_PinBitReset(PIR_SI_Port,PIR_SI_Pin);	//SERIN_L();

	for(int j = 0; j <=650; j++)	//delay_us(650);
		asm("NOP");
}

//void MyPIR_ReadDirectLink(uint32_t *Data)
void MyPIR_ReadDirectLink()
{
	int i,d;
    unsigned int  uibitmask;
    unsigned long ulbitmask;

    MyGPIO_PinBitSet(PIR_WU_Port,PIR_WU_Pin);	 				// Set DL = High, to force fast uC controlled DL read out
    MyGPIO_SetPinModeOUtput_PIR(PIR_WU_Port,PIR_WU_Pin);		// Configure PORT DL as Output

	for(d = 0; d <=150; d++)									// delay_us(140)  generates at the DirectLink line a LOW to HIGH transition and holds HIGH for at least 120µs (tDS ). ;
		asm("NOP");

	// get first 15bit out-off-range and ADC value
	uibitmask = 0x4000;              // Set BitPos
    PIRval    = 0;
    for (i=0; i < 15; i++)
    {
    	//  create low to high transition

    	MyGPIO_PinBitReset(PIR_WU_Port,PIR_WU_Pin);				// Set DL = Low, duration must be > 200 ns (tL)

    	if (i != 0)
        	MyGPIO_SetPinModeOUtput_PIR(PIR_WU_Port,PIR_WU_Pin);// Configure DL as Output


    	MyGPIO_PinBitSet(PIR_WU_Port,PIR_WU_Pin);				// Set DL = High, duration must be > 200 ns (tH)

    	MyGPIO_SetPinModeInput_PIR(PIR_WU_Port,PIR_WU_Pin);		// Configure DL as Input

    	for(d = 0; d <=15; d++)									//  delay_us(3); 	// Wait for stable low signal
    		asm("NOP");

        // If DL High set masked bit in PIRVal
        if (MyGPIO_PinInputValue(PIR_WU_Port,PIR_WU_Pin))
        	PIRval |= uibitmask;
        uibitmask>>=1;
    }
    // get 25bit status and config

    ulbitmask   = 0x1000000;                					// Set BitPos
    statcfg     = 0;

    for (i=0; i < 25; i++)
    {
    	//  create low to high transition

    	MyGPIO_PinBitReset(PIR_WU_Port,PIR_WU_Pin);				// Set DL = Low, duration must be > 200 ns (tL)

    	MyGPIO_SetPinModeOUtput_PIR(PIR_WU_Port,PIR_WU_Pin);	// Configure DL as Output

    	MyGPIO_PinBitSet(PIR_WU_Port,PIR_WU_Pin);				// Set DL = High, duration must be > 200 ns (tH)

    	MyGPIO_SetPinModeInput_PIR(PIR_WU_Port,PIR_WU_Pin);		// Configure DL as Input

    	for(d = 0; d <=15; d++)									//  delay_us(3);	// Wait for stable low signal, tbd empirically using scope
    		asm("NOP");

        // If DL High set masked bit
        if (MyGPIO_PinInputValue(PIR_WU_Port,PIR_WU_Pin))
        	statcfg |= ulbitmask;

        ulbitmask>>=1;
    }


    MyGPIO_PinBitReset(PIR_WU_Port,PIR_WU_Pin);					// Set DL = Low
    MyGPIO_SetPinModeOUtput_PIR(PIR_WU_Port,PIR_WU_Pin);		// Configure DL as Output
    asm("NOP");
    MyGPIO_SetPinModeInput_PIR(PIR_WU_Port,PIR_WU_Pin);			// Configure DL as Input
    PIRval &= 0x3FFF;                							// clear unused bit

    if (!(statcfg & 0x60))
    {             												// ADC source to PIR band pass         // number in 14bit two's complement
    	if(PIRval & 0x2000)
    		PIRval -= 0x4000;
    }
    My_printf(int2str(PIRval));

}

void MyPIR_ResetMotionDetect()
{
	MyNVIC_DisableIRQ(NVIC_GPIO4_15);
	// Clear MotionDetect Interrupt
	MyGPIO_PinBitReset(PIR_WU_Port,PIR_WU_Pin);
	MyGPIO_SetPinModeOUtput_PIR(PIR_WU_Port,PIR_WU_Pin);

	for(int d = 0; d <=150; d++)									// delay_us(140)  generates at the DirectLink line a LOW to HIGH transition and holds HIGH for at least 120µs (tDS ). ;
		asm("NOP");

	/*Configure PIR Wake Up pin to RISING interrupt*/
	MyGPIO_Conf2IT_RISING(PIR_WU_Port,PIR_WU_Pin);

	MyNVIC_EnableIRQ(NVIC_GPIO4_15);
}


void MyPIR_GetInterrupt()
{
	static int i = 0;
	if (i == 0)
	{
		MyLED_Green(LED_On);
		i++;
	}
	else
	{
		MyLED_Green(LED_Off);
		i=0;
	}

	MyPIR_ResetMotionDetect();

}
