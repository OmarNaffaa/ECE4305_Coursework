/*****************************************************************//**
 * @file gpio_core.cpp
 *
 * @brief implementation of various i/o related classes
 *
 * @author p chu
 * @version v1.0: initial release
 ********************************************************************/

#include "blinkled_core.h"


/**********************************************************************
 * GpoCore
 **********************************************************************/
BlinkLedCore::BlinkLedCore(uint32_t core_base_addr) {
   base_addr = core_base_addr;
   for(auto& led : wr_data) led = 0;
}

BlinkLedCore::~BlinkLedCore() {
}

void BlinkLedCore::write(uint32_t ledIdx, uint32_t data) {
   wr_data[ledIdx] = data;
   io_write(base_addr, ledIdx, wr_data[ledIdx]);
}

