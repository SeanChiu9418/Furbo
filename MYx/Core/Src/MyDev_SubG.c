/*
 * MyDev_SubG.c
 *
 *  Created on: Aug 6, 2020
 *      Author: SeanChiu
 */

#include "MyDev_SubG.h"
#include "main.h"

//#define DELAY_CS_SCLK			0x70	/* Delay between CSn falling edge & start of SCLK */
//extern SPI_HandleTypeDef hspi1;
//#define SPI_ENTER_CRITICAL()           __disable_irq()
//#define SPI_EXIT_CRITICAL()            __enable_irq()
//#define SdkEvalSPICSLow()        HAL_GPIO_WritePin(SPI1_CSN_GPIO_Port, SPI1_CSN_Pin, GPIO_PIN_RESET)
//#define SdkEvalSPICSHigh()       HAL_GPIO_WritePin(SPI1_CSN_GPIO_Port, SPI1_CSN_Pin, GPIO_PIN_SET)
/*#define WAIT_FOR_SPI_TC()       	{while(!__HAL_DMA_GET_FLAG(hspi1.hdmarx, __HAL_DMA_GET_TC_FLAG_INDEX(hspi1.hdmarx)));\
                                          do{HAL_DMA_IRQHandler(hspi1.hdmarx);\
                                          HAL_DMA_IRQHandler(hspi1.hdmatx);\
                                          }while(HAL_SPI_GetState(&hspi1) != HAL_SPI_STATE_READY);}
*/
#define VCO_CENTER_FREQ                 3600000000           /*!< VCO center frequency in Hz */
#define HIGH_BAND_FACTOR                4     /*!< Band select factor for high band. Factor B in the equation 2 */
#define MIDDLE_BAND_FACTOR              8      /*!< Band select factor for middle band. Factor B in the equation 2 */
#define HIGH_BAND_LOWER_LIMIT           825900000   /*!< Lower limit of the high band: 860 MHz   (S2-LPQTR)*/
#define HIGH_BAND_UPPER_LIMIT           1056000000   /*!< Upper limit of the high band: 940 MHz  (S2-LPCBQTR)*/
#define MIDDLE_BAND_LOWER_LIMIT         412900000   /*!< Lower limit of the middle band: 430 MHz (S2-LPQTR)*/
#define MIDDLE_BAND_UPPER_LIMIT         527100000   /*!< Upper limit of the middle band: 470 MHz (S2-LPCBQTR)*/

#define HEADER_WRITE_MASK		0x00 /* !< Write mask for header byte */
#define HEADER_READ_MASK		0x01 /* !< Read mask for header byte */
#define HEADER_ADDRESS_MASK		0x00 /* !< Address mask for header byte */
#define HEADER_COMMAND_MASK		0x80 /* !< Command mask for header byte */
#define BUILT_HEADER(add_comm, w_r) (add_comm | w_r)  /*!< macro to build the header byte*/
#define WRITE_HEADER    BUILT_HEADER(HEADER_ADDRESS_MASK, HEADER_WRITE_MASK) /*!< macro to build the write header byte*/
#define READ_HEADER     BUILT_HEADER(HEADER_ADDRESS_MASK, HEADER_READ_MASK)  /*!< macro to build the read header byte*/
#define COMMAND_HEADER  BUILT_HEADER(HEADER_COMMAND_MASK, HEADER_WRITE_MASK) /*!< macro to build the command header byte*/

#define IS_FREQUENCY_BAND_HIGH(FREQUENCY) ((FREQUENCY)>=HIGH_BAND_LOWER_LIMIT && \
                                           (FREQUENCY)<=HIGH_BAND_UPPER_LIMIT)

#define IS_FREQUENCY_BAND_MIDDLE(FREQUENCY) ((FREQUENCY)>=MIDDLE_BAND_LOWER_LIMIT && \
                                             (FREQUENCY)<=MIDDLE_BAND_UPPER_LIMIT)

#define IS_FREQUENCY_BAND(FREQUENCY) (IS_FREQUENCY_BAND_HIGH(FREQUENCY) || \
                                      IS_FREQUENCY_BAND_MIDDLE(FREQUENCY))

/**
 * @brief  S2LP Status global variable.
 *         This global variable of @ref S2LPStatus type is updated on every SPI transaction
 *         to maintain memory of S2LP Status.
 */
volatile S2LPStatus g_xStatus
;/**
* @brief  The Xtal frequency. To be set by the user (see SetXtalFreq() function)
*/
static uint32_t s_lXtalFrequency=50000000;

