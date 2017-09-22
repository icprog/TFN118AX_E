//app_var.h
#include "stdint.h"
#include "stdbool.h"
#define RAM_PARA_LEN										256
#define RAM_RESEVER_LEN									256
#define RAM_USER1_LEN										512
#define RAM_USER2_LEN										512

#define RAM_PARA												PARA_BEGIN
#define RAM_RESEVER											(RAM_PARA + 1024)
#define RAM_USER1												(RAM_RESEVER + 1024)
#define RAM_USER2												(RAM_USER1 + 1024)

#define RECORD_LEN											16
//内部FLASH区类型
#define RAMTYPE_PARA										0
#define RAMTYPE_RESEVER									1
#define RAMTYPE_USER1										2
#define RAMTYPE_USER2										3

//射频频道和方向选择
#define CHANNEL_DATA										0
#define CHANNEL_CONFIG									1
#define RADIO_RX												0
#define RADIO_TX												1
//默认射频频道
#define DEFAULT_CHANNEL_DATA						23
#define DEFAULT_CHANNEL_CONFIG					7

/*******************************************
							内部参数位置
*******************************************/
// #define UNDEFINE												0

// #define PARA_ADD_IDX										0//5字节射频地址
// #define PARA_DATACHANNEL_IDX						5//数据频道
// #define PARA_CONFIGCHANNEL_IDX					6//配置频道
// #define PARA_BRIEFNUM_IDX								7//短号
// #define PARA_PWR_IDX										8//发射功率
// #define PARA_RPINFOSRC_IDX							8//自动上报所需携带的信息来源
// #define PARA_PRIODSEND_IDX							8//使能周期发送
// #define PARA_PRIO_IDX										9//定位优先级
// #define PARA_FIX_IDX										9//固定
// #define PARA_TAGID_IDX									9//输出标签的ID类型：0-工厂，1-用户自定义
// #define PARA_MSGTYPE_IDX								9//输出标签信息类型：0-短信息，1-长信息
// #define PARA_NOLINKTEST_IDX							10//关闭连接检测，0-检查连接，1关闭检测连接
// #define PARA_RPTRGTYPE_IDX							10//触发方式：0-变动触发，1-定时触发
// #define PARA_INTERVAL_IDX								10//心跳间隔/定时间隔，单位2S：0-无心跳/定时，最大间隔120S
// #define PARA_RSSI_IDX										11//RSSI门限
// #define PARA_DELIVERID_IDX							12//工厂ID，4B，0x00000000-待写入
/*内部参数取值*/
//发射功率，0～7；0--30dbm，1--20dbm，2--16dbm,3--12dbm,4--8dbm,5--4dbm,6--0dbm,7-+4dbm
// #define PARA_PWR_Pos										4
// #define PARA_PWR_Msk										0xF0
#define PARA_PWR_N30DBM									0
#define PARA_PWR_N20DBM									1
#define PARA_PWR_N16DBM									2
#define PARA_PWR_N12DBM									3
#define PARA_PWR_N8DBM									4
#define PARA_PWR_N4DBM									5
#define PARA_PWR_P0DBM									6
#define PARA_PWR_P4DBM									7
// //自动上报携带的信息来源，0～2；0-保留区，1-用户区1，2-用户区2
// #define PARA_RPINFOSRC_Pos							2
// #define PARA_RPINFOSRC_Msk							0x0C
// #define PARA_RPINFOSRC_RESEVER					0
// #define PARA_RPINFOSRC_USER1						1
// #define PARA_RPINFOSRC_USER2						2
// //使能周期发送，1-使能
// #define PARA_PRIODSEND_Pos							0
// #define PARA_PRIODSEND_Msk							0x03
// //定位优先级，0～3；0-不参与定位，1～3数值越大定位优先级越高
// #define PARA_PRIO_Pos										6
// #define PARA_PRIO_Msk										0xC0
// //固定值，1
// #define PARA_FIX_Pos										5
// #define PARA_FIX_Msk										0x20
// //输出标签ID类型，0/1；0-工厂ID，1-用户自定义ID
// #define PARA_TAGID_Pos									4
// #define PARA_TAGID_Msk									0x10
// #define PARA_TAGID_DELIVERID						0
// #define PARA_TAGID_SELFDEFINEID					1
// //输出标签信息类型：0-短信息，1-长信息
// #define PARA_MSGTYPE_Pos								3
// #define PARA_MSGTYPE_Msk								0x08
// #define PARA_MSGTYPE_LONG								(1 << PARA_MSGTYPE_Pos)
// #define PARA_MSGTYPE_SHORT							(0 << PARA_MSGTYPE_Pos)
// //关闭连接检测，0/1；-0-检测连接，1-关闭检测连接
// #define PARA_NOLINKTEST_Pos							7
// #define PARA_NOLINKTEST_Msk							0x80
// #define PARA_NOLINKTEST_TEST						(0 << PARA_NOLINKTEST_Pos)
// #define PARA_NOLINKTEST_NOTEST					(1 << PARA_NOLINKTEST_Pos)
// //触发输出，0/1；0-变动触发；1定时触发
// #define PARA_RPTRGTYPE_Pos							6
// #define PARA_RPTRGTYPE_Msk							0x40
// #define PARA_RPTRGTYPE_VARTRG						(0 << PARA_RPTRGTYPE_Pos)
// #define PARA_RPTRGTYPE_CYCLETRG					(1 << PARA_RPTRGTYPE_Pos)
// //心跳间隔/定时间隔，0～60，单位2S：0-无心跳/无定时输出，最大价格120S
// #define PARA_INTERVAL_Pos										0
// #define PARA_INTERVAL_Msk										0x3F
// //RSSI门限，0～0x7F
// #define PARA_RSSI_Pos										0
// #define PARA_RSSI_Msk										0x7F

