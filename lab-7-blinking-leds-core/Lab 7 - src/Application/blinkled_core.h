
#ifndef _BLED_H_INCLUDED
#define _BLED_H_INCLUDED

#include "chu_init.h"

/**********************************************************************
 * 	Lab 7 Blink-Led core driver
 **********************************************************************/
/**
 * 	Lab 7 Blink-Led core driver
 *  - write data to blink core.
 */
class BlinkLedCore {
public:
   /**
    * register map
    *
    */
   enum {
	      LED1 = 0,
	      LED2 = 1,
	      LED3 = 2,
	      LED4 = 3,
   };
   /**
    * constructor.
    *
    */
   BlinkLedCore(uint32_t core_base_addr);
   ~BlinkLedCore();                  // not used

   /**
    * write a 32-bit word
    * @param data 32-bit data
    *GpoCore
    */
   void write(uint32_t ledIdx, uint32_t data);


private:
   uint32_t base_addr;
   uint32_t wr_data[4];      // same as GPO core data reg
};



#endif  // _BLED_H_INCLUDED