/**
* @brief  It represents the available channel bandwidth times 10 for 26 Mhz xtal.
* @note   The channel bandwidth for others xtal frequencies can be computed since this table
*         multiplying the current table by a factor xtal_frequency/26e6.
*/
static const uint16_t s_vectnBandwidth26M[90]=
{
  8001, 7951, 7684, 7368, 7051, 6709, 6423, 5867, 5414, \
    4509, 4259, 4032, 3808, 3621, 3417, 3254, 2945, 2703, \
      2247, 2124, 2015, 1900, 1807, 1706, 1624, 1471, 1350, \
        1123, 1062, 1005,  950,  903,  853,  812,  735,  675, \
          561,  530,  502,  474,  451,  426,  406,  367,  337, \
            280,  265,  251,  237,  226,  213,  203,  184,  169, \
              140,  133,  126,  119,  113,  106,  101,   92,   84, \
                70,   66,   63,   59,   56,   53,   51,   46,   42, \
                  35,   33,   31,   30,   28,   27,   25,   23,   21, \
                    18,   17,   16,   15,   14,   13,   13,   12,   11
};


static uint8_t tx_buff[130];
static uint8_t rx_buff[130];


uint16_t MySubG_S2LPSpiReadRegisters(uint8_t address, uint8_t n_bytes, uint8_t* buffer)
{
  tx_buff[0]=READ_HEADER;
  tx_buff[1]=address;

  uint16_t status;

#if 1
  MySPI_TransmitReceive_DMA(tx_buff, rx_buff, 2+n_bytes);
#else
  SPI_ENTER_CRITICAL();
  SdkEvalSPICSLow();
  for(volatile uint32_t i=0;i<DELAY_CS_SCLK;i++);

  HAL_SPI_TransmitReceive_DMA(&hspi1, tx_buff, rx_buff, 2+n_bytes);
  WAIT_FOR_SPI_TC();

  for(volatile uint32_t i=0;i<DELAY_CS_SCLK;i++);
  SdkEvalSPICSHigh();
  SPI_EXIT_CRITICAL();
#endif
  for(uint32_t i=0;i<n_bytes;i++)
  {
    buffer[i]=rx_buff[i+2];
  }

  ((uint8_t*)&status)[1]=rx_buff[0];
  ((uint8_t*)&status)[0]=rx_buff[1];

  return status;
}

uint16_t MySubG_S2LPSpiWriteRegisters(uint8_t address, uint8_t n_bytes, uint8_t* buffer)
{
  tx_buff[0]=WRITE_HEADER;
  tx_buff[1]=address;

  uint16_t status;

  for(uint32_t i=0;i<n_bytes;i++)
  {
    tx_buff[i+2]=buffer[i];
  }

#if 1
  MySPI_TransmitReceive_DMA(tx_buff, rx_buff, 2+n_bytes);
#else
  SPI_ENTER_CRITICAL();

  /* Puts the SPI chip select low to start the transaction */
  SdkEvalSPICSLow();
  for(volatile uint32_t i=0;i<DELAY_CS_SCLK;i++);

  HAL_SPI_TransmitReceive_DMA(&hspi1, tx_buff, rx_buff, 2+n_bytes);
  WAIT_FOR_SPI_TC();

  for(volatile uint32_t i=0;i<DELAY_CS_SCLK;i++);
  /* Puts the SPI chip select high to end the transaction */
  SdkEvalSPICSHigh();

  SPI_EXIT_CRITICAL();
#endif
  ((uint8_t*)&status)[1]=rx_buff[0];
  ((uint8_t*)&status)[0]=rx_buff[1];

  return status;
}

uint16_t MySubG_S2LPSpiCommandStrobes(uint8_t command)
{
  tx_buff[0]=COMMAND_HEADER;
  tx_buff[1]=command;

  uint16_t status;

#if 1
  MySPI_TransmitReceive_DMA(tx_buff, rx_buff, 2);
#else
  SPI_ENTER_CRITICAL();
  SdkEvalSPICSLow();
  for(volatile uint32_t i=0;i<DELAY_CS_SCLK;i++);


  HAL_SPI_TransmitReceive_DMA(&hspi1, tx_buff, rx_buff, 2);
  WAIT_FOR_SPI_TC();

  for(volatile uint32_t i=0;i<DELAY_CS_SCLK;i++);
  SdkEvalSPICSHigh();
  SPI_EXIT_CRITICAL();
#endif

  ((uint8_t*)&status)[1]=rx_buff[0];
  ((uint8_t*)&status)[0]=rx_buff[1];

  return status;
}

void MySubG_S2LPRadioInitAction(void)
{

  SRadioInit xRadioInit;
  uint8_t err;

  xRadioInit.lFrequencyBase = MyCLI_UnsignedCommandArgument(0);
  xRadioInit.xModulationSelect = (ModulationSelect)MyCLI_UnsignedCommandArgument(1);
  xRadioInit.lDatarate = MyCLI_UnsignedCommandArgument(2);
  xRadioInit.lFreqDev = MyCLI_UnsignedCommandArgument(3);
  xRadioInit.lBandwidth = MyCLI_UnsignedCommandArgument(4);

  err = MySubG_S2LPRadioInit(&xRadioInit);
	//responsePrintf("{&N API call...&tx}\r\n",
         //"S2LPRadioInit", "error", err );

}

