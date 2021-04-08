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


void SpectrumDisplay(PwmCore *pwm_p, double inputVoltage) {

   struct Color { double red{0.0}, green{0.0}, blue{0.0};};

   constexpr auto piecewiseSections = 6;
   constexpr auto sectionWidthVolts = 1.0/*volts*/ / piecewiseSections;
   constexpr auto numVEndPts = piecewiseSections + 1;
   constexpr double rVOut[numVEndPts] = {1.0,1.0,0.0,0.0,0.0,1.0,1.0};
   constexpr double gVOut[numVEndPts] = {0.0,1.0,1.0,1.0,0.0,0.0,0.0};
   constexpr double bVOut[numVEndPts] = {0.0,0.0,0.0,1.0,1.0,1.0,0.0};

   //0->5 peicewise sections
   const auto currentSectionIdx = uint32_t{inputVoltage / sectionWidthVolts};
   //section input begin voltage = idx * 0.17666
   const auto currentSectionBeginV = static_cast<double>(currentSectionIdx) * sectionWidthVolts;
   //0.0->1.0 of section
   const auto posInSection = (inputVoltage - currentSectionBeginV) / sectionWidthVolts;

   auto computeOutV = [](double posInSection, double outBeginV, double outEndV) -> double {
      return posInSection * (outEndV - outBeginV) + outBeginV;
   };
   auto currentColor = Color{ 
	   computeOutV(posInSection, rVOut[currentSectionIdx], rVOut[currentSectionIdx + 1]),
	   computeOutV(posInSection, gVOut[currentSectionIdx], gVOut[currentSectionIdx + 1]),
	   computeOutV(posInSection, bVOut[currentSectionIdx], bVOut[currentSectionIdx + 1])
   };

   enum COLOR {BLUE_1 = 0, GREEN_1, RED_1, BLUE_2, GREEN_2, RED_2};
   pwm_p->set_duty(currentColor.red, COLOR::RED_1);
   pwm_p->set_duty(currentColor.green, COLOR::GREEN_1);
   pwm_p->set_duty(currentColor.blue, COLOR::BLUE_1);
   pwm_p->set_duty(currentColor.red, COLOR::RED_2);
   pwm_p->set_duty(currentColor.green, COLOR::GREEN_2);
   pwm_p->set_duty(currentColor.blue, COLOR::BLUE_2);

   sleep_ms(1000);

   for(int n = 0; n < 3; n++) 
   {
      pwm_p->set_duty(0.0, n);
      pwm_p->set_duty(0.0, n + 3);
   }
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

   pwm.set_freq(50);
   sseg.set_dp(0x04);
   for(uint32_t n = 0; n < 3; n++)
	   sseg.write_1ptn(sseg.h2s(0), n);

   while(1) {
	   auto inVoltage = adc.read_adc_in(0);

	   auto tenths = static_cast<int>(inVoltage * 10) % 10;
	   sseg.write_1ptn(sseg.h2s(tenths), 1);
	   auto hundredths =  static_cast<int>(inVoltage * 100) % 10;
	   sseg.write_1ptn(sseg.h2s(hundredths), 0);

	   SpectrumDisplay(&pwm, inVoltage);
   } 
} //main

