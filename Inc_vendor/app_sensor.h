#ifndef APP_SENSOR
#define APP_SENSOR
//app_sensor.h
//#define SAMPLE_ON_TIME												5	//��������ʱ�䣬��λ1s��14s  //10
//#define SAMPLE_MIN_TIME												40	//������С����ʱ�䣬��λ125ms��10s->15     //120
#define SAMPLE_ON_TIME												10	//��������ʱ�䣬��λ1s��14s  //10
#define SAMPLE_MIN_TIME												120	//������С����ʱ�䣬��λ125ms��10s->15     //120
#define RATE_BUF_LEN													35	//���ʻ��泤�ȣ�Ϊ���ʸ���+1
#define NB_LEN	5 //���򳤶�

#define HEARTRATE_MAX													200
#define HEARTRATE_MIN													40

void Sensor_timeout(void);//������ʱ

//FILE_END
#endif
