//app_init.h
#include "stdint.h"
#include "nrf51.h"
#include "nrf51_bitfields.h"
#include "nrf_delay.h"
#include "string.h"

#ifdef  HARD1
#define IO_OFF_I 	17
#define SENSOR_IN	23
#define	KEY				26     //26
#define LED				27
#define SSR_PWREN	28
#define IO_OFF_D 	5
#else
#define IO_OFF_I 	17
#define SENSOR_IN	23
#define	KEY				2  //24
#define LED				25
#define SSR_PWREN	28
#define IO_OFF_D 	29
#endif

#define GPIO_INVALID	0x00000002//引脚无效，关至最低功耗
#define OFF_SEC_DELAY													40	//佩戴后心率启动延时

typedef enum
{
    APP_IRQ_PRIORITY_HIGHEST = 0,
    APP_IRQ_PRIORITY_HIGH    = 1,
    APP_IRQ_PRIORITY_MID     = 2,
    APP_IRQ_PRIORITY_LOW     = 3
} app_irq_priority_t;

typedef enum{
	LEVEL_IDLE = 0,
	LEVEL_RADIO,
}run_level_t;

#define RADIO_PRIORITY		APP_IRQ_PRIORITY_MID
#define RTC_PRIORITY			APP_IRQ_PRIORITY_HIGHEST

#define TIM_PRIORITY      APP_IRQ_PRIORITY_HIGHEST
#define PORT_PRIORITY     APP_IRQ_PRIORITY_HIGH

#define DAY_TIME												86400				//一天秒数		

void rtc_Init(void);
void radio_Init(void);
void Osc_LFCLK(void);
void power_Init(void);
void timer_Init(void);
//FILE END
