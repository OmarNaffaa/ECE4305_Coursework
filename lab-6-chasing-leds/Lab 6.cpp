/*****************************************************************//**
 * @file main_vanilla_test.cpp
 *
 * @brief Basic test of 4 basic i/o cores
 *
 * @author p chu
 * @version v1.0: initial release
 *********************************************************************/

//#define _DEBUG
#include "chu_init.h"
#include "gpio_cores.h"

// instantiate switch, led
GpoCore led(get_slot_addr(BRIDGE_BASE, S2_LED));
GpiCore sw(get_slot_addr(BRIDGE_BASE, S3_SW));


void chasing_led(GpoCore *led_p, GpiCore *sw_p)
{
	constexpr uint32_t numSwCtrlPeriod = 5;
	constexpr uint32_t minPeriod = 50;//ms
	constexpr uint32_t maxPeriod = 500;//ms
	constexpr uint32_t numDifferentPeriods = 0xFFFFFFFF >> (32 - numSwCtrlPeriod);
	constexpr uint32_t defaultLedVal = 0x00000001;
	enum class LedChaseDirection {left = 0, right};

	uint32_t swVal, speedCtrl, newSpeedCtrl, stop;
	uint32_t ledVal = defaultLedVal, period = minPeriod;

	LedChaseDirection currentDir;

	//initial value
	led_p->write(ledVal);
	currentDir = LedChaseDirection::left;
	while(1)
	{
		swVal = sw_p->read();
		newSpeedCtrl = (swVal & 0x0000FFFE) >> 1;
		stop = swVal & 0x00000001;

		period = (int)(newSpeedCtrl * (maxPeriod - minPeriod) / numDifferentPeriods) + minPeriod;
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
			led_p->write(ledVal);
			currentDir = LedChaseDirection::left;
		}
		else
		{
			switch(currentDir)
			{
			case LedChaseDirection::left:
				ledVal <<= 1;
				led_p->write(ledVal);
				break;
			case LedChaseDirection::right:
				ledVal >>= 1;
				led_p->write(ledVal);
				break;
			default:
				ledVal = defaultLedVal;
				led_p->write(ledVal);
				currentDir = LedChaseDirection::left;
				break;
			}
		}

		if(ledVal == 0x00000001)
			currentDir = LedChaseDirection::left;
		else if(ledVal == 0x00008000)
			currentDir = LedChaseDirection::right;

		sleep_ms(period);
	}
}

int main() {

	chasing_led(&led, &sw);
} //main