/********************************************
						读写器下发(下发指令/回复)
********************************************/
//下发指令索引
#define DOWN_TYPE_IDX										1
#define DOWN_DESTID_IDX									2
#define DOWN_BRIEFNUM_IDX								7//短号0～15

//取值
//包类型，8/10；8-常规信息，10-携带指令
#define DOWN_TYPE_Pos										0
#define DOWN_TYPE_Msk										0x0F

#define DOWN_WITH_CMD										10
/********************************************
				读写器上传（自动上报/回复）
********************************************/
//上报指令索引
#define UP_FIX_IDX											1
#define UP_TYPE_IDX											1
#define UP_READERID_IDX									2
#define UP_RXWIN_IDX										6
#define UP_BRIEFNUM_IDX									7
#define UP_RXINDIC_IDX									12
#define UP_RXRPD_IDX										12
#define UP_EXTCMD												12
//取值
#define UP_FIX_Pos											4
#define UP_FIX_Msk											0xF0
#define UP_FIX_VAL											0
//数据类型，0/2；0-常规信息，2-携带指令
#define UP_TYPE_Pos											0
#define UP_TYPE_Msk											0x0F
#define UP_TYPE_INFO										(0<<UP_TYPE_Pos)
#define UP_TYPE_EXTCMD									(2<<UP_TYPE_Pos)
//接收窗口指示，0/1；1-携带接收窗
#define UP_RXWIN_Pos										7
#define UP_RXWIN_Msk										0x80
//收到指令指示 1-收到，0-未收到
#define UP_RXINDIC_Pos									7
#define UP_RXINDIC_Msk									0x80
#define UP_RXINDIC_VALID								(1 << UP_RXINDIC_Pos)
//收到指令RPD有效指示，1-RPD有效
#define UP_RXRPD_Pos										6
#define UP_RXRPD_Msk										0x40
#define UP_RXRPD_VALID									(1 << UP_RXRPD_Pos)

