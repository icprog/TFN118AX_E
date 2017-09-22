#include "pah8011drv.h"

// pah8011
#include "pah_driver.h"

// motion algorithm
#include "pah8series_api_c.h"

// pah
#include "pah_driver_8011_reg.h"
#include "pah_verify.h"
//Pixart Factory test 
#include "pah_factory_test.h"

#include "pah_comm.h"
#include "pah_driver_types.h"
// c
#include <string.h>
#include <stdlib.h>
//nrf51
#include "nrf_delay.h"
#include "pah8011_io.h"
#include "sys.h"
#include "twi_master.h"
#include "app_sensor.h"
#include "nrf_gpio.h"
#include "app_tim.h"
#include "accelerometer.h"
#include "Debug_log.h"

#include "nrf_nvmc.h"
#include "app_init.h"

//#define PAH8011_EXT_DS_DATA_READ_ENABLE() \
//    do {                                                \
//		NRF_GPIO->PIN_CNF[PPG_INT_PIN_NUM]=0x000000; /* enable pull up for HANDLINE  AS7000 GPIO5*/  \
//		} while ( 0 )
//		
#define PAH8011_EXT_DS_DATA_READ_LowP() \
    do {                                                \
		NRF_GPIO->PIN_CNF[PPG_INT_PIN_NUM]=0x000002; /*disable input buffer*/   \
    } while ( 0 )
//		
//				/*使能中断*/		
//#define PAH8011_EXT_DS_DATA_READ_ENABLE_CN_INTERRUPT() \
//    do { 																							\
//				NRF_GPIOTE->EVENTS_IN[0] = 0; /* clear interrupt status */    \
//				NRF_GPIOTE->INTENSET  = GPIOTE_INTENSET_IN0_Enabled;\
//				NVIC_EnableIRQ(GPIOTE_IRQn);\
//    } while ( 0 )

//#define PAH8011_EXT_DS_DATA_READ_CHECK_REQUEST_BIT() \
//     (Read_HANDLINE == (AS7000_EXT_DS_DATA_READ_REQUEST) )


//#define PAH8011_EXT_DS_DATA_READ_DISABLE_CN_INTERRUPT() \
//    do {                                                        \
//				NRF_GPIOTE->EVENTS_IN[0] = 0;\
//				NRF_GPIOTE->INTENSET = GPIOTE_INTENSET_IN0_Disabled;		\
//        NVIC_DisableIRQ(GPIOTE_IRQn); /* disable change notification interrupts */ \
//    } while ( 0 )

		
typedef enum
{
    main_status_idle,
    main_status_start_healthcare,
} main_status_e;



typedef enum
{
    factory_test_light_leak,
    factory_test_ir_led_golden_only,
    factory_test_ir_led_target_sample,
    factory_test_green_led_golden_only,
    factory_test_green_led_target_sample,
    factory_test_led_golden_only,
    factory_test_led_target_sample,
    factory_test_num,
} factory_test_e;


typedef struct
{
    // status
    main_status_e       status;

    // pxialg
    void*                pxialg_buffer;
    pah8series_data_t    pxialg_data;
    bool                pxialg_has_init;
    uint64_t            last_report_time;
    
    // gsensor
#if defined(ENABLE_MEMS_ZERO)
    int16_t                mems_data[MAX_MEMS_SAMPLE_NUM * 3];
    
#else
    int16_t                mems_data[MAX_MEMS_SAMPLE_NUM * 3];
#endif
    
} main_state_s;



pah8011Data_t pah8011Data;


/*============================================================================
STATIC VARIABLES
============================================================================*/
static main_state_s _state;

static bool need_log_header = true;

static volatile bool        has_interrupt_button = false;

static volatile bool        has_interrupt_pah = false;
static volatile uint64_t    interrupt_pah_timestamp = 0;

static uint32_t Expo_time_backup[3]={0};
static uint8_t LEDDAC_backup[3]={0};

//uint8_t PAH8011_I2C_SLAVE_ADDRESS;
uint8_t pah8011I2cAddress;

///*============================================================================
//STATIC FUNCTION PROTOTYPES
//============================================================================*/


static void start_healthcare_ppg(void);
static void start_healthcare_ppg_touch(void);
static void start_healthcare_touch_only(void);
static void stop_healthcare(void);

static void report_fifo_data(uint64_t timestamp, uint8_t *fifo_data, uint32_t fifo_data_num_per_ch, uint32_t ch_num, bool is_touched);