/**
* @brief  Return the XTAL frequency.
* @param  void.
* @retval uint32_t XTAL frequency.
*/
uint32_t MySubG_S2LPRadioGetXtalFrequency(void)
{
  return s_lXtalFrequency;
}

/**
 * @brief  Get Radio parameters.
 *         <p><b>HowToCall</b>:  S2LPRadioGetInfo</p>
 * @param  None
 * @retval Radio parameters.
 */
void MySubG_S2LPRadioGetInfoAction(void)
{
  SRadioInit xRadioInit;
  MySubG_S2LPRadioGetInfo(&xRadioInit);
  uint32_t xtal = MySubG_S2LPRadioGetXtalFrequency();

  My_printf(int2str(xRadioInit.lFrequencyBase));
  My_printf(int2str(xRadioInit.xModulationSelect));
  My_printf(int2str(xRadioInit.lDatarate));
  My_printf(int2str(xRadioInit.lFreqDev));
  My_printf(int2str(xRadioInit.lBandwidth));
  My_printf(int2str(xtal));
/*
  responsePrintf("{&N API call...&t4x&tx&t4x&t4x&t4x&t4x}\r\n", "S2LPRadioGetInfo",
   "Frequency_base", (uint32_t)xRadioInit.lFrequencyBase,
   "Modulation", xRadioInit.xModulationSelect,
   "Data_rate", (uint32_t)xRadioInit.lDatarate,
   "Frequency_deviation", (uint32_t)xRadioInit.lFreqDev,
   "Channel_filter_bandwidth", (uint32_t)xRadioInit.lBandwidth,
   "XTAL_frequency", xtal);
   */
}

/**
* @brief  To know if the reference deivider is enabled or disabled.
* @param  None.
* @retval SFunctionalState SET (enabled) or RESET (disabled).
*/
SFunctionalState MySubG_S2LPRadioGetRefDiv(void)
{
  uint8_t tmp;

  *(uint8_t*)&g_xStatus = MySubG_S2LPSpiReadRegisters(XO_RCO_CONF0_ADDR, 1, &tmp);
  My_printf(int2str(tmp));
  if(tmp & REFDIV_REGMASK)
  {
    return S_ENABLE;
  }
  else
  {
    return S_DISABLE;
  }
}

/**
* @brief  Set the digital divider .
* @param  xNewState S_DISABLE to disable the digital divider and S_ENABLE to enable it.
* @retval None.
*/

void MySubG_S2LPRadioSetDigDiv(SFunctionalState xNewState)
{
  uint8_t tmp;

  s_assert_param(IS_SFUNCTIONAL_STATE(xNewState));

  MySubG_S2LPSpiReadRegisters(XO_RCO_CONF1_ADDR, 1, &tmp);

  if(xNewState == S_ENABLE) {
    tmp &= ~PD_CLKDIV_REGMASK;
  } else {
    tmp |= PD_CLKDIV_REGMASK;
  }
  *(uint8_t*)&g_xStatus = MySubG_S2LPSpiWriteRegisters(XO_RCO_CONF1_ADDR, 1, &tmp);
}

/**
* @brief  Get the digital divider .
* @retval SFunctionalState S_DISABLE to disable the digital divider and S_ENABLE to enable it.
*/
SFunctionalState MySubG_S2LPRadioGetDigDiv(void)
{

	  My_printf("~~~~~ MySubG_S2LPRadioGetDigDiv");
  uint8_t tmp;

  *(uint8_t*)&g_xStatus = MySubG_S2LPSpiReadRegisters(XO_RCO_CONF1_ADDR, 1, &tmp);

  if(tmp & PD_CLKDIV_REGMASK) {
    return S_DISABLE;
  } else {
    return S_ENABLE;
  }
}

/**
* @brief  Computes the synth word from a given frequency.
* @param  frequency Target frequency value expressed in Hz.
* @param  refdiv reference divider value value (it can be only 1 or 2).
* @retval uint32_t SYNTH_WORD.
*/
uint32_t MySubG_S2LPRadioComputeSynthWord(uint32_t frequency, uint8_t refdiv)
{
  uint8_t band;

  if(IS_FREQUENCY_BAND_HIGH(frequency)) {
    band = HIGH_BAND_FACTOR;
  }
  else {
    band = MIDDLE_BAND_FACTOR;
  }

  uint64_t tgt1,tgt2,tgt;
  uint32_t synth;

  tgt = (((uint64_t)frequency)<<19)*(band*refdiv);
  synth=(uint32_t)(tgt/s_lXtalFrequency);
  tgt1 = (uint64_t)s_lXtalFrequency*(synth);
  tgt2 = (uint64_t)s_lXtalFrequency*(synth+1);

  synth=((tgt2-tgt)<(tgt-tgt1))?(synth+1):(synth);

  return synth;
}
/**
* @brief  Computes the ANALOG_IF and DIGITAL_IF register values staring from a image frequency
*          value in Hz.
* @param  nIF datarate expressed in bps. This parameter ranging between 100 and 500000.
* @param  pcAnaIf pointer to the register of analog IF.
* @param  pcDigIf pointer to the returned of digital IF.
* @retval None.
*/
void MySubG_S2LPRadioComputeIF(uint32_t nIF, uint8_t* pcAnaIf, uint8_t* pcDigIf)
{
  uint32_t f_dig=s_lXtalFrequency;

  if(f_dig>DIG_DOMAIN_XTAL_THRESH) {
    f_dig >>= 1;
  }

  (*pcAnaIf)=(uint8_t)((((uint64_t)nIF)<<13)*3/s_lXtalFrequency-100);
  (*pcDigIf)=(uint8_t)((((uint64_t)nIF)<<13)*3/f_dig-100);
}

