#include "chu_init.h"
#include "gpio_cores.h"
#include "sseg_core.h"
#include "ps2_core.h"

/*
	aliases
*/
#define LEFT true;
#define RIGHT false;

/*
	global constants
*/
constexpr uint8_t numOfLeds = 16;
constexpr int numOfSsegDisp = 8;
constexpr int minSpeed = 50;
constexpr int maxSpeed = 1000;
constexpr double swipeSensitivity = 0.15;

/*
	global core instantiations
*/
GpoCore led(get_slot_addr(BRIDGE_BASE, S2_LED));
SsegCore sseg(get_slot_addr(BRIDGE_BASE, S8_SSEG));
Ps2Core ps2(get_slot_addr(BRIDGE_BASE, S11_PS2));

/*
	function prototypes
*/
void ledChase(GpoCore *led_p, Ps2Core *ps2_p, uint8_t* pauseChaser, int *chaserSpeed);
void chaserMouseControl(Ps2Core *ps2_p, uint8_t *toggle, int *chaserSpeed);
void DispOff(SsegCore *sseg_p);

int main()
{
	int id = -1;
	int chaserSpeed = 0;
	uint8_t pauseOrResumeChaser = 0;

	DispOff(&sseg);

	while (id != 2) // wait for mouse to initialize
	{
		id = ps2.init();
	}

	while (1)
	{
		chaserMouseControl(&ps2, &pauseOrResumeChaser, &chaserSpeed);
		ledChase(&led, &ps2, &pauseOrResumeChaser, &chaserSpeed);
	}
}

void ledChase(GpoCore *led_p, Ps2Core *ps2_p, uint8_t *pauseChaser, int *currChaseSpeed)
{
	static bool moveDir = LEFT;
	static uint8_t currLedPos = 0;
	static uint8_t prevLedPos = 0;
	unsigned long last;

	if (*currChaseSpeed == 0)
		*currChaseSpeed = minSpeed;
	if (*pauseChaser)
		return;

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

	// poll inputs during chaser delays
	last = now_ms();
	while (now_ms() - last < ((long unsigned) *currChaseSpeed))
	{
		chaserMouseControl(ps2_p, pauseChaser, currChaseSpeed);
	}
}

void chaserMouseControl(Ps2Core *ps2_p, uint8_t *toggle, int *chaserSpeed)
{
	int lbtn, rbtn, xmov, ymov;
	int rClickToggle = 0;

	if (ps2_p->get_mouse_activity(&lbtn, &rbtn, &xmov, &ymov))
	{
		if (lbtn == 1)
		{
			uart.disp("Left button clicked ");
			uart.disp("\n\r");
			uart.disp("\n\r");

			*toggle = (*toggle == 1) ? 0 : 1;
			return;
		}
		if (rbtn == 1)
		{
			rbtn = 0;
			rClickToggle = 1;

			uart.disp("Right button clicked ");
			uart.disp(rClickToggle);
			uart.disp("\n\r");

			uart.disp("Current Rate: ");
			uart.disp(*chaserSpeed);
			uart.disp("\n\r");

			do
			{
				if (ps2_p->get_mouse_activity(&lbtn, &rbtn, &xmov, &ymov))
				{
					if (rbtn)
						rClickToggle = 0;
					else
						*chaserSpeed += (int)(((double) xmov) * swipeSensitivity);
				}

			} while (rClickToggle);

			if (*chaserSpeed > maxSpeed)
				*chaserSpeed = maxSpeed;
			if (*chaserSpeed < minSpeed)
				*chaserSpeed = minSpeed;

			uart.disp("Updated Rate: ");
			uart.disp(*chaserSpeed);
			uart.disp("\n\r");
			uart.disp("\n\r");

			return;
		}
	}
}

void DispOff(SsegCore *sseg_p)
{
	int i;
	for (i = 0; i < 8; i++)
		sseg.write_1ptn(0xff, i);
	sseg.set_dp(0x00);
}
