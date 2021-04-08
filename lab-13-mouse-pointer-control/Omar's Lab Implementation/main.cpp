#include "chu_init.h"
#include "gpio_cores.h"
#include "sseg_core.h"
#include "ps2_core.h"
#include "vga_core.h"

/*
	Global Variables
*/
float currMouseX = 0;
float currMouseY = 0;

constexpr int horizRes = 640;
constexpr int vertRes = 480;
constexpr float swipeSensitivity = 0.25;

/*
	External Core Instantiation
*/
GpoCore led(get_slot_addr(BRIDGE_BASE, S2_LED));
SsegCore sseg(get_slot_addr(BRIDGE_BASE, S8_SSEG));
Ps2Core ps2(get_slot_addr(BRIDGE_BASE, S11_PS2));
SpriteCore mouse(get_sprite_addr(BRIDGE_BASE, V1_MOUSE), 1024);
SpriteCore ghost(get_sprite_addr(BRIDGE_BASE, V3_GHOST), 1024);

/*
	Function Prototypes
*/
void mouseControl(Ps2Core *ps2_p, SpriteCore *mouse_p);
void DispOff(SsegCore *sseg_p);

int main()
{
	DispOff(&sseg);
	mouse.bypass(0);
	ghost.bypass(1);

	int id = -1;
	id = ps2.init();

	while (1)
	{
		if (id == 2)
		{
			mouseControl(&ps2, &mouse);
		}
		else
		{
			id = ps2.init();
		}
	}
}

void mouseControl(Ps2Core *ps2_p, SpriteCore *mouse_p)
{
	int newHorizPos, newVertPos;
	int lbtn, rbtn, xmov, ymov;
	float xNew = 0;
	float yNew = 0;

	if (ps2_p->get_mouse_activity(&lbtn, &rbtn, &xmov, &ymov))
	{
		// determine amount of movement
		newHorizPos = currMouseX + ((float) xmov) * swipeSensitivity;
		newVertPos = currMouseY + ((float) ymov) * swipeSensitivity;

		// Make sure new position is within the screen
		if (newHorizPos < 0)
			newHorizPos = 0;
		else if ((newHorizPos - horizRes) > 0)
			newHorizPos = horizRes;

		if (newVertPos < 0)
			newVertPos = 0;
		else if ((newVertPos - vertRes) > 0)
			newVertPos = vertRes;

		if (newHorizPos != 0)
		{
			if (currMouseX >= newHorizPos) // left motion logic
			{
				xNew = currMouseX - ((currMouseX - newHorizPos));
			}
			else // right motion logic
			{
				xNew = newHorizPos - ((newHorizPos - currMouseX));
			}

			currMouseX = newHorizPos;
		}
		if (newVertPos != 0)
		{
			if (currMouseY >= newVertPos) // up motion logic
			{
				yNew = newVertPos - ((currMouseY - newVertPos));
			}
			else // down motion logic
			{
				yNew = currMouseY - ((currMouseY - newVertPos));
			}

			currMouseY = newVertPos;
		}

		sleep_ms(5);

		// "flip" yNew to account for inverted vertical input
		yNew = (vertRes - (yNew - 1));

		mouse_p->move_xy(xNew, yNew);
	}
}

void DispOff(SsegCore *sseg_p)
{
	int i;
	for (i = 0; i < 8; i++)
		sseg.write_1ptn(0xff, i);
	sseg.set_dp(0x00);
}
