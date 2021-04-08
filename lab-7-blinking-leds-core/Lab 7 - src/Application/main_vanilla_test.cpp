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
#include "blinkled_core.h"

// instantiate switch, led
GpoCore led(get_slot_addr(BRIDGE_BASE, S2_LED));
GpiCore sw(get_slot_addr(BRIDGE_BASE, S3_SW));
BlinkLedCore blinkled(get_slot_addr(BRIDGE_BASE, S4_USER));

int main() {
	using BLC = BlinkLedCore;
	blinkled.write(BLC::LED1,500);
	blinkled.write(BLC::LED2,1000);
	blinkled.write(BLC::LED3,2000);
	blinkled.write(BLC::LED4,4000);
} //main
