/*
 * MyDev_SubG.h
 *
 *  Created on: Aug 6, 2020
 *      Author: SeanChiu
 */

#ifndef INC_MYDEV_SUBG_H_
#define INC_MYDEV_SUBG_H_

#include "My_Config.h"

/**
 * \brief SYNT3 register
 * \code
 * Read and Write
 * Default value: 0x42
 * 7:5 PLL_CP_ISEL: Set the charge pump current according to the XTAL frequency.
 * 4 BS: Synthesizer band select. This parameter selects the out-of loop divide factor of the synthesizer:,  0: 4, band select factor for high band,  1: 8, band select factor for middle band.
 * 3:0 SYNT_27_24: MSB bits of the PLL programmable divider.
 * \endcode
 */
#define SYNT3_ADDR			((uint8_t)0x05)
#define PLL_CP_ISEL_REGMASK			((uint8_t)0xE0)
#define BS_REGMASK			((uint8_t)0x10)
#define SYNT_27_24_REGMASK			((uint8_t)0x0F)

/** @defgroup Configuration_Exported_Constants  Configuration Exported Constants
 * @{
 */
#define DIG_DOMAIN_XTAL_THRESH  30000000        /*!< Digital domain logic threshold for XTAL in MHz */

/**
 * \brief IF_OFFSET_ANA register
 * \code
 * Read and Write
 * Default value: 0x2A
 * 7:0 IF_OFFSET_ANA: Intermediate frequency setting for the analog RF synthesizer, default: 300 kHz.
 * \endcode
 */
#define IF_OFFSET_ANA_ADDR			((uint8_t)0x09)
#define IF_OFFSET_ANA_REGMASK			((uint8_t)0xFF)

/**
 * \brief MOD4 register
 * \code
 * Read and Write
 * Default value: 0x83
 * 7:0 DATARATE_M[15:8]: The MSB of the mantissa value of the data rate equation, default 38.4 ksps.
 * \endcode
 */
#define MOD4_ADDR			((uint8_t)0x0E)
#define DATARATE_M_15_8_REGMASK			((uint8_t)0xFF)

/**
 * \brief MOD2 register
 * \code
 * Read and Write
 * Default value: 0x77
 * 7:4 MOD_TYPE: Modulation type:,  0: 2-FSK,  1: 4-FSK,  2: 2-GFSK BT=1,  3: 4-GFSK BT=1,  5: ASK/OOK,  7: unmodulated,  10: 2-GFSK BT=0.5,  12: 4-GFSK BT=0.5
 * 3:0 DATARATE_E: The exponent value of the data rate equation, default 38.4 ksps.
 * \endcode
 */
#define MOD2_ADDR			((uint8_t)0x10)
#define MOD_TYPE_REGMASK			((uint8_t)0xF0)
#define DATARATE_E_REGMASK			((uint8_t)0x0F)

/**
 * \brief MOD1 register
 * \code
 * Read and Write
 * Default value: 0x03
 * 7 PA_INTERP_EN: 1: enable the power interpolator
 * 6 MOD_INTERP_EN: 1: enable frequency interpolator
 * 5:4 G4FSK_CONST_MAP: Select the constellation map for 4-GFSK.
 * 3:0 FDEV_E: The exponent value of the frequency deviation equation, default 20 kHz.
 * \endcode
 */
#define MOD1_ADDR			((uint8_t)0x11)
#define PA_INTERP_EN_REGMASK			((uint8_t)0x80)
#define MOD_INTERP_EN_REGMASK			((uint8_t)0x40)
#define G4FSK_CONST_MAP_REGMASK			((uint8_t)0x30)
#define FDEV_E_REGMASK			((uint8_t)0x0F)

/**
 * \brief CHFLT register
 * \code
 * Read and Write
 * Default value: 0x23
 * 7:4 CHFLT_M: The mantissa value of the receiver channel filter, default 100 kHz.
 * 3:0 CHFLT_E: The exponent value of the receiver channel filter, default 100 kHz.
 * \endcode
 */
#define CHFLT_ADDR			((uint8_t)0x13)
#define CHFLT_M_REGMASK			((uint8_t)0xF0)
#define CHFLT_E_REGMASK			((uint8_t)0x0F)