/**
* @brief  Returns the charge pump word for a given VCO frequency.
* @param  cp_isel pointer to the charge pump register value.
* @param  pfd_split pointer to the pfd register value.
* @param  lFc channel center frequency expressed in Hz.
*         This parameter can be a value in one of the following ranges:<ul>
*         <li> High_Band: from 779 MHz to 915 MHz </li>
*         <li> Middle Band: from 387 MHz to 470 MHz </li>
* @param  refdiv reference divider value value (it can be only 1 or 2).
* @retval uint8_t Charge pump word.
*/
void MySubG_S2LPRadioSearchWCP(uint8_t* cp_isel, uint8_t* pfd_split, uint32_t lFc, uint8_t refdiv)
{
  uint32_t vcofreq, lFRef;
  uint8_t BFactor = MIDDLE_BAND_FACTOR;

  s_assert_param(IS_FREQUENCY_BAND(lFc));

  /* Search the operating band */
  if(IS_FREQUENCY_BAND_HIGH(lFc)) {
    BFactor = HIGH_BAND_FACTOR;
  }

  /* Calculates the VCO frequency VCOFreq = lFc*B */
  vcofreq = lFc*BFactor;

  /* Calculated the reference frequency clock */
  lFRef = s_lXtalFrequency/refdiv;

  /* Set the correct charge pump word */
  if(vcofreq>=VCO_CENTER_FREQ) {
    if(lFRef>DIG_DOMAIN_XTAL_THRESH) {
      *cp_isel = 0x02;
      *pfd_split = 0;
    }
    else {
      *cp_isel = 0x01;
      *pfd_split = 1;
    }
  }
  else {
    if(lFRef>DIG_DOMAIN_XTAL_THRESH) {
      *cp_isel = 0x03;
      *pfd_split = 0;
    }
    else {
      *cp_isel = 0x02;
      *pfd_split = 1;
    }
  }

}
/**
* @brief  Set the Synth word and the Band Select register according to desired base carrier frequency.
*         In this API the Xtal configuration is read out from
*         the corresponding register. The user shall fix it before call this API.
* @param  lFBase the base carrier frequency expressed in Hz as unsigned word.
* @retval Error code: 0=no error, 1=error during calibration of VCO.
*/

uint8_t MySubG_S2LPRadioSetFrequencyBase(uint32_t lFBase)
{
  uint32_t tmp32;
  uint8_t tmpBuffer[4], cp_isel, bs = 1, pfd_split, tmp, cRefDiv;

  s_assert_param(IS_FREQUENCY_BAND(lFBase));

  tmp32 = MySubG_S2LPRadioComputeSynthWord(lFBase, ((uint8_t)MySubG_S2LPRadioGetRefDiv()+1));

  if(IS_FREQUENCY_BAND_HIGH(lFBase)) {
    bs = 0;
  }

  cRefDiv = (uint8_t)MySubG_S2LPRadioGetRefDiv() + 1;

  /* Search the VCO charge pump word and set the corresponding register */
  MySubG_S2LPRadioSearchWCP(&cp_isel, &pfd_split, lFBase, cRefDiv);

  MySubG_S2LPSpiReadRegisters(SYNTH_CONFIG2_ADDR, 1, &tmp);
  tmp &= ~PLL_PFD_SPLIT_EN_REGMASK;
  tmp |= (pfd_split<<2);
  MySubG_S2LPSpiWriteRegisters(SYNTH_CONFIG2_ADDR, 1, &tmp);

  /* Build the array of registers values for the analog part */
  tmpBuffer[0] = (((uint8_t)(tmp32>>24)) & SYNT_27_24_REGMASK) | cp_isel<<5 | (bs<<4) ;
  tmpBuffer[1] = (uint8_t)(tmp32>>16);
  tmpBuffer[2] = (uint8_t)(tmp32>>8);
  tmpBuffer[3] = (uint8_t)tmp32;

  *(uint8_t*)&g_xStatus = MySubG_S2LPSpiWriteRegisters(SYNT3_ADDR, 4, tmpBuffer);

  return 0;
}

