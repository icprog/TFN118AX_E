/*==============================================================================
* Edit History
* 
* This section contains comments describing changes made to the module. Notice
* that changes are listed in reverse chronological order. Please use ISO format
* for dates.
* 
* when       who       what, where, why
* ---------- ---       -----------------------------------------------------------
* 2016-04-12 bh        Add license information and revision information
* 2016-04-07 bh        Initial revision.
==============================================================================*/

#include "pah_comm.h"

// platform support
#include "pah_platform_functions.h"

#include "pah8011drv.h"
extern uint8_t pah8011I2cAddress;
/*============================================================================
STATIC VARIABLE DEFINITIONS
============================================================================*/

// valid bank range: 0x00 ~ 0x03
static uint8_t _curr_bank = 0xFF;


/*============================================================================
PUBLIC FUNCTION DEFINITIONS
============================================================================*/
bool pah_comm_write(uint8_t addr, uint8_t data)
{
		uint8_t txData[2];
    if (addr == 0x7F)
    {
        if (_curr_bank == data)
            return true;
				
				txData[0] = addr;
				txData[1] = data;
        if (I2C_SendDates(pah8011I2cAddress,txData,2))
            return false;

        _curr_bank = data;
        return true;
    }
		txData[0] = addr;
		txData[1] = data;
    return ~I2C_SendDates(pah8011I2cAddress,txData,2);
}

bool pah_comm_read(uint8_t addr, uint8_t *data)
{
		return ~I2C_ReadDatas(pah8011I2cAddress,addr,data,1); 
}

bool pah_comm_burst_read(uint8_t addr, uint8_t *data, uint16_t num)
{
		return ~I2C_ReadDatas(pah8011I2cAddress,addr,data,num); 
}