static void log_pah8series_data_header(uint32_t ch_num, uint32_t ir_ch_num, uint32_t g_sensor_mode);
static void log_pah8series_data(const pah8series_data_t *data);
static void log_pah8series_no_touch(void);

static bool hr_algorithm_calculate(pah8series_data_t *pxialg_data, uint32_t ch_num);
static void hr_algorithm_close(void);

////static void demo_factory_test(void);
static void pah8011_ae_info_check(void);
//static void gpiote_interrupt_config(void);

static void Error_Handler(void);
//void demo_factory_mode(void);
void demo_ppg_dri(void);
float findMedian(float *data, u8 size);
void tim_deinit(void);
//tim

uint64_t ucstamp;
uint64_t sample_time;
uint8_t time_int_flag;

/*
功能：时间戳
*/
uint32_t get_tick_count(void)
{
	return  ucstamp;
}

/*
功能：中断，获取滴答时钟
*/
void onDataReadyEvent(void)
{
	interrupt_pah_timestamp = get_tick_count();
	pah_ret ret = pah_err_unknown;
	if (_state.status == main_status_start_healthcare)
	{
		ret = pah_task();
		if (ret == pah_success)
		{
				if (pah_is_ppg_mode() )
				{
						uint8_t ppg_mode_flag =0 ;
				#ifdef PPG_MODE_ONLY
						ppg_mode_flag = 1 ;
				#endif
						bool is_touched = pah_is_touched();
						if (is_touched || ppg_mode_flag)
						{
								uint8_t *fifo_data = pah_get_fifo_data();
								uint32_t fifo_data_num_per_ch = pah_fifo_data_num_per_ch();
								uint32_t fifo_ch_num = pah_get_fifo_ch_num();
								report_fifo_data(interrupt_pah_timestamp, fifo_data, fifo_data_num_per_ch, fifo_ch_num, is_touched);
						}
						else
						{
								// log
								log_pah8series_no_touch();
								hr_algorithm_close();
								start_healthcare_touch_only();
						}
				
				}
				else if (pah_touch_mode == pah_query_mode())
				{
						if (pah_is_touched())
								start_healthcare_ppg_touch();
				}
		}
		else if (ret == pah_pending)
		{
				// ignore
		}
		else
		{
				debug_printf("pah_task fail, ret = %d \n", ret);
				Error_Handler();
		}		
		if (has_interrupt_button)
		{
				has_interrupt_button = false;
				
				// log
				log_pah8series_no_touch();
				hr_algorithm_close();
				
				stop_healthcare();
				_state.status = main_status_idle;
		}
	}
	else if (_state.status == main_status_idle)
	{
			if (has_interrupt_button)
			{
					has_interrupt_button = false;
					
			#if defined(PPG_MODE_ONLY)
					start_healthcare_ppg();
			#else
					start_healthcare_touch_only();
			#endif
					_state.status = main_status_start_healthcare;
			}
	}	


}

/*
功能：滴答时钟,51822没有滴答时钟，用定时器代替
*/
static void onTIMER0Event()
{
	ucstamp++;
	if(ucstamp == sample_time)
	{
		tim_deinit();
		pah8011Data.close_flag = 1;
	}
//	if(1000 == ucstamp)
//	{
//		ucstamp = 0;
//		Sensor_timeout();
//	}

}
/*
功能：data准备好，中断引脚配置
*/
static void pah8011_interrupt_config(void)
{
	//配置PAH8011ET int1引脚，Data Ready
//		
	NRF_GPIO->PIN_CNF[PPG_INT_PIN_NUM]=0x020000;//high level
	NRF_GPIOTE->EVENTS_PORT=0UL;
	NRF_GPIOTE->INTENSET=0x80000000;	//PORT
	NVIC_SetPriority(GPIOTE_IRQn, PORT_PRIORITY);
	NVIC_EnableIRQ(GPIOTE_IRQn); 
	
//	NRF_GPIOTE->CONFIG[0] =  (GPIOTE_CONFIG_POLARITY_LoToHi << GPIOTE_CONFIG_POLARITY_Pos)
//														 | (PPG_INT_PIN_NUM<< GPIOTE_CONFIG_PSEL_Pos)  
//														 | (GPIOTE_CONFIG_MODE_Event << GPIOTE_CONFIG_MODE_Pos);
//	NRF_GPIOTE->EVENTS_IN[0] = 0; 
//	NRF_GPIOTE->INTENSET  = GPIOTE_INTENSET_IN0_Set << GPIOTE_INTENSET_IN0_Pos;
//	NVIC_SetPriority(GPIOTE_IRQn, 1);
//	NVIC_EnableIRQ(GPIOTE_IRQn); 

}
/*
功能：不使能中断，低功耗模式
*/
static void pah8011_interrupt_deconfig(void)
{
	NRF_GPIOTE->EVENTS_PORT = 0;
	NRF_GPIOTE->INTENSET = GPIOTE_INTENSET_PORT_Disabled;		
	NVIC_DisableIRQ(GPIOTE_IRQn); /* disable change notification interrupts */ 
	NRF_GPIO->PIN_CNF[PPG_INT_PIN_NUM]=0x000002; /*disable input buffer*/   
}

