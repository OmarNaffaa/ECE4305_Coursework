//#define _DEBUG
#include "chu_init.h"
#include "gpio_cores.h"

// instantiate switch, led
GpoCore led(get_slot_addr(BRIDGE_BASE, S2_LED));
GpiCore sw(get_slot_addr(BRIDGE_BASE, S3_SW));

// function prototypes
uint16_t ledInit(GpoCore *led_p, GpiCore *sw_p);	  // reset LED to rightmost position (SW0)
uint32_t ledSpeed(GpoCore *led_p, GpiCore *sw_p);  	  // determine and return arbitrary speed increments (SW1 - SW5)

int main()
{
	// variable declaration
	bool moveLeft = true;
	uint16_t ledIter;
	uint32_t chaseSpeed, chaseSpeedPrev;

	// force initial state instead of unknown case
	ledIter = ledInit(&led, &sw);

	while (1) {
		// continually check for initialization and LED speed
		if(sw.read(0) == 1)
			ledIter = ledInit(&led, &sw);

		chaseSpeedPrev = chaseSpeed;
		chaseSpeed = ledSpeed(&led, &sw);

		// determine when LED should change directions
		if(ledIter == 0x8000) moveLeft = false;
		if(ledIter == 0x0001) moveLeft = true;

		// write 16-bit "ledIter" map to FPGA LEDs
		led.write(ledIter);

		// "slide" LED based on direction determined above
		ledIter = (moveLeft) ? ledIter << 1 : ledIter >> 1;

		// wait according to delay and display delay time if a change occurs
		if(chaseSpeed != chaseSpeedPrev)
		{
			uart.disp("Current Speed: ");
			uart.disp(chaseSpeed, 10, 4);	// display a decimal value up to 4 digits
			uart.disp("\n\r");
		}

		sleep_ms(chaseSpeed);
	}

}

uint16_t ledInit(GpoCore *led_p, GpiCore *sw_p)
{
	led_p->write(0x0001);

	// set count value to 0
	return 0x0001;
}

uint32_t ledSpeed(GpoCore *led_p, GpiCore *sw_p)
{
	uint32_t ledSpeed;

	/*
	 	 assign a delay - in ms - based on the values from the switches

	 	 if all switches down return 100 ms, otherwise use equation below to calculate delay value
	*/

	ledSpeed = (sw_p->read() == 0x0000) ? 100 : ((300 * sw_p->read(5)) + (250 * sw_p->read(4)) + (200 * sw_p->read(3)) + (130 * sw_p->read(2)) + (119 * sw_p->read(1)));

	return ledSpeed;
}