/**
 * \brief AFC2 register
 * \code
 * Read and Write
 * Default value: 0xC8
 * 7 AFC_FREEZE_ON_SYNC: 1: enable the freeze AFC correction upon sync word detection.
 * 6 AFC_ENABLED: 1: enable the AFC correction.
 * 5 AFC_MODE: Select AFC mode:,  0: AFC loop closed on slicer,  1: AFC loop closed on second conversion stage.
 * 4:0 RESERVED: -
 * \endcode
 */
#define AFC2_ADDR			((uint8_t)0x14)
#define AFC_FREEZE_ON_SYNC_REGMASK			((uint8_t)0x80)
#define AFC_ENABLED_REGMASK			((uint8_t)0x40)
#define AFC_MODE_REGMASK			((uint8_t)0x20)

/**
 * \brief MOD4 register
 * \code
 * Read and Write
 * Default value: 0x83
 * 7:0 DATARATE_M[15:8]: The MSB of the mantissa value of the data rate equation, default 38.4 ksps.
 * \endcode
 */
#define MOD4_ADDR			((uint8_t)0x0E)
#define DATARATE_M_15_8_REGMASK			((uint8_t)0xFF)


/**
 * \brief PA_POWER0 register
 * \code
 * Read and Write
 * Default value: 0x47
 * 7 DIG_SMOOTH_EN: 1: enable the generation of the internal signal TX_DATA which is the input of the FIR.
 * 6 PA_MAXDBM: 1: configure the PA to send maximum output power.
 * 5 PA_RAMP_EN: 1: enable the power ramping
 * 4:3 PA_RAMP_STEP_LEN: Set the step width (unit: 1/8 of bit period).
 * 2:0 PA_LEVEL_MAX_IDX: Final level for power ramping or selected output power index.
 * \endcode
 */
#define PA_POWER0_ADDR			((uint8_t)0x62)
#define DIG_SMOOTH_EN_REGMASK			((uint8_t)0x80)
#define PA_MAXDBM_REGMASK			((uint8_t)0x40)
#define PA_RAMP_EN_REGMASK			((uint8_t)0x20)
#define PA_RAMP_STEP_LEN_REGMASK			((uint8_t)0x18)
#define PA_LEVEL_MAX_IDX_REGMASK			((uint8_t)0x07)

/**
 * \brief SYNTH_CONFIG2 register
 * \code
 * Read and Write
 * Default value: 0xD0
 * 7 RESERVED: -
 * 6 RESERVED: -
 * 5 RESERVED: -
 * 4:3 RESERVED: -
 * 2 PLL_PFD_SPLIT_EN: Enables increased DN current pulses to improve linearization of CP/PFD.
 * 1:0 RESERVED: -
 * \endcode
 */
#define SYNTH_CONFIG2_ADDR			((uint8_t)0x65)
#define PLL_PFD_SPLIT_EN_REGMASK			((uint8_t)0x04)

/**
 * \brief XO_RCO_CONF1 register
 * \code
 * Read and Write
 * Default value: 0x6C
 * 7:6 RESERVED: -
 * 5 RESERVED: -
 * 4 PD_CLKDIV: 1: disable both dividers of digital clock (and reference clockfor the SMPS) and IF-ADC clock.
 * 3:2 RESERVED: -
 * 1:0 RESERVED: -
 * \endcode
 */
#define XO_RCO_CONF1_ADDR			((uint8_t)0x6C)
#define PD_CLKDIV_REGMASK			((uint8_t)0x10)

/**
 * \brief XO_RCO_CONF0 register
 * \code
 * Read and Write
 * Default value: 0x30
 * 7 EXT_REF: 0: reference signal from XO circuit, 1: reference signal from XIN pin.
 * 5:4 GM_CONF: Set the driver gm of the XO at start up.
 * 3 REFDIV: 1: enable the the reference clock divider.
 * 2 RESERVED: -
 * 1 EXT_RCO_OSC: 1: the 34.7 kHz signal must be supplied from any GPIO.
 * 0 RCO_CALIBRATION: 1: enable the automatic RCO calibration.
 * \endcode
 */
#define XO_RCO_CONF0_ADDR			((uint8_t)0x6D)
#define EXT_REF_REGMASK			((uint8_t)0x80)
#define GM_CONF_REGMASK			((uint8_t)0x70)
#define REFDIV_REGMASK			((uint8_t)0x08)
#define EXT_RCO_OSC_REGMASK			((uint8_t)0x02)
#define RCO_CALIBRATION_REGMASK			((uint8_t)0x01)


