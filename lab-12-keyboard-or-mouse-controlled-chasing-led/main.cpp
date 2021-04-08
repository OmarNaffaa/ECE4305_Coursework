#include "chu_init.h"
#include "gpio_cores.h"
#include "xadc_core.h"
#include "sseg_core.h"
#include "spi_core.h"
#include "i2c_core.h"
#include "ps2_core.h"
#include "ddfs_core.h"
#include "adsr_core.h"

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

void chasingLeds(GpoCore* led_p, uint32_t period_ms, bool pause)
{
   
   //*****************************************************helper functions & parameters
   enum class LedChaseDirection {left = 0, right};
   constexpr auto numSwCtrlPeriod = 5u;
   constexpr auto minPeriod = 0u;//ms
   constexpr auto maxPeriod = 999u;//ms
   constexpr auto defaultLedVal = 0x00000001;

   //**************************************chasing leds static state
   static auto ledVal = defaultLedVal;
   static auto period = uint32_t{};
   static auto currentDir = LedChaseDirection::left;
   static auto timeSinceLastFlash = 0;

   period = period_ms;
   const auto currTime = now_ms();

   if(currTime - timeSinceLastFlash >= period)
   {
      timeSinceLastFlash = now_ms();

      if(!pause)
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

         if(ledVal == 0x00000001)
            currentDir = LedChaseDirection::left;
         else if(ledVal == 0x00008000)
            currentDir = LedChaseDirection::right;
      }
   }
}

void queryKeyboard(Ps2Core* ps2_p, uint32_t& period_ms, bool& pause)
{
   //*****************************************************helper functions & parameters
   enum class QueryState { waitingForSeqBegin = 0, hundredsDigit, tensDigit, onesDigit };
   constexpr auto initKey = char{ '\xF0' };
   constexpr auto pauseKey = char{ '\x70' };
   auto isValidDigitKey = [](char key)
   {
      return key >= 0x30 && key <= 0x39;
   };
   auto getDigitFromKey = [](char key)
   {
      return key - 0x30;
   };
   //*****************************************************static state of function
   static auto currentState = QueryState::waitingForSeqBegin;
   static uint32_t hundreds, tens, ones;
   //*****************************************************current state logic
   char buf;
   if(ps2_p->get_kb_ch(&buf))
   {
      pause = (buf == pauseKey) ? !pause : pause;

      switch (currentState)
      {
      case QueryState::waitingForSeqBegin:
         if(buf == initKey)
            currentState = QueryState::hundredsDigit;
         break;
      case QueryState::hundredsDigit:
         if(isValidDigitKey(buf))
         {
            hundreds = getDigitFromKey(buf);
            currentState = QueryState::tensDigit;
         }
         else
            currentState = QueryState::waitingForSeqBegin;
         break;
      case QueryState::tensDigit:
         if(isValidDigitKey(buf))
         {
            tens = getDigitFromKey(buf);
            currentState = QueryState::onesDigit;
         }
         else
            currentState = QueryState::waitingForSeqBegin;
         break;
      case QueryState::onesDigit:
         if(isValidDigitKey(buf))
         {
            ones = getDigitFromKey(buf);
            period_ms = 100u * hundreds + 10u * tens + 1u * ones;
            uart.disp("\nCurrent Period: ");
            uart.disp((int)period_ms);
         }
         currentState = QueryState::waitingForSeqBegin;
         break;
      default:
         currentState = QueryState::waitingForSeqBegin;
         break;
      }

   }
}

int main() {
   const auto id = ps2.init();
   auto period_ms = uint32_t{500};
   bool pause = false;
   while (1) {
      queryKeyboard(&ps2, period_ms, pause);
      chasingLeds(&led, period_ms, pause);
   } //while
} //main

