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

#ifndef __pah_comm_h__
#define __pah_comm_h__


#include "pah_platform_types.h"


bool  pah_comm_write(uint8_t addr, uint8_t data);
bool  pah_comm_read(uint8_t addr, uint8_t *data);
bool  pah_comm_burst_read(uint8_t addr, uint8_t *data, uint16_t num);


#endif  // header guard