/**
 * \brief MC_STATE0 register
 * \code
 * Read only
 * Default value: 0x07
 * 7:1 STATE: Current state.
 * 0 XO_ON: 1: XO is operating.
 * \endcode
 */
#define MC_STATE0_ADDR			((uint8_t)0x8E)
#define STATE_REGMASK			((uint8_t)0xFE)
#define XO_ON_REGMASK			((uint8_t)0x01)

/**
 * @brief  S2LP Functional state. Used to enable or disable a specific option.
 */
typedef enum {
  S_DISABLE = 0,
  S_ENABLE = !S_DISABLE
} SFunctionalState;

/**
 * @brief  S2LP Modulation enumeration
 */
typedef enum {
  MOD_NO_MOD       = 0x70, /*!< CW modulation selected */
  MOD_2FSK         = 0x00, /*!< 2-FSK modulation selected */
  MOD_4FSK         = 0x10, /*!< 4-FSK modulation selected */
  MOD_2GFSK_BT05   = 0xA0, /*!< G2FSK modulation selected with BT = 0.5 */
  MOD_2GFSK_BT1    = 0x20, /*!< G2FSK modulation selected with BT = 1 */
  MOD_4GFSK_BT05   = 0xB0, /*!< G4FSK modulation selected with BT = 0.5 */
  MOD_4GFSK_BT1    = 0x30, /*!< G4FSK modulation selected with BT = 1 */
  MOD_ASK_OOK      = 0x50, /*!< OOK modulation selected. */
  MOD_POLAR        = 0x60, /*!< OOK modulation selected. */
} ModulationSelect;

/**
  * @brief  S2LP Commands codes enumeration
  */
typedef enum
{
  CMD_TX =  ((uint8_t)(0x60)),                    /*!< Start to transmit; valid only from READY */
  CMD_RX =  ((uint8_t)(0x61)),                    /*!< Start to receive; valid only from READY */
  CMD_READY =  ((uint8_t)(0x62)),                 /*!< Go to READY; valid only from STANDBY or SLEEP or LOCK */
  CMD_STANDBY =  ((uint8_t)(0x63)),               /*!< Go to STANDBY; valid only from READY */
  CMD_SLEEP = ((uint8_t)(0x64)),                  /*!< Go to SLEEP; valid only from READY */
  CMD_LOCKRX = ((uint8_t)(0x65)),                 /*!< Go to LOCK state by using the RX configuration of the synth; valid only from READY */
  CMD_LOCKTX = ((uint8_t)(0x66)),                 /*!< Go to LOCK state by using the TX configuration of the synth; valid only from READY */
  CMD_SABORT = ((uint8_t)(0x67)),                 /*!< Force exit form TX or RX states and go to READY state; valid only from TX or RX */
  CMD_LDC_RELOAD = ((uint8_t)(0x68)),             /*!< LDC Mode: Reload the LDC timer with the value stored in the  LDC_PRESCALER / COUNTER  registers; valid from all states  */
  CMD_RCO_CALIB =  ((uint8_t)(0x69)),             /*!< Start (or re-start) the RCO calibration */
  CMD_SRES = ((uint8_t)(0x70)),                   /*!< Reset of all digital part, except SPI registers */
  CMD_FLUSHRXFIFO = ((uint8_t)(0x71)),            /*!< Clean the RX FIFO; valid from all states */
  CMD_FLUSHTXFIFO = ((uint8_t)(0x72)),            /*!< Clean the TX FIFO; valid from all states */
  CMD_SEQUENCE_UPDATE =  ((uint8_t)(0x73)),       /*!< Autoretransmission: Reload the Packet sequence counter with the value stored in the PROTOCOL[2] register valid from all states */
} S2LPCmd;

/**
 * @brief  S2LP Radio Init structure definition
 */
typedef struct {
  uint32_t          lFrequencyBase;     /*!< Specifies the base carrier frequency (in Hz),
                                        i.e. the carrier frequency of channel #0.
                                        This parameter can be in one of the following ranges:
                                        High_Band: from 860 MHz to 940 MHz
                                        Middle Band: from 430 MHz to 470 MHz */
  ModulationSelect  xModulationSelect;  /*!< Specifies the modulation @ref ModulationSelect */
  uint32_t          lDatarate;          /*!< Specifies the datarate expressed in bps.
                                        This parameter can be in the range
                                        between 100 bps and 500 kbps */
  uint32_t          lFreqDev;           /*!< Specifies the frequency deviation expressed in Hz.
                                        This parameter can be in the range:
                                        [F_Xo*8/2^18, F_Xo*7680/2^18] Hz */
  uint32_t          lBandwidth;         /*!< Specifies the channel filter bandwidth
                                        expressed in Hz. This parameter can be
                                        in the range between 1100 and 800100 Hz */
} SRadioInit;

