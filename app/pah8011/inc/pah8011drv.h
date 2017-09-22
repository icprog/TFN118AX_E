#ifndef _pah8011_drv
#define _pah8011_drv
#include "sys.h"

void pah8011_power_on(void);

void pah8011_power_off(void);

u16 GetHeartValue(void);
void pah8011_gpio_init(void);
/*心率值
hr:心率0~255
hr_trust_level:0~3,值越高越好
grade:0~100
*/
typedef struct
{
	float hr;
	int hr_trust_level;
	int16_t grade;
	bool has_updated ;
	uint8_t return_value;
	uint8_t tid;
	float heart[100];
	uint8_t close_flag;
}pah8011Data_t;


#endif