/********************************************
									扩展信息定义
********************************************/
#define EXTINFO_LEN											20
#define EXTINFO_BASE_IDX								12
#define EXTINFO_CMD_IDX									(EXTINFO_BASE_IDX + 0)//指令
#define EXTINFO_FLSPG_IDX								(EXTINFO_BASE_IDX + 1)//内部Flash页地址，针对指令0，1，2
#define EXTINFO_RDVLD_IDX								(EXTINFO_BASE_IDX + 1)//记录偏移有效，针对指令0，1
#define EXTINFO_RDOS_IDX								(EXTINFO_BASE_IDX + 1)//记录偏移，针对指令0，1
#define EXTINFO_CODENUM_IDX							(EXTINFO_BASE_IDX + 1)//包编号，针对指令6
#define EXTINFO_DATA_IDX								(EXTINFO_BASE_IDX + 4)//16字节读写内容

#define EXTINFO_CMD_IDX2								(0)//指令
#define EXTINFO_FLSPG_IDX2							(1)//内部Flash页地址，针对指令0，1，2
#define EXTINFO_RDVLD_IDX2							(1)//记录偏移有效，针对指令0，1
#define EXTINFO_RDOS_IDX2								(1)//记录偏移，针对指令0，1
#define EXTINFO_CODENUM_IDX2						(1)//包编号，针对指令6
#define EXTINFO_DATA_IDX2								(4)//16字节读写内容
//扩展信息取值
#define EXTINFO_CMD_Pos									0
#define EXTINFO_CMD_Msk									0x0F
#define EXTINFO_CMD_READ								0
#define EXTINFO_CMD_WRITE								1
#define EXTINFO_CMD_ERASE								2
#define EXTINFO_CMD_READCURRAM					4//读运行参数
#define EXTINFO_CMD_WRITECURRAM					5//写运行参数
#define EXTINFO_CMD_RESUME							10//唤醒
#define EXTINFO_CMD_SLEEP								11//休眠

//内部Flash页地址，0-内部参数区，1-保留区，2-用户区1，3-用户区2
#define EXTINFO_FLSPG_Pos								6
#define EXTINFO_FLSPG_Msk								0xC0
#define EXTINFO_FLSPG_INNERPARA					0
#define EXTINFO_FLSPG_RESERVER					1
#define EXTINFO_FLSPG_USER1							2
#define EXTINFO_FLSPG_USER2							3
//记录偏移有效性，1-有效
#define EXTINFO_RDVLD_Pos								5
#define EXTINFO_RDVLD_Msk								0x20
#define EXTINFO_RDVLD_VALID							1
//记录偏移，0~31页地址0和1各16条记录，页地址2和3各32条记录
#define EXTINFO_RDOS_Pos								0
#define EXTINFO_RDOS_Msk								0x1F
//包编号，0~21每个包16字节，21个包共336字节，传送内部固定RAM块内容
#define EXTINFO_CODENUM_Pos							0
#define EXTINFO_CODENUM_Msk							0xFF

/********************************************
							UART上报的标签信息定义
********************************************/
#define SR_SOH_IDX											0//6B固定值，0xFFFFFFFFFF00
#define SR_LEN_IDX											6//2B
#define SR_ADDR_IDX											8//4B
#define SR_ATTR_IDX											12//2B
#define SR_TYPE_IDX											14//1B
#define SR_INFO_IDX											15//xB
//#define SR_XOR_IDX										16//1B

#define TAGMSG_SHORT_ID_IDX							0//4B
#define TAGMSG_SHORT_STAT_IDX						4//1B
#define TAGMSG_SHORT_REF_IDX						5//1B,传感器数值，位置信息/传感数据，0xff无效值
#define TAGMSG_LONG_SELFDEFINE					6//10B,用户自定义

#define TAGMSG_SHORT_ISMVLD_Pos					7
#define TAGMSG_SHORT_ISMVLD_Msk					0x80
#define TAGMSG_SHORT_ERRVLD_Pos					3
#define TAGMSG_SHORT_ERRVLD_Msk					0x08
#define TAGMSG_SHORT_KEY_Pos						2
#define TAGMSG_SHORT_KEY_Msk						0x04
#define TAGMSG_SHORT_SHORK_Pos					1
#define TAGMSG_SHORT_SHORK_Msk					0x02
#define TAGMSG_SHORT_LOWVOLTAG_Pos			0
#define TAGMSG_SHORT_LOWVOLTAG_Msk			0x01
/********************************************
							UART下发接口定义
********************************************/
#define UART_LEN_IDX										0
#define UART_ADDR_IDX										2
#define UART_ATTR_IDX										6
#define UART_TYPE_IDX										8
#define UART_INFO_IDX										9