/**
 * @brief  S2LP States enumeration.
 */
typedef enum {
  MC_STATE_READY             =0x00,  /*!< READY */
  MC_STATE_SLEEP_NOFIFO      =0x01,  /*!< SLEEP NO FIFO RETENTION */
  MC_STATE_STANDBY           =0x02,  /*!< STANDBY */
  MC_STATE_SLEEP             =0x03,  /*!< SLEEP */
  MC_STATE_LOCKON            =0x0C,  /*!< LOCKON */
  MC_STATE_RX                =0x30,  /*!< RX */
  MC_STATE_LOCK_ST           =0x14,  /*!< LOCK_ST */
  MC_STATE_TX                =0x5C,  /*!< TX */
  MC_STATE_SYNTH_SETUP       =0x50   /*!< SYNTH_SETUP */
} S2LPState;

/**
 * @brief S2LP Status. This definition represents the single field of the S2LP
 *        status returned on each SPI transaction, equal also to the MC_STATE registers.
 *        This field-oriented structure allows user to address in simple way the single
 *        field of the S2LP status.
 *        The user shall define a variable of S2LPStatus type to access on S2LP status fields.
 * @note  The fields order in the structure depends on used endianness (little or big
 *        endian). The actual definition is valid ONLY for LITTLE ENDIAN mode. Be sure to
 *        change opportunely the fields order when use a different endianness.
 */
typedef struct {
  uint8_t XO_ON:1;           /*!< XO is operating state */
  S2LPState MC_STATE: 7;     /*!< The state of the Main Controller of S2LP @ref S2LPState */
  uint8_t ERROR_LOCK: 1;     /*!< RCO calibration error */
  uint8_t RX_FIFO_EMPTY: 1;  /*!< RX FIFO is empty */
  uint8_t TX_FIFO_FULL: 1;   /*!< TX FIFO is full */
  uint8_t ANT_SELECT: 1;     /*!< Currently selected antenna */
  uint8_t RCCAL_OK: 1;       /*!< RCO successfully terminated */
  uint8_t : 3;               /*!< This 3 bits field are reserved and equal to 2 */
}S2LPStatus;

#ifdef  S2LP_USE_FULL_ASSERT
 /**
   * @brief  The s_assert_param macro is used for function's parameters check.
   * @param  expr If expr is false, it calls assert_failed function which reports
   *         the name of the source file and the source line number of the call
   *         that failed. If expr is true, it returns no value.
   * @retval None
   */
  #define s_assert_param(expr) ((expr) ? (void)0 : s_assert_failed((uint8_t *)__FILE__, __LINE__))
  void s_assert_failed(uint8_t* file, uint32_t line);
#elif  S2LP_USE_VCOM_ASSERT
  /**
   * @brief  The s_assert_param macro is used for function's parameters check.
   * @param  expr  If expr is false, it calls assert_failed function which reports
   *         the name of the source file and the source line number of the call
   *         that failed. If expr is true, it returns no value.
   * @retval None
   */
  #define s_assert_param(expr) ((expr) ? (void)0 : s_assert_failed((uint8_t *)__FILE__, __LINE__,#expr))
  void s_assert_failed(uint8_t* file, uint32_t line, char* expression);
#else
  #define s_assert_param(expr)        {}
#endif

