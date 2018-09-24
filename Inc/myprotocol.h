/**
 *	\file
 *	\author	<a href="http://www.innomatic.ca">innomatic</a>
 * 	\copyright <a rel="license" href="http://creativecommons.org/licenses/by-nc/4.0/"><img alt="Creative Commons Licence" style="border-width:0" src="https://i.creativecommons.org/l/by-nc/4.0/88x31.png" /></a><br />This work is licensed under a <a rel="license" href="http://creativecommons.org/licenses/by-nc/4.0/">Creative Commons Attribution-NonCommercial 4.0 International License</a>.
 */
#ifndef __MY_PROTOCOL_H
#define __MY_PROTOCOL_H

// System control
#define SYS_SRESET			0x00    ///< perform system reset
#define SYS_WRESET			0x01	///< perform watchdog reset
#define SYS_STPMOD			0x02	///< enter stop mode
#define SYS_DSLMOD			0x03	///< enter deep sleep mode
#define SYS_SLPMOD			0x04	///< enter sleep mode

// DIO control
#define DIO_SETVAL			0x10	///< DIO set value
#define DIO_RSTVAL			0x11	///< DIO reset value
#define DIO_GETVAL			0x12	///< DIO get value
#define DIO_TGLVAL			0x13	///< DIO toggle value

// ADC control
#define ADC_SETMAG			0x20	///< ADC set magnitude
#define ADC_SETFRQ			0x21	///< ADC set frequency
#define ADC_CONSTV			0x23	///< ADC dc output
#define ADC_SINEWV			0x24	///< ADC sine waveform
#define ADC_SWTHWV			0x25	///< ADC sawtooth wavefrom
#define ADC_TRNGWV			0x26	///< ADC triangle waveform

// DAC control
#define DAC_SETFRQ			0x30	///< DAC set sample frequency
#define DAC_CAPSGL			0x31	///< DAC capture single
#define DAC_CAPCNT			0x32	///< DAC capture continuous

// TSC control
#define TSC_RPTVAL			0x40	///< TSC start reporting

// Report data
#define RPT_U08X01			0x81    ///< report with uint8_t data
#define RPT_S08X01			0x82    ///< report with int8_t data
#define RPT_U16X01			0x83    ///< report with uint16_t data
#define RPT_S16X01			0x84    ///< report with int16_t data
#define RPT_U32X01			0x85    ///< report with uint16_t data
#define RPT_S32X01			0x86    ///< report with int16_t data


#endif //__MY_PROTOCOL_H
