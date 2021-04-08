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



/*************************************************************************
                        Helper Types/Classes          
 *************************************************************************/

//Created solely for the purpose of preventing
//accidental mistreatment of Celsius/Fahrenheit double values
struct celsTemp
{
   double val;

   static double convertCelsToFahr(celsTemp celsTemp)
   {
      return (celsTemp.val * (9.0/5.0) + 32.0);
   }
};

//the temperature is required to be four digits
//in length when displayed
constexpr auto numTempDigits = 4;
using tempDigitArr = uint8_t[numTempDigits];


/*************************************************************************
                        Function Declarations          
 *************************************************************************/
void        thermometer(I2cCore*, SsegCore*);
void        displayTempUart(celsTemp);
celsTemp    getTempInC(I2cCore*);
void        dblTempToDigitArr(double temp, tempDigitArr);
void        displayNothing(SsegCore*);
void        displayTemp(SsegCore*, const tempDigitArr, uint8_t tempUnitHexLetter);
void        dispCelsOnlyState(SsegCore*, celsTemp);
void        dispFahrOnlyState(SsegCore*, celsTemp);
void        dispCelsFahrState(SsegCore*, celsTemp);

/*************************************************************************
                        Main          
 *************************************************************************/
int main() {
   while (1) {
      thermometer(&adt7420, &sseg);
   }
}

/*************************************************************************
                        Misc Invariants          
 *************************************************************************/
namespace DisplayStateMask { 
   constexpr auto btnuBitIdx = 0u, 
                  btndBitIdx = 2u, 
                  btnlBitIdx = 3u, 
                  btnrBitIdx = 1u;
   constexpr auto celsOnlyDisp = 0x1u << btnuBitIdx;
   constexpr auto fahrOnlyDisp = 0x1u << btndBitIdx;
   constexpr auto celsFahrDisp = 0x1u << btnlBitIdx; 
   constexpr auto blankOutDisp = 0x1u << btnrBitIdx;
};
constexpr auto initState = DisplayStateMask::celsOnlyDisp;

/*************************************************************************
                        Function Definitions         
 *************************************************************************/
void thermometer(I2cCore* adt7420_p, SsegCore* sseg_p)
{
   static auto currState = initState;  //retains state on each call

   const auto currTemp = getTempInC(adt7420_p);

   displayTempUart(currTemp);

   switch (currState)
   {
   case DisplayStateMask::celsOnlyDisp: 
      dispCelsOnlyState(sseg_p, currTemp);
      break;
   case DisplayStateMask::fahrOnlyDisp: 
      dispFahrOnlyState(sseg_p, currTemp);
      break;
   case DisplayStateMask::celsFahrDisp: 
      dispCelsFahrState(sseg_p, currTemp);
      break;
   case DisplayStateMask::blankOutDisp: 
      displayNothing(sseg_p);
      break;
   default:
      displayNothing(sseg_p);
      break;
   }

   const auto btnPress = btn.read();
   if(btnPress)
   {
      if(btnPress & DisplayStateMask::celsOnlyDisp)
         currState = DisplayStateMask::celsOnlyDisp;
      else if(btnPress & DisplayStateMask::fahrOnlyDisp)
         currState = DisplayStateMask::fahrOnlyDisp;
      else if(btnPress & DisplayStateMask::celsFahrDisp)
         currState = DisplayStateMask::celsFahrDisp;
      else if(btnPress & DisplayStateMask::blankOutDisp)
         currState = DisplayStateMask::blankOutDisp;
         
   }
}

void displayTempUart(celsTemp currTemp)
{
   uart.disp("\nTemp C: ");
   uart.disp(currTemp.val);
   uart.disp("\tTemp F: ");
   uart.disp(celsTemp::convertCelsToFahr(currTemp));
}

celsTemp getTempInC(I2cCore* adt7420_p)
{
   constexpr auto deviceAddress = uint8_t{0x4b};
   enum class requiresRestart { no = 0, yes = 1};
   constexpr auto numWriteBytes = 1u;
   constexpr auto numReadBytes = 2u;

   uint8_t wBytes[numWriteBytes], rBytes[numReadBytes];

   wBytes[0] = 0x00u;   //zero out dummy send byte
   (void)adt7420_p->write_transaction(deviceAddress, wBytes, numWriteBytes, (int)requiresRestart::yes);
   (void)adt7420_p->read_transaction(deviceAddress, rBytes, numReadBytes, (int)requiresRestart::no);

   constexpr auto adtValBitOffset = uint8_t{ 3u };
   constexpr auto byteLen = uint8_t{ 8u };
   const auto upperByte = static_cast<uint16_t>( rBytes[0] << (byteLen - adtValBitOffset) );
   const auto lowerByte = static_cast<uint16_t>( rBytes[1] >> adtValBitOffset );
   const auto prelimCelsTemp = upperByte | lowerByte;

   constexpr auto signBitMask = uint16_t{ 0x1u << (2u * byteLen - adtValBitOffset)};

   const auto tempInCels =  celsTemp{  ( prelimCelsTemp & signBitMask ) ? 
                                       ( -1.0 * static_cast<double>(prelimCelsTemp ^ signBitMask) / 16.0) :
                                       ( +1.0 * static_cast<double>(prelimCelsTemp) / 16.0)
                           };
                           
   return tempInCels;
}