/**
 * @brief  Updates the gState (the global variable used to maintain memory of S2LP Status)
 *         reading the MC_STATE register of S2LP.
 * @param  None
 * @retval None
 */
void MySubG_S2LPRefreshStatus(void)
{
  uint8_t tempRegValue;
  /* Read the MC_STATE both from register and from SPI header and exit when they match.
      This will protect against possible transition state changes */
  do
  {
    /* Reads the MC_STATE register to update the g_xStatus */
    *(uint8_t*)&g_xStatus = MySubG_S2LPSpiReadRegisters(MC_STATE0_ADDR, 1, &tempRegValue);
  }
  while((tempRegValue>>1)!=g_xStatus.MC_STATE);
}



/**
* @brief  Returns the mantissa and exponent, whose value used in the datarate formula
*         will give the datarate value closer to the given datarate.
* @param  fDatarate datarate expressed in bps. This parameter ranging between 100 and 500000.
* @param  pcM pointer to the returned mantissa value.
* @param  pcE pointer to the returned exponent value.
* @retval None.
*/
uint32_t MySubG_S2LPRadioComputeDatarate(uint16_t cM, uint8_t cE)
{
  uint32_t f_dig=s_lXtalFrequency;
  uint64_t dr;

  if(f_dig>DIG_DOMAIN_XTAL_THRESH) {
    f_dig >>= 1;
  }

  if(cE==0) {
    dr=((uint64_t)f_dig*cM);
    return (uint32_t)(dr>>32);
  }

  dr=((uint64_t)f_dig)*((uint64_t)cM+65536);

  return (uint32_t)(dr>>(33-cE));
}

/**
* @brief  Returns the mantissa and exponent, whose value used in the datarate formula
*         will give the datarate value closer to the given datarate.
* @param  fDatarate datarate expressed in bps. This parameter ranging between 100 and 500000.
* @param  pcM pointer to the returned mantissa value.
* @param  pcE pointer to the returned exponent value.
* @retval None.
*/
uint32_t MySubG_S2LPRadioComputeFreqDeviation(uint8_t cM, uint8_t cE, uint8_t bs, uint8_t refdiv)
{
  uint32_t f_xo=s_lXtalFrequency;

  if(cE==0) {
    return (uint32_t)(((uint64_t)f_xo*cM)>>22);
  }

  return (uint32_t)(((uint64_t)f_xo*(256+cM))>>(23-cE));
}

/**
* @brief  Computes the channel filter value starting from mantissa and exponent.
* @param  cM mantissa value.
* @param  cE exponent value.
* @retval uint32_t the channel filter value in Hz.
*/
uint32_t MySubG_S2LPRadioComputeChannelFilterBw(uint8_t cM, uint8_t cE)
{
  uint32_t f_dig=s_lXtalFrequency;

  if(f_dig>DIG_DOMAIN_XTAL_THRESH) {
    f_dig >>= 1;
  }

  return (uint32_t)((uint64_t)100*s_vectnBandwidth26M[cM+(cE*9)]*f_dig/26000000);

}

/**
* @brief  Computes a frequency from a given SYNTH word.
* @param  lSynthWord the given SYNTH word.
* @param  bs value of the PLL divider.
* @param  refdiv reference divider value value (it can be only 1 or 2).
* @retval uint32_t the frequency base filter value in Hz.
*/
uint32_t MySubG_S2LPRadioComputeFrequencyBase(uint32_t lSynthWord, uint8_t bs, uint8_t refdiv)
{
  return (uint32_t)((((uint64_t)s_lXtalFrequency*lSynthWord)>>19)/bs/refdiv);
}

/**
* @brief  Returns the mantissa and exponent, whose value used in the datarate formula
*         will give the datarate value closer to the given datarate.
* @param  fDatarate datarate expressed in bps. This parameter ranging between 100 and 500000.
* @param  pcM pointer to the returned mantissa value.
* @param  pcE pointer to the returned exponent value.
* @retval None.
*/
void MySubG_S2LPRadioSearchDatarateME(uint32_t lDatarate, uint16_t* pcM, uint8_t* pcE)
{
  uint32_t lDatarateTmp, f_dig=s_lXtalFrequency;
  uint8_t uDrE;
  uint64_t tgt1,tgt2,tgt;

  if(f_dig>DIG_DOMAIN_XTAL_THRESH) {
    f_dig >>= 1;
  }

  /* Search the exponent value */
  for(uDrE = 0; uDrE != 12; uDrE++) {
    lDatarateTmp = MySubG_S2LPRadioComputeDatarate(0xFFFF, uDrE);
    if(lDatarate<=lDatarateTmp)
      break;
  }
  (*pcE) = (uint8_t)uDrE;

  if(uDrE==0) {
    tgt=((uint64_t)lDatarate)<<32;
    (*pcM) = (uint16_t)(tgt/f_dig);
    tgt1=(uint64_t)f_dig*(*pcM);
    tgt2=(uint64_t)f_dig*((*pcM)+1);
  }
  else {
    tgt=((uint64_t)lDatarate)<<(33-uDrE);
    (*pcM) = (uint16_t)((tgt/f_dig)-65536);
    tgt1=(uint64_t)f_dig*((*pcM)+65536);
    tgt2=(uint64_t)f_dig*((*pcM)+1+65536);
  }


  (*pcM)=((tgt2-tgt)<(tgt-tgt1))?((*pcM)+1):(*pcM);

}