/*
功能：pah8011初始化
*/
void pah8011_gpio_init(void)
{
	#ifdef __TFN108
	NRF_GPIO->PIN_CNF[PAH8011_Power_PIN_NUM] = (GPIO_PIN_CNF_SENSE_Disabled << GPIO_PIN_CNF_SENSE_Pos)
																				| (GPIO_PIN_CNF_DRIVE_S0D1 << GPIO_PIN_CNF_DRIVE_Pos)
																				| (GPIO_PIN_CNF_PULL_Disabled << GPIO_PIN_CNF_PULL_Pos)
																				| (GPIO_PIN_CNF_INPUT_Connect << GPIO_PIN_CNF_INPUT_Pos)
																				| (GPIO_PIN_CNF_DIR_Output << GPIO_PIN_CNF_DIR_Pos);
	PAH8011_Power_Down;//关闭传感器
	twi_master_deinit();
	Timer1_Init();
	#ifdef LOG_ON
		debug_log_init();
	#endif
	debug_printf("====RST \n");
//	NRF_GPIO->PIN_CNF[IR_INT_PIN_NUM]=0x000002;//high level
//	PAH8011_EXT_DS_DATA_READ_LowP();//低功耗
//	pah8011_interrupt_config();
	#else
	nrf_gpio_cfg_output(PAH8011_Power_PIN_NUM);//配置电源口
	PAH8011_Power_Down;//关闭传感器	
//	GPIO_QuickConfigOutput(PAH8011_PD_PIN_NUM);//
//	PAH8011_Power_Down;//关闭传感器
	#endif
}

/*
功能：pah8011电源控制
*/
static void pah8011_gpio_deinit(void)
{
	PAH8011_Power_Down;
	pah8011_interrupt_deconfig();
}

/*
功能：50ms定时初始化，PAH8011算法必须用
*/
void tim_start(void)
{
	NRF_TIMER1->TASKS_CLEAR = 1;
	NVIC_EnableIRQ(TIMER1_IRQn);
	TIMER_Start(NRF_TIMER1);
}


/*
功能：释放时钟
*/
void tim_deinit(void)
{
	TIMER_DeInit(NRF_TIMER1);
}
/*
功能：开启心率采集
*/
uint32_t coo;
uint8_t pah_start;
uint32_t hr_cnt;
extern void Osc_HFCLK(void);
void Osc_HFCLK_Off(void);
void pah8011_power_on(void)
{

		pah8011I2cAddress = pah_get_i2c_slave_addr()<<1;
		debug_printf("====TEST0 \n");
		PAH8011_Power_Up;//上电
		
		memset(&pah8011Data,0,sizeof(pah8011Data));//清空数据
		debug_printf("====TEST1 \n");
		delay_ms(20);
		twi_master_init();
	  pah_start = 1;
	#ifdef LOG_ON
		need_log_header = true;
	#endif
		ucstamp = 0;
		sample_time = SAMPLE_ON_TIME*1000;
//	accelerometer_init();
		coo = 0;
		debug_printf("====TEST2 \n");
//	  pah_deinit();
//	twi_master_deinit();
		Osc_HFCLK();//很重要，用来降低功耗
		memset(&_state,0,sizeof(_state));
//		_state.pxialg_data.frame_count = 0;
		demo_ppg_dri();
		pah8011_interrupt_config();
		debug_printf("====TEST3 \n");
		tim_start();
	
}
/*
功能：关闭心率采集
*/
extern uint8_t SensorValue;
extern uint32_t hr_cnt;
uint8_t hr_buf[2];
uint32_t hr_addr = 0x30000;
uint8_t hr;
void pah8011_power_off(void)
{
		#if 1
		pah_start = 0;
		Osc_HFCLK_Off();
		
	  if(_state.status == main_status_start_healthcare)
		{
			stop_healthcare();
			hr_algorithm_close();
		}
//		hr_cnt++;
//		hr = findMedian(pah8011Data.heart,pah8011Data.tid);
//		tim_deinit();
		twi_master_deinit();		
		pah8011_gpio_deinit();

//	  if(hr > 0)
//		{
//			SensorValue = hr;
//			hr_buf[0] = hr_cnt;
//			hr_buf[1] = SensorValue;
//			nrf_nvmc_write_bytes(hr_addr,hr_buf,2);
//			hr_addr = hr_addr +2;
//		}
//		

//	  accelerometer_stop();
//		hr_algorithm_close();
		#endif
//		tim_deinit();
//		pah8011_gpio_deinit();
////		
//		delay_ms(1000);
//		twi_master_deinit();

}