//取值定义
#define UART_TYPE_SENDMODE_Pos					7
#define UART_TYPE_SENDMODE_Msk					0x80
#define UART_TYPE_OVERTIME_Pos					4
#define UART_TYPE_OVERTIME_Msk					0x70
#define UART_TYPE_DIR_Pos								3
#define UART_TYPE_DIR_Msk								0x08
#define UART_TYPE_INFO_Pos							1
#define UART_TYPE_INFO_Msk							0x02
#define UART_TYPE_CMDTYPE_Pos						0
#define UART_TYPE_CMDTYPE_Msk						0x01
/********************************************
									ID号段定义
********************************************/
#define ID_TAP_REGION_First							0x00000001//标签ID
#define ID_TAP_REGION_Last							0xFEFFFFFF
#define ID_RECEIVER_REGION_First				0xFF000000//接收器/读写器ID
#define ID_RECEIVER_REGION_Last					0xFFFDFFFF
#define ID_TRANSCEIVER_REGION_First			0xFFFE0000//做发卡器用(桌面)
#define ID_TRANSCEIVER_REGION_Last			0xFFFEFFFF
#define ID_RESERVER_REGION_First				0xFFFF0000//保留
#define ID_RESERVER_REGION_Last					0xFFFFFFFA
#define ID_SELF_RP											0xFFFFFFFB//本机ID，用于访问自身参数，使能回复(只在UART端出现)
#define ID_BC_TAP_RP										0xFFFFFFFC//广播ID，目标所有标签，使能回复
#define ID_BC_TAP_NRP										0xFFFFFFFD//广播ID，目标所有标签，禁止回复
#define ID_BROADCAST_READER_RP					0xFFFFFFFE//广播ID，目标所有接收器，使能回复
#define ID_RESERVER1										0xFFFFFFFF//保留
#define ID_RESERVER0										0x00000000//保留

