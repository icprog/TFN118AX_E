#ifndef APP_SENSOR
#define APP_SENSOR
//app_sensor.h
//#define SAMPLE_ON_TIME												5	//采样持续时间，单位1s，14s  //10
//#define SAMPLE_MIN_TIME												40	//采样最小周期时间，单位125ms，10s->15     //120
#define SAMPLE_ON_TIME												10	//采样持续时间，单位1s，14s  //10
#define SAMPLE_MIN_TIME												120	//采样最小周期时间，单位125ms，10s->15     //120
#define RATE_BUF_LEN													35	//心率缓存长度，为心率个数+1
#define NB_LEN	5 //邻域长度

#define HEARTRATE_MAX													200
#define HEARTRATE_MIN													40

void Sensor_timeout(void);//采样超时

//FILE_END
#endif
