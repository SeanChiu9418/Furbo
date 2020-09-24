/*
 * MyDev_PIR.h
 *
 *  Created on: Jul 9, 2020
 *      Author: SeanChiu
 */

#ifndef INC_MYDEV_PIR_H_
#define INC_MYDEV_PIR_H_

#define PIR_Reg_b0 		0 	// Pulse Detection Mode: count with (0) or without (1) BPF sign change
#define PIR_Reg_b1 		0	// Reserved: Must be set to dec 0
#define PIR_Reg_b2 		0	// HPF Cut-Off 0:0.4Hz 1:0.2Hz
#define PIR_Reg_b3t4 	2	// Reserved: Must be set to dec 2
#define PIR_Reg_b5t6 	0	// Signal Source 0: PIR (BPF) 1: PIR (LPF) 2: Reserved 3: Temperature Sensor
#define PIR_Reg_b7t8 	2	// Operation Modes 0: Forced Readout 1: Interrupt Readout 2: Wake Up 3: Reserved
#define PIR_Reg_b9t10 	2	// Window Time =  2s + [Reg Val]*s
#define PIR_Reg_b11t12 	1	// Pulse Counter = 1 + [Reg Val]
#define PIR_Reg_b13t16  2	// Blind Time = 0.5s + [Reg Val]*0.5s
#define PIR_Reg_b17t24  192	// Detection threshold on BPF value

#define PIR_COMMAND_TABLE \
{ "PIRI",	MyPIR_Init,    "", "" },  \
{ "PIRS",		MyPIR_WriterRegisterValue,    "", "" },  \
{ "PIRR",	MyPIR_ReadDirectLink, "", "" }


void MyPIR_Init();
void MyPIR_Write(uint32_t data);
void MyPIR_GetInterrupt();
void MyPIR_WriterRegisterValue();
//void MyPIR_ReadDirectLink(uint32_t *Data);
void MyPIR_ReadDirectLink();

#endif /* INC_MYDEV_PIR_H_ */
