// #define _DEBUG
#include "chu_init.h"
#include "gpio_cores.h"
#include "spi_core.h"
#include "sseg_core.h"
#include "math.h"

// global variables
const uint32_t LED_ON = 0x0000ffff;
const uint32_t LED_OFF = 0x00000000;
int tapTimes[50] = {0};
int signatureTapTimes[50] = {0};
uint32_t ledState;

// global instantiations
TimerCore tmr(get_slot_addr(BRIDGE_BASE, S0_SYS_TIMER));
GpoCore led(get_slot_addr(BRIDGE_BASE, S2_LED));
GpiCore sw(get_slot_addr(BRIDGE_BASE, S3_SW));
SsegCore sseg(get_slot_addr(BRIDGE_BASE, S8_SSEG));
SpiCore spi(get_slot_addr(BRIDGE_BASE, S9_SPI));

// function prototypes
void CaptureVerticalTapping(SpiCore *spi_p, TimerCore *tmr_p);
void DetectSignature(SpiCore *spi_p, TimerCore *tmr_p, GpiCore *sw_p);
void IsValidSignature(GpoCore *led_p);

int main()
{
	// turn off Sseg display
	int i;
	for (i = 0; i < 8; i++)
		sseg.write_1ptn(0xff, i);
	sseg.set_dp(0x00);

	// initialize LED state
	ledState = LED_OFF;

	while (1)
	{
	   if ((sw.read() & 0x00000007) == 1)       // record signature
	   {
		   CaptureVerticalTapping(&spi, &tmr);
	   }
	   else if ((sw.read() & 0x00000007) == 2)	// enter signature
	   {
		   DetectSignature(&spi, &tmr, &sw);
	   }
	   else if ((sw.read() & 0x00000007) == 4)  // determine if signature is correct
	   {
		   IsValidSignature(&led);
	   }
	}
}

void CaptureVerticalTapping(SpiCore *spi_p, TimerCore *tmr_p)
{
   const uint8_t RD_CMD = 0x0b;
   const uint8_t DATA_REG = 0x08;
   const float raw_max = 127.0 / 2.0;  //128 max 8-bit reading for +/-2
   int8_t yraw;
   float y;

   int tapTime_ms;
   int i = 0;

   tmr_p->clear();
   tmr_p->go();

   while (sw.read(0) == 1) // while recording signature
   {
		spi_p->set_freq(400000);
		spi_p->set_mode(0, 0);

		// read 8-bit y 'g' values once
		spi_p->assert_ss(0);
		spi_p->transfer(RD_CMD);
		spi_p->transfer(DATA_REG);

		(void) spi_p->transfer(0x00); // read twice to trigger increment from xraw to yraw
		yraw = spi_p->transfer(0x00);

		spi_p->deassert_ss(0);

		y = (float) yraw / raw_max;

		if ((abs(y) - 0.05) > 0)
		{
			tapTime_ms = (int) (tmr_p->read_time() / 10000);

			// display recorded information using UART
			uart.disp("Time tap occurred: ");
			uart.disp(tapTime_ms);
			uart.disp("\n\r");

			// record tap signature values
			tapTimes[i] = tapTime_ms;
			++i;

			sleep_ms(100);
		}
   }

   tmr_p->pause();
   tmr_p->clear();
}

void DetectSignature(SpiCore *spi_p, TimerCore *tmr_p, GpiCore *sw_p)
{
	const uint8_t RD_CMD = 0x0b;
	const uint8_t DATA_REG = 0x08;
	const float raw_max = 127.0 / 2.0;  //128 max 8-bit reading for +/-2
	int8_t yraw;
	float y;

	int tapTime_ms;
	int i = 0;

	tmr_p->clear();
	tmr_p->go();

	while (sw.read(1) == 1) // while attempting signature
	{
		spi_p->set_freq(400000);
		spi_p->set_mode(0, 0);

		// read 8-bit y 'g' values once
		spi_p->assert_ss(0);
		spi_p->transfer(RD_CMD);
		spi_p->transfer(DATA_REG);

		(void) spi_p->transfer(0x00); // read twice to trigger increment from xraw to yraw
		yraw = spi_p->transfer(0x00);

		spi_p->deassert_ss(0);

		y = (float) yraw / raw_max;

		if ((abs(y) - 0.05) > 0)
		{
			tapTime_ms = (int) (tmr_p->read_time() / 10000);

			// display recorded information using UART
			uart.disp("Time tap occurred: ");
			uart.disp(tapTime_ms);
			uart.disp("\n\r");

			// record tap signature values
			signatureTapTimes[i] = tapTime_ms;
			++i;

			sleep_ms(100);
		}
	}

	tmr_p->pause();
	tmr_p->clear();
}

void IsValidSignature(GpoCore *led_p)
{
	int correctTaps = 0;

	// determine if signature is correct
	int i, j;
	for (j = 0; j < 50; j++)
	{
		if (abs(tapTimes[j] - signatureTapTimes[j]) <= 100)
		   ++correctTaps;
	}

	if (correctTaps == 50)
	{
		if (ledState == LED_OFF)
		{
		   ledState = LED_ON;
		   led_p->write(ledState);
		}
		else
		{
		   ledState = LED_OFF;
		   led_p->write(ledState);
		}

		for (i = 0; i < 50; i++)
		{
			signatureTapTimes[i] = 0;
		}
	}
}