/*******************************************************************************
 * NOTE: as most this routine can process 127 data items
 */
float findMedian(float *data, u8 size)
{
    float bestValue;
    s8 bestDelta;

    u8 ctrVal;

    if (size > 127)
    {
        return 0;
    }

    bestValue = 0;
    bestDelta = 127;

    if (size> 10)
    {
			__nop();
			__nop();
			__nop();
    }

    for (ctrVal = 0; ctrVal < size; ctrVal++)
    {
        float currValue;
        s8 currOver;
        s8 currUnder;
        s8 currDelta;
        u8 ctrTest;

        currValue = data[ctrVal];
        currUnder = 0;
        currOver = 0;

        for (ctrTest = 0; ctrTest < size; ctrTest++)
        {
            if (data[ctrTest] < currValue) currUnder++;
            if (data[ctrTest] > currValue) currOver++;
        }

        currDelta = abs(currUnder - currOver);
        if (currDelta < bestDelta)
        {
            bestDelta = currDelta;
            bestValue = currValue;
        }
    }
    return bestValue;
}

/*
功能：io中断，端口中断更省电
*/

void GPIOTE_IRQHandler(void)
{
	// Event causing the interrupt must be cleared.
	
	if(NRF_GPIOTE->EVENTS_IN[0])
	{
		NRF_GPIOTE->EVENTS_IN[0] = 0; /* clear interrupt status */
		onDataReadyEvent();
		coo++;
	}
	else if(NRF_GPIOTE->EVENTS_PORT)
	{
		NRF_GPIOTE->EVENTS_PORT = 0;
		
		if(1 == Read_PPG)
		{
			coo++;
			onDataReadyEvent();
//			debug_printf("GPIO_S \n");
		}	
	}
	

}

void TIMER1_IRQHandler(void)
{
	if(NRF_TIMER1 ->EVENTS_COMPARE[1])
	{
		NRF_TIMER1 ->EVENTS_COMPARE[1] = 0;
		onTIMER0Event();
//		debug_printf("TIM_S \n");
	}
	
}
/*
功能：获取心率值
*/
u16 GetHeartValue(void)
{
	if(pah8011Data.has_updated)
	{
//		pah8011Data.has_updated = 0;
//		pah8011_power_off();
		return pah8011Data.hr;
	}
	else
		return 0;
}

#if 0
void demo_ppg(void)
{
	pah_ret ret = pah_err_unknown;
	if (!pah_init())
	{
			debug_printf("pah_init() fail. \n");
			Error_Handler();
	}
	if(!pah_enter_mode(pah_ppg_mode))
		Error_Handler();
	while(1)
	{
		if(has_interrupt_pah)
		{
			has_interrupt_pah = false;
			ret = pah_task();
			if(ret == pah_success)
			{
				if(pah_is_ppg_mode())
				{
					bool is_touched = pah_is_touched();
					uint8_t *fifo_data = pah_get_fifo_data();
					uint32_t fifo_data_num_per_ch = pah_fifo_data_num_per_ch();
					uint32_t fifo_ch_num = pah_get_fifo_ch_num();
					uint32_t *ppg_data = (uint32_t*)fifo_data;
					uint32_t i = 0,ch=0;
					for(i = 0;i<fifo_data_num_per_ch;++i)
					{
						debug_printf("i=%d,data={",i);
						for(ch = 0;ch<fifo_ch_num;++ch)
						{
							debug_printf("%d,",ppg_data[i*fifo_ch_num+ch]);
						}
						debug_printf("}\n");
					}
				}
			}
			else if(ret == pah_pending)
			{
				
			}
			else
			{
//				debug_printf();
				Error_Handler();
			}
		}
	}
	
}