void dblTempToDigitArr(double temp, tempDigitArr tempArr)
{
   const auto absTemp = (temp < 0.0) ? (temp * -1.0) : temp;

   const auto tensPlace = absTemp / 10.0 ;
   tempArr[3] = static_cast<uint8_t>(tensPlace);

   const auto onesPlace = static_cast<uint32_t>(absTemp) % 10u;
   tempArr[2] = static_cast<uint8_t>(onesPlace);

   const auto tenthsPlace = static_cast<uint32_t>(absTemp * 10.0) % 10u;
   tempArr[1] = static_cast<uint8_t>(tenthsPlace);

   const auto hundredthsPlace = static_cast<uint32_t>(absTemp * 100.0) % 10u;
   tempArr[0] = static_cast<uint8_t>(hundredthsPlace);

}

void displayNothing(SsegCore* sseg_p)
{
   constexpr auto dpOffPattern = uint8_t{0x00};
   sseg_p->set_dp(dpOffPattern);

   constexpr auto ssegOffPattern = uint8_t{0xff};
   constexpr auto leastSigSseg = 0u;
   constexpr auto mostSigSseg = 7u;
   for(auto ssegIdx = leastSigSseg; ssegIdx < mostSigSseg + 1; ssegIdx++)
      sseg_p->write_1ptn(ssegOffPattern, ssegIdx);
}

void displayTemp(SsegCore* sseg_p, const tempDigitArr displayDigits, uint8_t tempUnitHexLetter)
{
   displayNothing(sseg_p);

   constexpr auto decimalPointPattern = uint8_t{0x1 << 3u}; 
   sseg_p->set_dp(decimalPointPattern);

   constexpr auto tempLetterSsegIdx = 0u;
   sseg_p->write_1ptn(sseg_p->h2s(tempUnitHexLetter), tempLetterSsegIdx);

   constexpr auto lsbSsegIdx = 1u;
   constexpr auto msbSsegIdx = 4u;
   for(auto ssegIdx = lsbSsegIdx; ssegIdx < msbSsegIdx + 1; ssegIdx++)
      sseg_p->write_1ptn(sseg_p->h2s( displayDigits[ssegIdx - lsbSsegIdx] ), ssegIdx);
}

void dispCelsOnlyState(SsegCore* sseg_p, celsTemp temp)
{
   tempDigitArr displayDigits{};
   dblTempToDigitArr(temp.val, displayDigits);
   
   displayTemp(sseg_p, displayDigits, 0xC);
}

void dispFahrOnlyState(SsegCore* sseg_p, celsTemp temp)
{
   tempDigitArr displayDigits{};
   const auto fahrTemp = celsTemp::convertCelsToFahr(temp);
   dblTempToDigitArr(fahrTemp, displayDigits);
   
   displayTemp(sseg_p, displayDigits, 0xF);
}

void dispCelsFahrState(SsegCore* sseg_p, celsTemp temp)
{
   constexpr auto sizeOfNibble = 4u;
   constexpr auto decimalPointPattern = (0x1 << 2u) | (0x1 << (2u + sizeOfNibble)); 
   sseg_p->set_dp(decimalPointPattern);

   tempDigitArr displayCelDigits{};
   dblTempToDigitArr(temp.val, displayCelDigits);

   constexpr auto celsLsbSsegIdx = 0u;
   constexpr auto celsMsbSsegIdx = 3u;
   for(auto ssegIdx = celsLsbSsegIdx; ssegIdx < celsMsbSsegIdx + 1; ssegIdx++)
      sseg_p->write_1ptn(sseg_p->h2s( displayCelDigits[ssegIdx - celsLsbSsegIdx] ), ssegIdx);

   tempDigitArr displayFahrDigits{};
   const auto fahrTemp = celsTemp::convertCelsToFahr(temp);
   dblTempToDigitArr(fahrTemp, displayFahrDigits);

   constexpr auto fahrLsbSsegIdx = 4u;
   constexpr auto fahrMsbSsegIdx = 7u;
   for(auto ssegIdx = fahrLsbSsegIdx; ssegIdx < fahrMsbSsegIdx + 1; ssegIdx++)
      sseg_p->write_1ptn(sseg_p->h2s( displayFahrDigits[ssegIdx - fahrLsbSsegIdx] ), ssegIdx);
}

