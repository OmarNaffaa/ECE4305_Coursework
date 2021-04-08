// #define _DEBUG
#include "chu_init.h"
#include "gpio_cores.h"
#include "xadc_core.h"

// aliases
#define LEFT true;
#define RIGHT false;

// global constants
constexpr uint8_t numOfLeds = 16;
constexpr double maxSpeed = 1000.0;

// global core instantiations
GpoCore led(get_slot_addr(BRIDGE_BASE, S2_LED));
GpiCore sw(get_slot_addr(BRIDGE_BASE, S3_SW));
XadcCore adc(get_slot_addr(BRIDGE_BASE, S5_XDAC));

// function prototypes
void ledChase(GpoCore *led_p, GpiCore *sw_p, XadcCore *adc_p);
double ledChaseSpeed(XadcCore *adc_p);

int main()
{
	while(1)
	{
		ledChase(&led, &sw, &adc);
	}
}

void ledChase(GpoCore *led_p, GpiCore *sw_p, XadcCore *adc_p)
{
	static bool moveDir = LEFT;
	static uint8_t currLedPos = 0;
	static uint8_t prevLedPos = 0;
	static double currChaseSpeed, prevChaseSpeed;

	// check if initialization switch is high
	if(sw_p->read(0) == 1)
	{
		moveDir = LEFT;
		currLedPos = 0;
		prevLedPos = 0;

		led_p->write(0x00000001);

		return;
	}

	// led write logic
	led_p->write(0, prevLedPos);
	led_p->write(1, currLedPos);
	prevLedPos = currLedPos;

	// led position logic
	if(currLedPos == 0)
		moveDir = LEFT;
	if(currLedPos == numOfLeds - 1)
		moveDir = RIGHT;

	// led direction logic
	if(moveDir)
		currLedPos++;
	else
		currLedPos--;

	// track and display speed changes
	prevChaseSpeed = currChaseSpeed;
	currChaseSpeed = ledChaseSpeed(adc_p);

	if(currChaseSpeed != prevChaseSpeed)
	{
		uart.disp("Current Speed: ");
		uart.disp(currChaseSpeed, 10, 3);	// display a decimal value up to 3 digits
		uart.disp("\n\r");
	}

	sleep_ms(currChaseSpeed);
}

double ledChaseSpeed(XadcCore *adc_p)
{
	double swSpeedValue = adc_p->read_adc_in(0) * maxSpeed;

	return swSpeedValue;
}