#endif


#if 1
void demo_ppg_dri(void)
{
//    pah_ret ret = pah_err_unknown;
    
    // PAH
    if (!pah_init())
    {
        debug_printf("pah_init() fail. \n");
        Error_Handler();
    }
//		if (!accelerometer_init())
//        Error_Handler();
#if defined(PPG_MODE_ONLY)
    start_healthcare_ppg();
#else
    start_healthcare_touch_only();
#endif
    _state.status = main_status_start_healthcare;
}
#endif
#if 0


void demo_factory_mode(void)
{
    
#ifdef FACTORY_TEST_ES
    
    factory_test_mode(factory_test_led_golden_only,1,0x42,0x42);
//factory_test_mode(factory_test_led_golden_only,Enable,expo_ch_b,expo_ch_c);
    //Customer can turn expo time to Get ADC_Data 2000
    factory_test_mode(factory_test_led_target_sample,0,0,0);
//Get Correct value for the expo_ch_b,expo_ch_c
    //Please Modify the pah_verify_8011_reg.h pah8011_verify_led_brightness_array[][2] array
    factory_test_mode(factory_test_light_leak,0,0,0);
    
#endif
#ifdef FACTORY_TEST_ET
    factory_test_mode(factory_test_led_golden_only,1,0x32,0x32);
    //factory_test_mode(factory_test_green_led_golden_only,Enable,expo_ch_b,expo_ch_c);
    //Customer can turn expo time to Get ADC_Data 2000

    factory_test_mode(factory_test_led_target_sample,0,0,0);
    //Get Correct value for the expo_ch_b,expo_ch_c
    //Please Modify the pah_verify_8011_reg.h pah8011_verify_led_brightness_array[][2] array
    factory_test_mode(factory_test_light_leak,0,0,0);
#endif
    
}
#endif

static void start_healthcare_ppg(void)
{
    // gsensor
  #ifndef ENABLE_MEMS_ZERO  
    accelerometer_start();
  #endif  
    // PAH
    if (!pah_enter_mode(pah_ppg_mode))
        Error_Handler();
    
    _state.last_report_time = get_tick_count();
}

static void start_healthcare_ppg_touch(void)
{
    // gsensor
#ifndef ENABLE_MEMS_ZERO 
    accelerometer_start();
#endif 
    // PAH
    if (!pah_enter_mode(pah_ppg_touch_mode))
        Error_Handler();
    
    _state.last_report_time = get_tick_count();
}

static void start_healthcare_touch_only(void)
{
    // gsensor
#ifndef ENABLE_MEMS_ZERO 
    accelerometer_stop();
#endif 
    
    // PAH
    if (!pah_enter_mode(pah_touch_mode))
        Error_Handler();
}

static void stop_healthcare(void)
{
    // gsensor
#ifndef ENABLE_MEMS_ZERO 
    if(accelerometer_stop()!=1) 
		{
			Error_Handler();
		}
#endif     
    // PAH
    if (!pah_enter_mode(pah_stop_mode))
        Error_Handler();
}

static void report_fifo_data(uint64_t timestamp, uint8_t *fifo_data, uint32_t fifo_data_num_per_ch, uint32_t ch_num, bool is_touched)
{
    // gsensor
#if defined(ENABLE_MEMS_ZERO)
    _state.pxialg_data.mems_data = _state.mems_data;
    _state.pxialg_data.nf_mems = fifo_data_num_per_ch;
#else
    accelerometer_get_fifo(&_state.pxialg_data.mems_data, &_state.pxialg_data.nf_mems);
#endif  // ENABLE_MEMS_ZERO
        
    _state.pxialg_data.touch_flag = is_touched ? 1 : 0;
    _state.pxialg_data.time = (uint32_t)(timestamp - _state.last_report_time);
    _state.pxialg_data.ppg_data = (int32_t*)fifo_data;
    _state.pxialg_data.nf_ppg_channel = ch_num;
    _state.pxialg_data.nf_ppg_per_channel = fifo_data_num_per_ch;
    ++_state.pxialg_data.frame_count;
    
    _state.last_report_time = timestamp;
#ifdef Timing_Tuning    
    pah_do_timing_tuning(_state.pxialg_data.time, _state.pxialg_data.nf_ppg_per_channel);
#endif
    // log header
    if (need_log_header)
    {
        need_log_header = false;
        
        log_pah8series_data_header(ch_num, PPG_IR_CH_NUM, ALG_GSENSOR_MODE);
    }
    
    // log
    log_pah8series_data(&_state.pxialg_data);
        pah8011_ae_info_check();
    
    // hr
    hr_algorithm_calculate(&_state.pxialg_data, ch_num);
}