/**
* @brief  Returns the mantissa and exponent, whose value used in the frequency deviation formula
*         will give a frequency deviation value most closer to the given frequency deviation.
* @param  fFDev frequency deviation expressed in Hz. This parameter can be a value in the range [F_Xo*8/2^18, F_Xo*7680/2^18].
* @param  pcM pointer to the returned mantissa value.
* @param  pcE pointer to the returned exponent value.
* @retval None.
*/
void MySubG_S2LPRadioSearchFreqDevME(uint32_t lFDev, uint8_t* pcM, uint8_t* pcE)
{
  uint8_t uFDevE, tmp, bs = MIDDLE_BAND_FACTOR, refdiv = 1;
  uint32_t lFDevTmp;
  uint64_t tgt1,tgt2,tgt;

  s_assert_param(IS_F_DEV(lFDev, s_lXtalFrequency));

  MySubG_S2LPSpiReadRegisters(SYNT3_ADDR, 1, &tmp);
  if((tmp&BS_REGMASK) == 0) {
    bs = HIGH_BAND_FACTOR;
  }

  if(MySubG_S2LPRadioGetRefDiv()) {
    refdiv = 2;
  }

  /* Search the exponent of the frequency deviation value */
  for(uFDevE = 0; uFDevE != 12; uFDevE++) {
    lFDevTmp = MySubG_S2LPRadioComputeFreqDeviation(255, uFDevE, bs, refdiv);
    if(lFDev<lFDevTmp)
      break;
  }
  (*pcE) = (uint8_t)uFDevE;

  if(uFDevE==0)
  {
    tgt=((uint64_t)lFDev)<<22;
    (*pcM)=(uint32_t)(tgt/s_lXtalFrequency);
    tgt1=(uint64_t)s_lXtalFrequency*(*pcM);
    tgt2=(uint64_t)s_lXtalFrequency*((*pcM)+1);
  }
  else
  {
    tgt=((uint64_t)lFDev)<<(23-uFDevE);
    (*pcM)=(uint32_t)(tgt/s_lXtalFrequency)-256;
    tgt1=(uint64_t)s_lXtalFrequency*((*pcM)+256);
    tgt2=(uint64_t)s_lXtalFrequency*((*pcM)+1+256);
  }

  (*pcM)=((tgt2-tgt)<(tgt-tgt1))?((*pcM)+1):(*pcM);
}


/**
* @brief  Returns the mantissa and exponent for a given bandwidth.
*         Even if it is possible to pass as parameter any value in the below mentioned range,
*         the API will search the closer value according to a fixed table of channel
*         bandwidth values (@ref s_vectnBandwidth), as defined in the datasheet, returning the corresponding mantissa
*         and exponent value.
* @param  lBandwidth bandwidth expressed in Hz. This parameter ranging between 1100 and 800100.
* @param  pcM pointer to the returned mantissa value.
* @param  pcE pointer to the returned exponent value.
* @retval None.
*/
void MySubG_S2LPRadioSearchChannelBwME(uint32_t lBandwidth, uint8_t* pcM, uint8_t* pcE)
{
  int8_t i, i_tmp;
  uint32_t f_dig=s_lXtalFrequency;
  int32_t chfltCalculation[3];


  if(f_dig>DIG_DOMAIN_XTAL_THRESH) {
    f_dig >>= 1;
  }

  s_assert_param(IS_CH_BW(lBandwidth,f_dig));

  /* Search the channel filter bandwidth table index */
  for(i=0;i<90 && (lBandwidth<(uint32_t)(((uint64_t)s_vectnBandwidth26M[i]*f_dig)/260000));i++);

  if(i!=0) {
    /* Finds the index value with best approximation in i-1, i and i+1 elements */
    i_tmp = i;

    for(uint8_t j=0;j<3;j++) {
      if(((i_tmp+j-1)>=0) && ((i_tmp+j-1)<=89)) {
        chfltCalculation[j] = (int32_t)lBandwidth - (int32_t)(((uint64_t)s_vectnBandwidth26M[i_tmp+j-1]*f_dig)/260000);
      }
      else {
        chfltCalculation[j] = 0x7FFFFFFF;
      }
    }
    uint32_t chfltDelta = 0xFFFFFFFF;

    for(uint8_t j=0;j<3;j++) {
      if(S_ABS(chfltCalculation[j])<chfltDelta) {
        chfltDelta = S_ABS(chfltCalculation[j]);
        i=i_tmp+j-1;
      }
    }
  }
  (*pcE) = (uint8_t)(i/9);
  (*pcM) = (uint8_t)(i%9);

}