#define ID_TYPE_TAP											0
#define ID_TYPE_RECEIVER								1
#define ID_TYPE_TRANSCEIVER							2
#define ID_TYPE_ERROR										4
#define ID_TYPE_SELF_RP									5
#define ID_TYPE_ALLTAP_RP								6
#define ID_TYPE_ALLTAP_NRP							7
#define ID_TYPE_ALLRECEIVER_RP					8
/********************************************
					标签内部参数
********************************************/
#define TAGP_ADD_IDX										0//5字节射频地址
#define TAGP_DATACHANNEL_IDX						5//数据频道
#define TAGP_CONFIGCHANNEL_IDX					6//配置频道
#define TAGP_BRIEFNUM_IDX								7//短号，0～16
#define TAGP_PWR_IDX										8//发射功率，0～7
#define TAGP_RPINFOSRC_IDX							8//自动上报所需携带的信息来源，0～2
#define TAGP_WORKMODE_IDX								8//工作模式，0-保存模式，1-活动模式
#define TAGP_DELIVERID_IDX							9//工厂ID，4B，0xFFFFFFFF-待写入
#define TAGP_SENSORTYPE_IDX							13//传感类型，0-无，1-温度，2-心率，0～2
#define TAGP_SSRUNIT_IDX								14//传感采样周期单位，0-秒，1-分，2-时
#define TAGP_SSRVALUE_IDX								14
/*内部参数取值*/
//发射功率，0～7；0--30dbm，1--20dbm，2--16dbm,3--12dbm,4--8dbm,5--4dbm,6--0dbm,7-+4dbm
#define TAGP_PWR_Pos										4
#define TAGP_PWR_Msk										0xF0
#define TAGP_PWR_N30DBM									0
#define TAGP_PWR_N20DBM									1
#define TAGP_PWR_N16DBM									2
#define TAGP_PWR_N12DBM									3
#define TAGP_PWR_N8DBM									4
#define TAGP_PWR_N4DBM									5
#define TAGP_PWR_P0DBM									6
#define TAGP_PWR_P4DBM									7
//自动上报携带的信息来源，0～2；0-保留区，1-用户区1，2-用户区2
#define TAGP_RPINFOSRC_Pos							2
#define TAGP_RPINFOSRC_Msk							0x0C
#define TAGP_RPINFOSRC_RESEVER					0
#define TAGP_RPINFOSRC_USER1						1
#define TAGP_RPINFOSRC_USER2						2
//工作模式,0-保存模式，1-活动模式
#define TAGP_WORKMODE_Pos								0
#define TAGP_WORKMODE_Msk								0x03
//传感类型，0-无，1-温度，2-心率
#define TAGP_SENSORTYPE_Pos							4
#define TAGP_SENSORTYPE_Msk							0xF0
//传感参数，单位
#define TAGP_SSRUNIT_Pos								6
#define TAGP_SSRUNIT_Msk								0xC0
//传感参数，值,0~60,0-关闭
#define TAGP_SSRVALUE_Pos								0
#define TAGP_SSRVALUE_Msk								0x3F
/********************************************
							标签上报参数定义
********************************************/
#define TAG_SER_IDX											1//顺序号
#define TAG_TYPE_IDX										1//信息标签类型
#define TAG_ID_IDX											2//标签ID
#define TAG_WINDOWS_IDX									6//接收窗口指示
#define TAG_JOINTYPE_IDX								6//静态接入指示
#define TAG_ERROR_IDX										6//数据异常指示
#define TAG_KEY_IDX											6//按键指示
#define TAG_SHOCK_IDX										6//振动指示
#define TAG_LOWVOLTAGE_IDX							6//低电标志
#define TAG_LOWPWR_IDX									6//仓储模式
#define TAG_LOCATION_IDX								7//标签定位，所属激励器
#define TAG_SENSORTYPE_IDX							8//传感器类型
#define TAG_VALUE_IDX										9//传感器数据
#define TAG_UNIT_IDX										10//传感器参数-采样单位
#define TAG_UNIT_VALUE_IDX							10//传感器参数-采样周期，0-关闭传感器
#define TAG_TIME_IDX										11//当前时间计数器值，用于推算分数时间
#define TAG_RXINDIC_IDX									12//收到下行指示
#define TAG_RXRPD_IDX										12//收到下行RPD有效指示
#define TAG_EXTINFO_IDX									12//扩展信息