static void log_pah8series_data_header(uint32_t ch_num, uint32_t ir_ch_num, uint32_t g_sensor_mode)
{
    //log pah8series data header
    // (1)Using total channel numbers;
    // (2)reserved;
    // (3)reserved;
    // (4)IR channel number;
    // (5)MEMS mode 0:2G, 1:4G, 2:8G
    log_printf("PPG CH#, %d, %d, %d, %d, %d\n", ch_num, pah8011_setting_version(), 0, ir_ch_num, g_sensor_mode);
}

static void log_pah8series_data(const pah8series_data_t *pxialg_data)
{
    int i = 0;
    uint32_t *ppg_data = (uint32_t *)pxialg_data->ppg_data;
    int16_t *mems_data = pxialg_data->mems_data;
    int data_num = pxialg_data->nf_ppg_channel * pxialg_data->nf_ppg_per_channel;
    
    log_printf("Frame Count, %d \n", pxialg_data->frame_count);
    log_printf("Time, %d \n", pxialg_data->time);
    log_printf("PPG, %d, %d, ", pxialg_data->touch_flag, data_num);
    for (i = 0; i < data_num; ++i)
    {
        log_printf("%d, ", *ppg_data);
        ppg_data++;
    }
    log_printf("\n");
    log_printf("MEMS, %d, ", pxialg_data->nf_mems);
    for (i = 0; i < pxialg_data->nf_mems * 3; ++i)
    {
        log_printf("%d, ", *mems_data);
        mems_data++;
    }
    log_printf("\n");
}

static void log_pah8series_no_touch(void)
{
    static const int DUMMY_PPG_DATA_NUM = 20;
    static const int DUMMY_PPG_CH_NUM = 2;
    int i = 0;
    
    log_printf("Frame Count, 0 \n");
    log_printf("Time, %d \n", DUMMY_PPG_DATA_NUM * 50); // 20Hz = 50ms
    log_printf("PPG, 0, %d, ", DUMMY_PPG_DATA_NUM * DUMMY_PPG_CH_NUM);
    for (i = 0; i < DUMMY_PPG_DATA_NUM * DUMMY_PPG_CH_NUM; i++)
    {
        log_printf("0, ");
    }
    log_printf("\n");
    log_printf("MEMS, %d, ", DUMMY_PPG_DATA_NUM);
    for (i = 0; i < DUMMY_PPG_DATA_NUM * 3; ++i)
    {
        log_printf("0, ");
    }
    log_printf("\n");
}