/**
* @brief  Initializes the S2LP analog and digital radio part according to the specified
*         parameters in the pxSRadioInitStruct.
* @param  pxSRadioInitStruct pointer to a SRadioInit structure that
*         contains the configuration information for the analog radio part of S2LP.
* @retval Error code: 0=no error, 1=error during calibration of VCO.
*/
uint8_t MySubG_S2LPRadioInit(SRadioInit* pxSRadioInitStruct)
{
  uint8_t tmpBuffer[6], tmp8, dr_e, fdev_m, fdev_e, bw_m, bw_e;
  uint16_t dr_m;
  SFunctionalState xState;

  s_assert_param(IS_FREQUENCY_BAND(pxSRadioInitStruct->lFrequencyBase));
  s_assert_param(IS_MODULATION(pxSRadioInitStruct->xModulationSelect));
  s_assert_param(IS_DATARATE(pxSRadioInitStruct->lDatarate,s_lXtalFrequency));
  s_assert_param(IS_F_DEV(pxSRadioInitStruct->lFreqDev,s_lXtalFrequency));

  /* Configure the digital, ADC, SMPS reference clock divider */
  xState = MySubG_S2LPRadioGetDigDiv();


  if(((s_lXtalFrequency<DIG_DOMAIN_XTAL_THRESH) && (xState==S_ENABLE)) || ((s_lXtalFrequency>DIG_DOMAIN_XTAL_THRESH) && (xState==S_DISABLE)))
  {

	MySubG_S2LPSpiCommandStrobes(CMD_STANDBY);
    do{
      for(volatile uint8_t i=0; i!=0xFF; i++);
      MySubG_S2LPRefreshStatus();      // add a timer expiration callback
    }while(g_xStatus.MC_STATE!=MC_STATE_STANDBY);

    xState = (SFunctionalState)!xState;
    MySubG_S2LPRadioSetDigDiv(xState);

    MySubG_S2LPSpiCommandStrobes(CMD_READY);

    do{
      for(volatile uint8_t i=0; i!=0xFF; i++);
      MySubG_S2LPRefreshStatus();      // add a timer expiration callback
    }while(g_xStatus.MC_STATE!=MC_STATE_READY);
  }

  if(xState==S_ENABLE) {
    s_assert_param(IS_CH_BW(pxSRadioInitStruct->lBandwidth,(s_lXtalFrequency>>1)));
  }
  else {
    s_assert_param(IS_CH_BW(pxSRadioInitStruct->lBandwidth,s_lXtalFrequency));
  }

  /* Intermediate Frequency setting */
  MySubG_S2LPRadioComputeIF(300000, &tmpBuffer[0], &tmpBuffer[1]);
  MySubG_S2LPSpiWriteRegisters(IF_OFFSET_ANA_ADDR, 2, tmpBuffer);

  /* Calculates the datarate register values */
  MySubG_S2LPRadioSearchDatarateME(pxSRadioInitStruct->lDatarate, &dr_m, &dr_e);
  tmpBuffer[0] = (uint8_t)(dr_m>>8);
  tmpBuffer[1] = (uint8_t)dr_m;
  tmpBuffer[2] = (uint8_t)(pxSRadioInitStruct->xModulationSelect | dr_e);

  /* Calculates the frequency deviation register values */
  MySubG_S2LPRadioSearchFreqDevME(pxSRadioInitStruct->lFreqDev, &fdev_m, &fdev_e);
  MySubG_S2LPSpiReadRegisters(MOD1_ADDR, 1, &tmpBuffer[3]);
  tmpBuffer[3] &= ~FDEV_E_REGMASK;
  tmpBuffer[3] |= fdev_e;
  tmpBuffer[4] = fdev_m;

  /* Calculates the channel filter register values */
  MySubG_S2LPRadioSearchChannelBwME(pxSRadioInitStruct->lBandwidth, &bw_m, &bw_e);
  tmpBuffer[5] = (bw_m<<4) | bw_e;

  /* Configures the radio registers */
  MySubG_S2LPSpiWriteRegisters(MOD4_ADDR, 6, tmpBuffer);

  MySubG_S2LPSpiReadRegisters(PA_POWER0_ADDR, 3, &tmpBuffer[0]);

  /* if OOK is selected enable the PA_FC else enable it */
  if((pxSRadioInitStruct->xModulationSelect)!=MOD_ASK_OOK)
  {
    tmpBuffer[0] &= 0x7F;
    tmpBuffer[1] &= 0xFD;
  }
  else
  {
    tmpBuffer[0] |= 0x80;
    tmpBuffer[1] |= 0x02;
  }

  tmpBuffer[2]&=0xFC;

  /* Bessel filter config */
  if(pxSRadioInitStruct->lDatarate<16000)
  {
    tmpBuffer[2]|=0x00;
  }
  else if(pxSRadioInitStruct->lDatarate<32000)
  {
    tmpBuffer[2]|=0x01;
  }
  else if(pxSRadioInitStruct->lDatarate<62500)
  {
    tmpBuffer[2]|=0x02;
  }
  else
  {
    tmpBuffer[2]|=0x03;
  }

  MySubG_S2LPSpiWriteRegisters(PA_POWER0_ADDR, 3, &tmpBuffer[0]);

  /* Enable the freeze option of the AFC on the SYNC word */
  MySubG_S2LPSpiReadRegisters(AFC2_ADDR, 1, &tmp8);
  tmp8 |= AFC_FREEZE_ON_SYNC_REGMASK; MySubG_S2LPSpiWriteRegisters(AFC2_ADDR, 1, &tmp8);

  return MySubG_S2LPRadioSetFrequencyBase(pxSRadioInitStruct->lFrequencyBase);

}