#define S2LP_COMMAND_TABLE \
{ "S2LPRadioInit",      MySubG_S2LPRadioInitAction,    "wuwwww", "\r\nuint32 frequency base [Hz]\r\nbyte modulation\r\nuint32 data rate [bps]\r\nuint32 frequency deviation [Hz]\r\nuint32 channel filter bandwidth [Hz]\r\nuint32 XTAL frequency in Hz, 0 means auto [Hz]" },  \
{ "S2LPRadioGetInfo",   MySubG_S2LPRadioGetInfoAction, "", "" }/*, \
{ "S2LPRadioGetXtalFrequency",   S2LPRadioGetXtalFrequencyAction, "", "\r\nGet the XTAL frequency [Hz]" }, \
{ "S2LPRadioSetFrequencyBase",   S2LPRadioSetFrequencyBaseAction, "w", "\r\nuint32 Frequency base [Hz]" }, \
{ "S2LPRadioGetFrequencyBase",   S2LPRadioGetFrequencyBaseAction, "", "" }, \
{ "S2LPRadioSetModulation",   S2LPRadioSetModulationAction, "u", "\r\nuint8 Modulation code" }, \
{ "S2LPRadioGetModulation",   S2LPRadioGetModulationAction, "", "" }, \
{ "S2LPRadioSetPALeveldBm",   S2LPRadioSetPALeveldBmAction, "wu", "\r\nfloatoutput power in dBm\r\nbyte pa level max index used" }, \
{ "S2LPRadioGetPALeveldBm",   S2LPRadioGetPALeveldBmAction, "u", "\r\npa level index" }, \
{ "S2LPRadioSetPALevelNdBm", S2LPRadioSetPALevelNdBmAction, "wu", "\r\nfloat power in dBm\r\nbyte pa level index used for ramping"}, \
{ "S2LPRadioSetPALevelMaxInd", S2LPRadioSetPALevelMaxIndAction, "u", "to set the max index for PA"}, \
{ "S2LPGetPktFrmt",   S2LPGetPktFrmtAction, "", "" }, \
{ "S2LPPktBasicInit",   S2LPPktBasicInitAction, "vuwuuuuuu", "\r\nuint16 preamble length\r\nbyte sync length\r\nuint32 sync word\r\n - 0 no length field\r\n - 0 1 byte\r\n - 1 2 bytes\r\n(1/0) address field enable\r\nbyte CRC mode\r\n(1/0) FEC enable\r\n(1/0) whitening enable" }, \
{ "S2LPPktBasicGetInfo", S2LPPktBasicGetInfoAction, "", "" }, \
{ "S2LPPktStackInit",   S2LPPktStackInitAction, "vuwuuuuu", "\r\nuint16 preamble length\r\nbyte sync length\r\nuint32 sync word\r\n - 0 no length field\r\n - 0 1 byte\r\n - 1 2 bytes\r\nbyte CRC mode\r\n(1/0) FEC enable\r\n(1/0) whitening enable" }, \
{ "S2LPPktStackGetInfo", S2LPPktStackGetInfoAction, "", "" }, \
{ "S2LPPktBasicSetPayloadLength", S2LPPktBasicSetPayloadLengthAction, "v", "\r\nuint16 byte preamble length" }, \
{ "S2LPPktBasicGetPayloadLength", S2LPPktBasicGetPayloadLengthAction, "", "" }, \
{ "S2LPPktStackSetPayloadLength", S2LPPktStackSetPayloadLengthAction, "v", "\r\nuint16 byte preamble length" }, \
{ "S2LPPktStackGetPayloadLength", S2LPPktStackGetPayloadLengthAction, "", "" }, \
{ "S2LPPktMbusSetPayloadLength",    S2LPPktMbusSetPayloadLengthAction, "v", "payload length" }, \
{ "S2LPPktMbusGetPayloadLength",    S2LPPktMbusGetPayloadLengthAction, "", "" }, \
{ "S2LPPktMbusInit",    S2LPPktMbusInitAction, "uuu", "\r\nbyte WMBUS mode :\r\n - 0 MBUS_SUBMODE_S1_S2_LONG_HEADER\r\n - 1 MBUS_SUBMODE_S1_M_S2_T2_OTHER_TO_METER\r\n - 3 MBUS_SUBMODE_T1_T2_METER_TO_OTHER\r\n - 5 MBUS_SUBMODE_R2_SHORT_HEADER\r\nbyte preamble to add to the standard default preamble\r\nbyte postamble to add to the standard default postamble" }, \
{ "S2LPPktMbusGetInfo", S2LPPktMbusGetInfoAction, "", "" }, \
{ "S2LPTimerSetRxTimeoutUs", S2LPTimerSetRxTimeoutUsAction, "w", "float rx timeout in us" }, \
{ "S2LPTimerGetRxTimeout",   S2LPTimerGetRxTimeoutAction, "", "RX timeout in us" }, \
{ "S2LPGpioInit",       S2LPGpioInitAction, "uuu", "\r\ngpio pin (0,1,2,3)\r\ngpio mode:\r\n - 0 input\r\n - 1 output lp\r\n - 2 output hp\r\nsignal (code)" }, \
{ "S2LPIrq",       S2LPIrqAction, "wu", "\r\nuint32_t irq code\r\n(1/0) enable/disable" }, \
{ "S2LPIrqGetStatus",   S2LPIrqGetStatusAction, "", "" }, \
{ "S2LPQiGetRssidBm",   S2LPQiGetRssidBmAction, "", "" }, \
{ "S2LPGetVersion",   S2LPGetVersionAction, "", "" }, \
{ "S2LPWhitening",    S2LPWhiteningAction, "u", "\r\nTo enable or disable whitening"}, \
{ "S2LPDirectRfSetTxMode",S2LPDirectRfSetTxModeAction, "u","\r\nSet the direct TX mode"}, \
{ "S2LPDirectRfSetRxMode",S2LPDirectRfSetRxModeAction, "u","\r\nSet the direct RX mode"}, \
{ "S2LPLinearFifoSetAlmostFullThresholdRx", S2LPLinearFifoSetAlmostFullThresholdRxAction, "u", "to set the Rx FIFO Thr"}, \
{ "S2LPLinearFifoSetAlmostEmptyThresholdTx", S2LPLinearFifoSetAlmostEmptyThresholdTxAction, "u", "to set the Tx FIFO Thr"}, \
{ "S2LPTimerSetWakeUpTimerUs", S2LPTimerSetWakeUpTimerUsAction, "w", "Wakeup timer timeout in us" }, \
{ "S2LPTimerLdcrMode",  S2LPTimerLdcrModeAction, "uu", "To set the LDC mode"}, \
{ "S2LPCalibrateRco",  S2LPCalibrateRcoAction, "", "Offline calibration of the RCO"}, \
{ "S2LPGetNBytesReportAll", S2LPGetNBytesRepAllAction, "u", "Decides if the getNBytes has to read addresses, seq num etc."}, \
{ "S2LPGetNBytes", S2LPGetNBytesHandler, "v", "this handler is to get N bytes in reception"}, \
{ "S2LPGetNBytesBatch", S2LPGetNBytesBatchHandler, "ww", "this handler is to get N bytes in reception"}, \
{ "S2LPSendPattern", S2LPSendPatternHandler, "b", "this handler is to sent a pattern sequence"}, \
{ "S2LPSendPatternRaw", S2LPSendPatternRawHandler, "", "this handler is to sent a pattern sequence"}, \
{ "S2LPSendNBytes", S2LPSendNBytesHandler, "b", "this handler is to sent a specified bytes sequence"}, \
{ "S2LPSendNBytesBatch", S2LPSendNBytesBatchHandler, "wwb", "this handler is to sent a specified bytes sequence every a specified num of ms"}, \
{ "S2LPGetRssiRunBatch", S2LPGetRssiRunBatchAction, "w", "RSSI run"}, \
{ "S2LPSendPatternGpio", S2LPSendPatternHandlerGpio, "w", "this handler is to sent a pattern sequence through the GPIO pin"}, \
{ "SdkEvalIrqHandler",     IrqHandlerAction, "u", "1 get IRQ in blocking mode" }, \
{ "S2LPGetLibVersion",   S2LPGetLibVersionAction, "", "" }, \
{ "S2LPSendBatchLP",   S2LPSendBatchLPAction, "u", "Enables the low power transmission in batch" }, \
{ "S2LPGetBatchLP",   S2LPGetBatchLPAction, "u", "Enables the low power reception in batch" }, \
{ "S2LPGetRcoFrequency",  S2LPGetRcoFrequencyAction, "", "Returns the RCO frequency in Hz"}
*/
 void MySubG_S2LPRadioInitAction(void);
 void MySubG_S2LPRadioGetInfoAction(void);
uint8_t MySubG_S2LPRadioInit(SRadioInit* pxSRadioInitStruct);
uint16_t MySubG_S2LPSpiReadRegisters(uint8_t address, uint8_t n_bytes, uint8_t* buffer);
SFunctionalState MySubG_S2LPRadioGetRefDiv(void);
void MySubG_S2LPRadioGetInfo(SRadioInit* pxSRadioInitStruct);
void MySubG_S2LPRest();
void MySubG_TestOnly();
#endif /* INC_MYDEV_SUBG_H_ */