//取值
//顺序号,0~15静态，16动态
#define TAG_SER_Pos											4
#define TAG_SER_Msk											0xF0
//类型，0-常规信息，2-携带指令
#define TAG_TYPE_Pos										0
#define TAG_TYPE_Msk										0x0F
#define TAG_TYPE_NORMAL_INFO						0x00
#define TAG_TYPE_WITH_CMD								0x02
//接收窗口指示，1-携带接收窗口
#define TAG_WINDOWS_Pos									7
#define TAG_WINDOWS_Msk									0x80
#define TAG_WINDOWS_NOWIN								0
#define TAG_WINDOWS_WITHWIN							0x80
//静态接入指示
#define TAG_JOINTYPE_Pos								5
#define TAG_JOINTYPE_Msk								0x20
#define TAG_JOINTYPE_STATIC							0x20
#define TAG_JOINTYPE_DYNAMIC						0x00
//卡片仓储模式
#define TAG_LOWPWR_Pos									4
#define TAG_LOWPWR_Msk									0x10
#define TAG_LOWPWR_ON										0x10
//数据异常指示，1-异常
#define TAG_ERROR_Pos										3
#define TAG_ERROR_Msk										0x08
#define TAG_ERROR_WARN									0x08
#define TAG_ERROR_NORMAL								0x00
//按键指示，1-有效
#define TAG_KEY_Pos											2
#define TAG_KEY_Msk											0x04
#define TAG_KEY_WARN										0x04
#define TAG_KEY_NORMAL									0x00
//振动指示，1-振动
#define TAG_SHOCK_Pos										1
#define TAG_SHOCK_Msk										0x02
#define TAG_SHOCK_WARN									0x02
#define TAG_SHOCK_NORMAL								0x00
//低压指示，1-低压
#define TAG_LOWVOLTAGE_Pos							0
#define TAG_LOWVOLTAGE_Msk							0x01
#define TAG_LOWVOLTAGE_WARN							0x01
#define TAG_LOWVOLTAGE_NORMAL						0x00
//传感类型，0-无，1-温度，2-心率
#define TAG_SENSORTYPE_Pos							4
#define TAG_SENSORTYPE_Msk							0xF0
#define TAG_SENSORTYPE_None							(0 << TAG_SENSORTYPE_Pos)
#define TAG_SENSORTYPE_temp							(1 << TAG_SENSORTYPE_Pos)
#define TAG_SENSORTYPE_heart						(2 << TAG_SENSORTYPE_Pos)
//传感数值，0xFF无效值
//传感参数，采样周期，单位，0-秒，1-分，2-时
#define TAG_UNIT_Pos										6
#define TAG_UNIT_Msk										0xC0
//传感器参数，采样周期，时间数值，0-关闭传感器
#define TAG_UNIT_VALUE_Pos							0
#define TAG_UNIT_VALUE_Msk							0x3F
//当前时间计数器值，用于推算分数时间
#define TAG_TIME_Pos										0
#define TAG_TIME_Msk										0x3F
//收到下行指示：0-未收到，1收到
#define TAG_RXINDIC_Pos									7
#define TAG_RXINDIC_Msk									0x80
#define TAG_RXINDIC_VALID								0x80
#define TAG_RXINDIC_INVALID							0x00
//收到下行RPD有效指示，1-RPD有效
#define TAG_RXRPD_Pos										6
#define TAG_RXRPD_Msk										0x40
#define TAG_RXRPD_VALID									0x40
#define TAG_RXRPD_INVALID								0x00
/*******************************************
										结构体
********************************************/
struct RADIO_FRAME {
	uint8_t XOR;													//XOR校验
	uint8_t type;													//SEQ顺序/TYP类型
	uint8_t info[10];											//INFO基础信息
	uint8_t exinfo[20];										//EXT扩展信息	
};

typedef struct RADIO_FRAME * PRADIO_FRAME;

#define FRAME_SEQ_IDX										1
#define FRAME_DIR_IDX										1
#define FRAME_EXT_IDX										1
//顺序号
#define FRAME_SEQ_Pos										4
#define FRAME_SEQ_Msk										0xF0
//数据方向，0-上行，1-下行
#define FRAME_DIR_Pos										3
#define FRAME_DIR_Msk										0x08
#define FRAME_DIR_UP										(0)
#define FRAME_DIR_DOWN									(1 << FRAME_DIR_Pos)
//信息内容指示，0-常规，1扩展信息携带指令
#define FRAME_EXT_Pos										1
#define FRAME_EXT_Msk										0x02
#define FRAME_EXT_INFO									(0)
#define FRAME_EXT_EXTCMD								(1 << FRAME_EXT_Pos)
/*
业务定义的宏
*/
#define NORMAL_INFO											0
#define WITH_CMD												1

#define NO_WINDOW												0
#define WITH_WINDOW											1

#define TYPE_PARA												0
#define TYPE_RESEVE											1
#define TYPE_USER1											2
#define TYPE_USER2											3

extern uint8_t para_Record[RECORD_LEN];							//用于读取内部参数
extern uint8_t CycleSendEn;

uint8_t Get_ValidPara(uint8_t type,uint8_t *pbuf,uint8_t isrun);
uint8_t Read_Para(uint8_t type,uint8_t idx,uint8_t *pbuf);
uint8_t Write_Para(uint8_t type,uint8_t idx,uint8_t *pbuf);
uint8_t Erase_Para(uint8_t type);
void radio_pwr(uint8_t s);
void GetDeviceID(void);
//FILE END