static bool hr_algorithm_calculate(pah8series_data_t *pxialg_data, uint32_t ch_num)
{
//    pah8011Data.has_updated = false;
    
#if defined(ENABLE_PXI_ALG)
    uint8_t ret = 0;
    
    // Algorithm support only data length >= 10
    if (pxialg_data->nf_ppg_per_channel < 10)
    {
        debug_log("hr_algorithm_calculate(). fifo_data_num_per_ch = %d, is not enough to run algorithm \n", pxialg_data->nf_ppg_per_channel);
        return false;
    }
    
    // Initialize algorithm
    if (!_state.pxialg_has_init)
    {
        uint32_t pxialg_open_size = 0;
    
        pxialg_open_size = pah8series_query_open_size();
        debug_printf("pah8series_query_open_size() = %d \n", pxialg_open_size);
        
        _state.pxialg_buffer = (void*)malloc(pxialg_open_size);
        ret = pah8series_open(_state.pxialg_buffer);
        if (ret != MSG_SUCCESS)
        {
            debug_printf("pah8series_open() failed, ret = %d \n", ret);
            return false;
        }

				uint32_t pah_ver = pah8series_version();
        debug_printf("pah8series_version = %d \n", pah8series_version());
        
        pah8series_set_param(PAH8SERIES_PARAM_IDX_PPG_CH_NUM, (float)ch_num);
        pah8series_set_param(PAH8SERIES_PARAM_IDX_HAS_IR_CH, (float)PPG_IR_CH_NUM);
        pah8series_set_param(PAH8SERIES_PARAM_IDX_GSENSOR_MODE, (float)ALG_GSENSOR_MODE);

        _state.pxialg_has_init = true;
    }
    
    // Calculate Heart Rate
    pah8011Data.return_value = pah8series_entrance(pxialg_data);
    if (pah8011Data.return_value == MSG_HR_READY)
    {
//        float hr = 0.0f;
//        int hr_trust_level = 0;
//        int16_t grade = 0;
//        
//        pah8series_get_hr(&hr);
//        pah8series_get_hr_trust_level(&hr_trust_level);
//        pah8series_get_signal_grade(&grade);
//        
//        debug_log("hr = %d, hr_trust_level = %d, grade = %d \n", (int)hr, hr_trust_level, grade);
			
			  pah8series_get_hr(&pah8011Data.hr);
        pah8series_get_hr_trust_level(&pah8011Data.hr_trust_level);
        pah8series_get_signal_grade(&pah8011Data.grade);
        pah8011Data.heart[pah8011Data.tid] = pah8011Data.hr;
				pah8011Data.tid++;
        debug_printf("hr = %d, hr_trust_level = %d, grade = %d \n", (int)pah8011Data.hr, pah8011Data.hr_trust_level, pah8011Data.grade);			
        pah8011Data.has_updated = true;
    }
    else
    {
        switch (pah8011Data.return_value)
        {
            case MSG_SUCCESS:
                debug_printf("Algorithm entrance success. \n");
                break;
            case MSG_ALG_NOT_OPEN:
                debug_printf("Algorithm is not initialized. \n");
                break;
            case MSG_MEMS_LEN_TOO_SHORT:
                debug_printf("MEMS data length is shorter than PPG data length. \n");
                break;
            case MSG_NO_TOUCH:
                debug_printf("PPG is no touch. \n");
                break;
            case MSG_PPG_LEN_TOO_SHORT:
                debug_printf("PPG data length is too short. \n");
                break;
            case MSG_FRAME_LOSS:
                debug_printf("Frame count is not continuous. \n");
                break;
            default:
                debug_printf("Algorithm unhandle error = %d \n", ret);
                break;
        }
    }
        
#endif // ENABLE_PXI_ALG
    
    return pah8011Data.has_updated;
}

static void hr_algorithm_close(void)
{
#if defined(ENABLE_PXI_ALG)
    if (_state.pxialg_has_init)
    {
        _state.pxialg_has_init = false;
        
        pah8series_close();
        free(_state.pxialg_buffer);
    }
#endif // ENABLE_PXI_ALG
}



static void pah8011_ae_info_check(void)
{
    uint8_t i;
    float VAR_MAX=0;
    float AE_VAR=0;
    uint32_t Expo_time[3]={0};
    uint8_t LEDDAC[3];
    pah8011_ae_info_read( Expo_time, LEDDAC);
    for( i = 0 ; i < 3 ; i++)
    {
        if(Expo_time_backup[i]>0)
        {
            AE_VAR= ((float)Expo_time[i]-(float)Expo_time_backup[i])/(float)Expo_time_backup[i];
            AE_VAR = (AE_VAR >= 0.0) ? AE_VAR : AE_VAR*(-1); 
            VAR_MAX = (AE_VAR >= VAR_MAX) ? AE_VAR : VAR_MAX;
        }
        Expo_time_backup[i] = Expo_time[i] ;
    }
    for( i = 0 ; i < 3 ; i++)
    {
        if(LEDDAC_backup[i]>0)
        {
            AE_VAR= ((float)LEDDAC[i]-(float)LEDDAC_backup[i])/(float)LEDDAC_backup[i];
            AE_VAR = (AE_VAR >= 0.0) ? AE_VAR : AE_VAR*(-1); 
            VAR_MAX = (AE_VAR >= VAR_MAX) ? AE_VAR : VAR_MAX; 
        }
        LEDDAC_backup[i] = LEDDAC[i];
    }
    
    debug_printf("INFO, %d, %d, %d, %d, %d, %d ,VAR, %f \n", Expo_time[0],Expo_time[1],Expo_time[2],LEDDAC[0],LEDDAC[1],LEDDAC[2],VAR_MAX);
    
}


uint32_t error_i = 0;
static void Error_Handler(void)
{

	error_i++;
//	tim_deinit();
}