/**
* @brief  Returns the S2LP analog and digital radio structure according to the registers value.
* @param  pxSRadioInitStruct pointer to a SRadioInit structure that
*         contains the configuration information for the analog radio part of S2LP.
* @retval None.
*/
void MySubG_S2LPRadioGetInfo(SRadioInit* pxSRadioInitStruct)
{
  uint8_t tmpBuffer[6];
  uint8_t band, cRefDiv, dr_e, fdev_m, fdev_e, bw_e, bw_m;
  uint16_t dr_m;
  uint32_t tmp32;

  MySubG_S2LPSpiReadRegisters(SYNT3_ADDR, 4, tmpBuffer);
  /* Reads the operating band masking the Band selected field */
  if(tmpBuffer[0] & BS_REGMASK) {
    band = MIDDLE_BAND_FACTOR;
  }
  else {
    band = HIGH_BAND_FACTOR;
  }

  /* Computes the synth word */
  tmp32 = (((uint32_t)(tmpBuffer[0] & SYNT_27_24_REGMASK))<<24) | (((uint32_t)tmpBuffer[1])<<16) | (((uint32_t)tmpBuffer[2])<<8) | ((uint32_t)tmpBuffer[3]);

  /* Calculates the frequency base */
  cRefDiv = (uint8_t)MySubG_S2LPRadioGetRefDiv() + 1;

  pxSRadioInitStruct->lFrequencyBase = MySubG_S2LPRadioComputeFrequencyBase(tmp32, band, cRefDiv);

  /* Reads the radio registers */
  *(uint8_t*)&g_xStatus = MySubG_S2LPSpiReadRegisters(MOD4_ADDR, 6, tmpBuffer);

  /* Modulation select */
  pxSRadioInitStruct->xModulationSelect = (ModulationSelect)(tmpBuffer[2] & MOD_TYPE_REGMASK);

  /* Reads the frequency deviation for mantissa and exponent */
  fdev_m = tmpBuffer[4];
  fdev_e = tmpBuffer[3] & FDEV_E_REGMASK;

  /* Reads the channel filter register for mantissa and exponent */
  bw_m = (tmpBuffer[5] & CHFLT_M_REGMASK)>>4;
  bw_e = tmpBuffer[5] & CHFLT_E_REGMASK;

  /* Reads the data rate register for mantissa and exponent */
  dr_m = ((uint16_t)tmpBuffer[0]<<8) | ((uint16_t)tmpBuffer[1]);
  dr_e = tmpBuffer[2] & DATARATE_E_REGMASK;

  /* Calculates the datarate */
  pxSRadioInitStruct->lDatarate = MySubG_S2LPRadioComputeDatarate(dr_m, dr_e);

  /* Calculates the frequency deviation */
  pxSRadioInitStruct->lFreqDev = MySubG_S2LPRadioComputeFreqDeviation(fdev_m, fdev_e, band, cRefDiv);

  /* Reads the channel filter bandwidth from the look-up table and return it */
  pxSRadioInitStruct->lBandwidth = MySubG_S2LPRadioComputeChannelFilterBw(bw_m, bw_e);

}

void MySubG_S2LPShutdownEnter(void)
{
  /* Puts high the GPIO connected to shutdown pin */
  MyGPIO_WritePin(S2LP_SDN_Port, S2LP_SDN_Pin, GPIO_PIN_SET);
}

void MySubG_S2LPShutdownExit(void)
{
  /* Puts low the GPIO connected to shutdown pin */
	MyGPIO_WritePin(S2LP_SDN_Port, S2LP_SDN_Pin, GPIO_PIN_RESET);
  /* Delay to allow the circuit POR, about 700 us */
  for(volatile uint32_t i=0;i<0x1E00;i++);
}

void MySubG_S2LPRest()
{
	MySubG_S2LPShutdownEnter();
	My_Delay(10);
	MySubG_S2LPShutdownExit();
	My_Delay(10);
}

void MySubG_TestOnly()
{

}

