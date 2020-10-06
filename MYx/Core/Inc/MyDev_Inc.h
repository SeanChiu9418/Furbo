/*
 * MyDev_Inc.h
 *
 *  Created on: Sep 25, 2020
 *      Author: SeanChiu
 */

#ifndef INC_MYDEV_INC_H_
#define INC_MYDEV_INC_H_

#include "MyMCU_Inc.h"

// Include Selected Device Item
#ifdef Dev_PIR_1598
 #include "MyDev_PIR.h"
#endif

#ifdef Dev_SubG_S2LP
 #include "MyDev_SubG.h"
#endif

#ifdef Dev_LED
 #include "MyDev_LED.h"
#endif

#ifdef Dev_Motor
 #include "MyDev_Motor.h"
#endif

#ifdef Dev_MotorEncoder
 #include "MyDev_MotorEncoder.h"
#endif

#ifdef Dev_MIC_VM3011
#include "MyDev_MIC_VM3011.h"
#endif

#endif /* INC_MYDEV_INC_H_ */
