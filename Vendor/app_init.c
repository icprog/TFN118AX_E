//app_init.c
#include "nrf.h"
#include "app_init.h"

#define PACKET0_S1_SIZE                  (0UL)  //!< S1 size in bits
#define PACKET0_S0_SIZE                  (0UL)  //!< S0 size in bits
#define PACKET0_PAYLOAD_SIZE             (0UL)  //!< payload size in bits
#define PACKET1_BASE_ADDRESS_LENGTH      (4UL)  //!< base address length in bytes
#define PACKET1_STATIC_LENGTH            (32UL)  //!< static length in bytes
#define PACKET1_PAYLOAD_SIZE             (32UL)  //!< payload size in bytes
uint8_t Off_sec_Delay = 0;												//腕带摘除延时
uint8_t Off_sec_Over = 0;
uint8_t Off_Flag = 1;

extern uint8_t radio_rcvok;
const uint8_t Version[20] = "TFN108SCF11V07";
/*******************************************************
函数名：RTC初始化
参  数:无
返  回:无
*******************************************************/
void rtc_Init(void)
{
	NRF_RTC0->PRESCALER = 4095;//125msCount计数
//	NRF_RTC0->CC[0] = 9;//1s中断,修正后约9个计数周期
	NRF_RTC0->EVENTS_TICK = 0;//EVENTS_TICK
	NRF_RTC0->INTENSET =  RTC_INTENSET_TICK_Enabled<< RTC_INTENSET_TICK_Pos;//RTC_INTENSET_TICK_Enabled
	NRF_RTC0->TASKS_START = 1;
	
	NVIC_SetPriority(RTC0_IRQn,RTC_PRIORITY);
	NVIC_ClearPendingIRQ(RTC0_IRQn);
	NVIC_EnableIRQ( RTC0_IRQn );
}

/*******************************************************
函数名:低频晶振初始化
参  数:无
返  回:无
*******************************************************/
void Osc_LFCLK(void)
{
	NRF_CLOCK->EVENTS_LFCLKSTARTED = 0;
	//选择外部晶振
	NRF_CLOCK->LFCLKSRC = CLOCK_LFCLKSRC_SRC_RC << CLOCK_LFCLKSRC_SRC_Pos;
  NRF_CLOCK->TASKS_LFCLKSTART = 1;
	while (NRF_CLOCK->EVENTS_LFCLKSTARTED == 0) 
  {
  }
}
/*******************************************************
函数名:2.5V低电检测
参  数:无
返  回:无
*******************************************************/
void power_Init(void)
{
	NRF_POWER->POFCON = 5UL;//Enable power failure comparator，set threshold to 2.5V
}
/*******************************************************
函数名:RADIO初始化
参  数:无
返  回:无
*******************************************************/
void radio_Init(void)
{
  // Radio config
  NRF_RADIO->TXPOWER = (RADIO_TXPOWER_TXPOWER_0dBm << RADIO_TXPOWER_TXPOWER_Pos);
  NRF_RADIO->FREQUENCY = 2UL;                // Frequency bin 7, 2407MHz
  NRF_RADIO->MODE = (RADIO_MODE_MODE_Nrf_1Mbit << RADIO_MODE_MODE_Pos);

  // Radio address config
  NRF_RADIO->PREFIX0 = 0xC4C3C2E7UL;  // Prefix byte of addresses 3 to 0
  NRF_RADIO->PREFIX1 = 0xC5C6C7C8UL;  // Prefix byte of addresses 7 to 4
  NRF_RADIO->BASE0   = 0xE7E7E7E7UL;  // Base address for prefix 0
  NRF_RADIO->BASE1   = 0x00C2C2C2UL;  // Base address for prefix 1-7
  NRF_RADIO->TXADDRESS = 0x00UL;      // Set device address 0 to use when transmitting
  NRF_RADIO->RXADDRESSES = 0x01UL;    // Enable device address 0 to use which receiving

  // Packet configuration
  NRF_RADIO->PCNF0 = (PACKET0_S1_SIZE << RADIO_PCNF0_S1LEN_Pos) |
                     (PACKET0_S0_SIZE << RADIO_PCNF0_S0LEN_Pos) |
                     (PACKET0_PAYLOAD_SIZE << RADIO_PCNF0_LFLEN_Pos); //lint !e845 "The right argument to operator '|' is certain to be 0"

  // Packet configuration
   NRF_RADIO->PCNF1 = (RADIO_PCNF1_WHITEEN_Disabled << RADIO_PCNF1_WHITEEN_Pos)    |
                      (RADIO_PCNF1_ENDIAN_Big << RADIO_PCNF1_ENDIAN_Pos)           |
                      (PACKET1_BASE_ADDRESS_LENGTH << RADIO_PCNF1_BALEN_Pos)       |
                      (PACKET1_STATIC_LENGTH << RADIO_PCNF1_STATLEN_Pos)           |
                      (PACKET1_PAYLOAD_SIZE << RADIO_PCNF1_MAXLEN_Pos); //lint !e845 "The right argument to operator '|' is certain to be 0"
											
	//shorts
	NRF_RADIO->SHORTS = (RADIO_SHORTS_READY_START_Enabled << RADIO_SHORTS_READY_START_Pos);

  // CRC Config
  NRF_RADIO->CRCCNF = (RADIO_CRCCNF_LEN_Two << RADIO_CRCCNF_LEN_Pos); // Number of checksum bits
  if ((NRF_RADIO->CRCCNF & RADIO_CRCCNF_LEN_Msk) == (RADIO_CRCCNF_LEN_Two << RADIO_CRCCNF_LEN_Pos))
  {
    NRF_RADIO->CRCINIT = 0xFFFFUL;      // Initial value      
    NRF_RADIO->CRCPOLY = 0x11021UL;     // CRC poly: x^16+x^12^x^5+1
  }
  else if ((NRF_RADIO->CRCCNF & RADIO_CRCCNF_LEN_Msk) == (RADIO_CRCCNF_LEN_One << RADIO_CRCCNF_LEN_Pos))
  {
    NRF_RADIO->CRCINIT = 0xFFUL;        // Initial value
    NRF_RADIO->CRCPOLY = 0x107UL;       // CRC poly: x^8+x^2^x^1+1
  }
  nrf_delay_ms(3);
	radio_rcvok = 0;
}

/*******************************************************
函数名：定时器初始化，10ms间隔，中断
参  数:无
返  回:无
*******************************************************/
void timer_Init(void)
{
	//传感器脉冲计算，16uS时钟
	NRF_TIMER1 -> PRESCALER = (8 << TIMER_PRESCALER_PRESCALER_Pos);
	NRF_TIMER1 -> BITMODE = (TIMER_BITMODE_BITMODE_16Bit << TIMER_BITMODE_BITMODE_Pos);
	NRF_TIMER1 -> MODE = (TIMER_MODE_MODE_Timer << TIMER_MODE_MODE_Pos);
	NRF_TIMER1 -> INTENSET = (TIMER_INTENSET_COMPARE0_Enabled << TIMER_INTENSET_COMPARE0_Pos);
	NRF_TIMER1 -> CC[0] = 812;//625-10ms,812-12.992ms
	
	NVIC_SetPriority(TIMER1_IRQn,TIM_PRIORITY);
	NVIC_EnableIRQ( TIMER1_IRQn );
}
//FILE END
