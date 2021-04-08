/*****************************************************************//**
 * @file main_sampler_test.cpp
 *
 * @brief Basic test of nexys4 ddr mmio cores
 *
 * @author p chu
 * @version v1.0: initial release
 *********************************************************************/

// #define _DEBUG
#include "chu_init.h"
#include "gpio_cores.h"
#include "xadc_core.h"
#include "sseg_core.h"
#include "spi_core.h"
#include "i2c_core.h"
#include "ps2_core.h"
#include "ddfs_core.h"
#include "adsr_core.h"


/**
 * read FPGA internal voltage temperature
 * @param adc_p pointer to xadc instance
 */

void adc_check(XadcCore *adc_p, GpoCore *led_p) {
   double reading;


	 uart.disp("\nAnalog channel/voltage: ");
	 uart.disp("0 -> ");
	 reading = adc_p->read_adc_in(0);
	 uart.disp(reading, 3);

}


GpoCore led(get_slot_addr(BRIDGE_BASE, S2_LED));
GpiCore sw(get_slot_addr(BRIDGE_BASE, S3_SW));
XadcCore adc(get_slot_addr(BRIDGE_BASE, S5_XDAC));
PwmCore pwm(get_slot_addr(BRIDGE_BASE, S6_PWM));
DebounceCore btn(get_slot_addr(BRIDGE_BASE, S7_BTN));
SsegCore sseg(get_slot_addr(BRIDGE_BASE, S8_SSEG));
SpiCore spi(get_slot_addr(BRIDGE_BASE, S9_SPI));
I2cCore adt7420(get_slot_addr(BRIDGE_BASE, S10_I2C));
Ps2Core ps2(get_slot_addr(BRIDGE_BASE, S11_PS2));
DdfsCore ddfs(get_slot_addr(BRIDGE_BASE, S12_DDFS));
AdsrCore adsr(get_slot_addr(BRIDGE_BASE, S13_ADSR), &ddfs);


int main() {

	constexpr uint32_t minPeriod = 50;//ms
	constexpr uint32_t maxPeriod = 1500;//ms
	constexpr uint32_t defaultLedVal = 0x00000001;
	constexpr uint32_t stopCtrlSwitchMask = 0x00000001;
	enum class LedChaseDirection {left = 0, right};

	uint32_t swVal, stop;
	uint32_t ledVal = defaultLedVal, period = minPeriod;
	double speedCtrl, newSpeedCtrl;

	LedChaseDirection currentDir;

	//initial value
	led.write(ledVal);
	currentDir = LedChaseDirection::left;

	while (1) {
		swVal = sw.read();
		stop = swVal & stopCtrlSwitchMask;

		newSpeedCtrl = adc.read_adc_in(0);

		period = (int)(newSpeedCtrl * (maxPeriod - minPeriod)) + minPeriod;
		if(newSpeedCtrl != speedCtrl)
		{
			uart.disp("\nCurrent speed: ");
			uart.disp((int)newSpeedCtrl);
			uart.disp("\tPeriod: ");
			uart.disp((int)period);
		}
		speedCtrl = newSpeedCtrl;

		if(stop)
		{
			ledVal = defaultLedVal;
			led.write(ledVal);
			currentDir = LedChaseDirection::left;
		}
		else
		{
			switch(currentDir)
			{
			case LedChaseDirection::left:
				ledVal <<= 1;
				led.write(ledVal);
				break;
			case LedChaseDirection::right:
				ledVal >>= 1;
				led.write(ledVal);
				break;
			default:
				ledVal = defaultLedVal;
				led.write(ledVal);
				currentDir = LedChaseDirection::left;
				break;
			}
		}

		if(ledVal == 0x00000001)
			currentDir = LedChaseDirection::left;
		else if(ledVal == 0x00008000)
			currentDir = LedChaseDirection::right;

		sleep_ms(period);
	} //while
} //main

