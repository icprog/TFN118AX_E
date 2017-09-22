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
//�ڲ�FLASH������
#define RAMTYPE_PARA										0
#define RAMTYPE_RESEVER									1
#define RAMTYPE_USER1										2
#define RAMTYPE_USER2										3

//��ƵƵ���ͷ���ѡ��
#define CHANNEL_DATA										0
#define CHANNEL_CONFIG									1
#define RADIO_RX												0
#define RADIO_TX												1
//Ĭ����ƵƵ��
#define DEFAULT_CHANNEL_DATA						23
#define DEFAULT_CHANNEL_CONFIG					7

/*******************************************
							�ڲ�����λ��
*******************************************/
// #define UNDEFINE												0

// #define PARA_ADD_IDX										0//5�ֽ���Ƶ��ַ
// #define PARA_DATACHANNEL_IDX						5//����Ƶ��
// #define PARA_CONFIGCHANNEL_IDX					6//����Ƶ��
// #define PARA_BRIEFNUM_IDX								7//�̺�
// #define PARA_PWR_IDX										8//���书��
// #define PARA_RPINFOSRC_IDX							8//�Զ��ϱ�����Я������Ϣ��Դ
// #define PARA_PRIODSEND_IDX							8//ʹ�����ڷ���
// #define PARA_PRIO_IDX										9//��λ���ȼ�
// #define PARA_FIX_IDX										9//�̶�
// #define PARA_TAGID_IDX									9//�����ǩ��ID���ͣ�0-������1-�û��Զ���
// #define PARA_MSGTYPE_IDX								9//�����ǩ��Ϣ���ͣ�0-����Ϣ��1-����Ϣ
// #define PARA_NOLINKTEST_IDX							10//�ر����Ӽ�⣬0-������ӣ�1�رռ������
// #define PARA_RPTRGTYPE_IDX							10//������ʽ��0-�䶯������1-��ʱ����
// #define PARA_INTERVAL_IDX								10//�������/��ʱ�������λ2S��0-������/��ʱ�������120S
// #define PARA_RSSI_IDX										11//RSSI����
// #define PARA_DELIVERID_IDX							12//����ID��4B��0x00000000-��д��
/*�ڲ�����ȡֵ*/
//���书�ʣ�0��7��0--30dbm��1--20dbm��2--16dbm,3--12dbm,4--8dbm,5--4dbm,6--0dbm,7-+4dbm
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
// //�Զ��ϱ�Я������Ϣ��Դ��0��2��0-��������1-�û���1��2-�û���2
// #define PARA_RPINFOSRC_Pos							2
// #define PARA_RPINFOSRC_Msk							0x0C
// #define PARA_RPINFOSRC_RESEVER					0
// #define PARA_RPINFOSRC_USER1						1
// #define PARA_RPINFOSRC_USER2						2
// //ʹ�����ڷ��ͣ�1-ʹ��
// #define PARA_PRIODSEND_Pos							0
// #define PARA_PRIODSEND_Msk							0x03
// //��λ���ȼ���0��3��0-�����붨λ��1��3��ֵԽ��λ���ȼ�Խ��
// #define PARA_PRIO_Pos										6
// #define PARA_PRIO_Msk										0xC0
// //�̶�ֵ��1
// #define PARA_FIX_Pos										5
// #define PARA_FIX_Msk										0x20
// //�����ǩID���ͣ�0/1��0-����ID��1-�û��Զ���ID
// #define PARA_TAGID_Pos									4
// #define PARA_TAGID_Msk									0x10
// #define PARA_TAGID_DELIVERID						0
// #define PARA_TAGID_SELFDEFINEID					1
// //�����ǩ��Ϣ���ͣ�0-����Ϣ��1-����Ϣ
// #define PARA_MSGTYPE_Pos								3
// #define PARA_MSGTYPE_Msk								0x08
// #define PARA_MSGTYPE_LONG								(1 << PARA_MSGTYPE_Pos)
// #define PARA_MSGTYPE_SHORT							(0 << PARA_MSGTYPE_Pos)
// //�ر����Ӽ�⣬0/1��-0-������ӣ�1-�رռ������
// #define PARA_NOLINKTEST_Pos							7
// #define PARA_NOLINKTEST_Msk							0x80
// #define PARA_NOLINKTEST_TEST						(0 << PARA_NOLINKTEST_Pos)
// #define PARA_NOLINKTEST_NOTEST					(1 << PARA_NOLINKTEST_Pos)
// //���������0/1��0-�䶯������1��ʱ����
// #define PARA_RPTRGTYPE_Pos							6
// #define PARA_RPTRGTYPE_Msk							0x40
// #define PARA_RPTRGTYPE_VARTRG						(0 << PARA_RPTRGTYPE_Pos)
// #define PARA_RPTRGTYPE_CYCLETRG					(1 << PARA_RPTRGTYPE_Pos)
// //�������/��ʱ�����0��60����λ2S��0-������/�޶�ʱ��������۸�120S
// #define PARA_INTERVAL_Pos										0
// #define PARA_INTERVAL_Msk										0x3F
// //RSSI���ޣ�0��0x7F
// #define PARA_RSSI_Pos										0
// #define PARA_RSSI_Msk										0x7F

/********************************************
						��д���·�(�·�ָ��/�ظ�)
********************************************/
//�·�ָ������
#define DOWN_TYPE_IDX										1
#define DOWN_DESTID_IDX									2
#define DOWN_BRIEFNUM_IDX								7//�̺�0��15

//ȡֵ
//�����ͣ�8/10��8-������Ϣ��10-Я��ָ��
#define DOWN_TYPE_Pos										0
#define DOWN_TYPE_Msk										0x0F

#define DOWN_WITH_CMD										10
/********************************************
				��д���ϴ����Զ��ϱ�/�ظ���
********************************************/
//�ϱ�ָ������
#define UP_FIX_IDX											1
#define UP_TYPE_IDX											1
#define UP_READERID_IDX									2
#define UP_RXWIN_IDX										6
#define UP_BRIEFNUM_IDX									7
#define UP_RXINDIC_IDX									12
#define UP_RXRPD_IDX										12
#define UP_EXTCMD												12
//ȡֵ
#define UP_FIX_Pos											4
#define UP_FIX_Msk											0xF0
#define UP_FIX_VAL											0
//�������ͣ�0/2��0-������Ϣ��2-Я��ָ��
#define UP_TYPE_Pos											0
#define UP_TYPE_Msk											0x0F
#define UP_TYPE_INFO										(0<<UP_TYPE_Pos)
#define UP_TYPE_EXTCMD									(2<<UP_TYPE_Pos)
//���մ���ָʾ��0/1��1-Я�����մ�
#define UP_RXWIN_Pos										7
#define UP_RXWIN_Msk										0x80
//�յ�ָ��ָʾ 1-�յ���0-δ�յ�
#define UP_RXINDIC_Pos									7
#define UP_RXINDIC_Msk									0x80
#define UP_RXINDIC_VALID								(1 << UP_RXINDIC_Pos)
//�յ�ָ��RPD��Чָʾ��1-RPD��Ч
#define UP_RXRPD_Pos										6
#define UP_RXRPD_Msk										0x40
#define UP_RXRPD_VALID									(1 << UP_RXRPD_Pos)

/********************************************
									��չ��Ϣ����
********************************************/
#define EXTINFO_LEN											20
#define EXTINFO_BASE_IDX								12
#define EXTINFO_CMD_IDX									(EXTINFO_BASE_IDX + 0)//ָ��
#define EXTINFO_FLSPG_IDX								(EXTINFO_BASE_IDX + 1)//�ڲ�Flashҳ��ַ�����ָ��0��1��2
#define EXTINFO_RDVLD_IDX								(EXTINFO_BASE_IDX + 1)//��¼ƫ����Ч�����ָ��0��1
#define EXTINFO_RDOS_IDX								(EXTINFO_BASE_IDX + 1)//��¼ƫ�ƣ����ָ��0��1
#define EXTINFO_CODENUM_IDX							(EXTINFO_BASE_IDX + 1)//����ţ����ָ��6
#define EXTINFO_DATA_IDX								(EXTINFO_BASE_IDX + 4)//16�ֽڶ�д����

#define EXTINFO_CMD_IDX2								(0)//ָ��
#define EXTINFO_FLSPG_IDX2							(1)//�ڲ�Flashҳ��ַ�����ָ��0��1��2
#define EXTINFO_RDVLD_IDX2							(1)//��¼ƫ����Ч�����ָ��0��1
#define EXTINFO_RDOS_IDX2								(1)//��¼ƫ�ƣ����ָ��0��1
#define EXTINFO_CODENUM_IDX2						(1)//����ţ����ָ��6
#define EXTINFO_DATA_IDX2								(4)//16�ֽڶ�д����
//��չ��Ϣȡֵ
#define EXTINFO_CMD_Pos									0
#define EXTINFO_CMD_Msk									0x0F
#define EXTINFO_CMD_READ								0
#define EXTINFO_CMD_WRITE								1
#define EXTINFO_CMD_ERASE								2
#define EXTINFO_CMD_READCURRAM					4//�����в���
#define EXTINFO_CMD_WRITECURRAM					5//д���в���
#define EXTINFO_CMD_RESUME							10//����
#define EXTINFO_CMD_SLEEP								11//����

//�ڲ�Flashҳ��ַ��0-�ڲ���������1-��������2-�û���1��3-�û���2
#define EXTINFO_FLSPG_Pos								6
#define EXTINFO_FLSPG_Msk								0xC0
#define EXTINFO_FLSPG_INNERPARA					0
#define EXTINFO_FLSPG_RESERVER					1
#define EXTINFO_FLSPG_USER1							2
#define EXTINFO_FLSPG_USER2							3
//��¼ƫ����Ч�ԣ�1-��Ч
#define EXTINFO_RDVLD_Pos								5
#define EXTINFO_RDVLD_Msk								0x20
#define EXTINFO_RDVLD_VALID							1
//��¼ƫ�ƣ�0~31ҳ��ַ0��1��16����¼��ҳ��ַ2��3��32����¼
#define EXTINFO_RDOS_Pos								0
#define EXTINFO_RDOS_Msk								0x1F
//����ţ�0~21ÿ����16�ֽڣ�21������336�ֽڣ������ڲ��̶�RAM������
#define EXTINFO_CODENUM_Pos							0
#define EXTINFO_CODENUM_Msk							0xFF

/********************************************
							UART�ϱ��ı�ǩ��Ϣ����
********************************************/
#define SR_SOH_IDX											0//6B�̶�ֵ��0xFFFFFFFFFF00
#define SR_LEN_IDX											6//2B
#define SR_ADDR_IDX											8//4B
#define SR_ATTR_IDX											12//2B
#define SR_TYPE_IDX											14//1B
#define SR_INFO_IDX											15//xB
//#define SR_XOR_IDX										16//1B

#define TAGMSG_SHORT_ID_IDX							0//4B
#define TAGMSG_SHORT_STAT_IDX						4//1B
#define TAGMSG_SHORT_REF_IDX						5//1B,��������ֵ��λ����Ϣ/�������ݣ�0xff��Чֵ
#define TAGMSG_LONG_SELFDEFINE					6//10B,�û��Զ���

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
							UART�·��ӿڶ���
********************************************/
#define UART_LEN_IDX										0
#define UART_ADDR_IDX										2
#define UART_ATTR_IDX										6
#define UART_TYPE_IDX										8
#define UART_INFO_IDX										9

//ȡֵ����
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
									ID�Ŷζ���
********************************************/
#define ID_TAP_REGION_First							0x00000001//��ǩID
#define ID_TAP_REGION_Last							0xFEFFFFFF
#define ID_RECEIVER_REGION_First				0xFF000000//������/��д��ID
#define ID_RECEIVER_REGION_Last					0xFFFDFFFF
#define ID_TRANSCEIVER_REGION_First			0xFFFE0000//����������(����)
#define ID_TRANSCEIVER_REGION_Last			0xFFFEFFFF
#define ID_RESERVER_REGION_First				0xFFFF0000//����
#define ID_RESERVER_REGION_Last					0xFFFFFFFA
#define ID_SELF_RP											0xFFFFFFFB//����ID�����ڷ������������ʹ�ܻظ�(ֻ��UART�˳���)
#define ID_BC_TAP_RP										0xFFFFFFFC//�㲥ID��Ŀ�����б�ǩ��ʹ�ܻظ�
#define ID_BC_TAP_NRP										0xFFFFFFFD//�㲥ID��Ŀ�����б�ǩ����ֹ�ظ�
#define ID_BROADCAST_READER_RP					0xFFFFFFFE//�㲥ID��Ŀ�����н�������ʹ�ܻظ�
#define ID_RESERVER1										0xFFFFFFFF//����
#define ID_RESERVER0										0x00000000//����

#define ID_TYPE_TAP											0
#define ID_TYPE_RECEIVER								1
#define ID_TYPE_TRANSCEIVER							2
#define ID_TYPE_ERROR										4
#define ID_TYPE_SELF_RP									5
#define ID_TYPE_ALLTAP_RP								6
#define ID_TYPE_ALLTAP_NRP							7
#define ID_TYPE_ALLRECEIVER_RP					8
/********************************************
					��ǩ�ڲ�����
********************************************/
#define TAGP_ADD_IDX										0//5�ֽ���Ƶ��ַ
#define TAGP_DATACHANNEL_IDX						5//����Ƶ��
#define TAGP_CONFIGCHANNEL_IDX					6//����Ƶ��
#define TAGP_BRIEFNUM_IDX								7//�̺ţ�0��16
#define TAGP_PWR_IDX										8//���书�ʣ�0��7
#define TAGP_RPINFOSRC_IDX							8//�Զ��ϱ�����Я������Ϣ��Դ��0��2
#define TAGP_WORKMODE_IDX								8//����ģʽ��0-����ģʽ��1-�ģʽ
#define TAGP_DELIVERID_IDX							9//����ID��4B��0xFFFFFFFF-��д��
#define TAGP_SENSORTYPE_IDX							13//�������ͣ�0-�ޣ�1-�¶ȣ�2-���ʣ�0��2
#define TAGP_SSRUNIT_IDX								14//���в������ڵ�λ��0-�룬1-�֣�2-ʱ
#define TAGP_SSRVALUE_IDX								14
/*�ڲ�����ȡֵ*/
//���书�ʣ�0��7��0--30dbm��1--20dbm��2--16dbm,3--12dbm,4--8dbm,5--4dbm,6--0dbm,7-+4dbm
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
//�Զ��ϱ�Я������Ϣ��Դ��0��2��0-��������1-�û���1��2-�û���2
#define TAGP_RPINFOSRC_Pos							2
#define TAGP_RPINFOSRC_Msk							0x0C
#define TAGP_RPINFOSRC_RESEVER					0
#define TAGP_RPINFOSRC_USER1						1
#define TAGP_RPINFOSRC_USER2						2
//����ģʽ,0-����ģʽ��1-�ģʽ
#define TAGP_WORKMODE_Pos								0
#define TAGP_WORKMODE_Msk								0x03
//�������ͣ�0-�ޣ�1-�¶ȣ�2-����
#define TAGP_SENSORTYPE_Pos							4
#define TAGP_SENSORTYPE_Msk							0xF0
//���в�������λ
#define TAGP_SSRUNIT_Pos								6
#define TAGP_SSRUNIT_Msk								0xC0
//���в�����ֵ,0~60,0-�ر�
#define TAGP_SSRVALUE_Pos								0
#define TAGP_SSRVALUE_Msk								0x3F
/********************************************
							��ǩ�ϱ���������
********************************************/
#define TAG_SER_IDX											1//˳���
#define TAG_TYPE_IDX										1//��Ϣ��ǩ����
#define TAG_ID_IDX											2//��ǩID
#define TAG_WINDOWS_IDX									6//���մ���ָʾ
#define TAG_JOINTYPE_IDX								6//��̬����ָʾ
#define TAG_ERROR_IDX										6//�����쳣ָʾ
#define TAG_KEY_IDX											6//����ָʾ
#define TAG_SHOCK_IDX										6//��ָʾ
#define TAG_LOWVOLTAGE_IDX							6//�͵��־
#define TAG_LOWPWR_IDX									6//�ִ�ģʽ
#define TAG_LOCATION_IDX								7//��ǩ��λ������������
#define TAG_SENSORTYPE_IDX							8//����������
#define TAG_VALUE_IDX										9//����������
#define TAG_UNIT_IDX										10//����������-������λ
#define TAG_UNIT_VALUE_IDX							10//����������-�������ڣ�0-�رմ�����
#define TAG_TIME_IDX										11//��ǰʱ�������ֵ�������������ʱ��
#define TAG_RXINDIC_IDX									12//�յ�����ָʾ
#define TAG_RXRPD_IDX										12//�յ�����RPD��Чָʾ
#define TAG_EXTINFO_IDX									12//��չ��Ϣ

//ȡֵ
//˳���,0~15��̬��16��̬
#define TAG_SER_Pos											4
#define TAG_SER_Msk											0xF0
//���ͣ�0-������Ϣ��2-Я��ָ��
#define TAG_TYPE_Pos										0
#define TAG_TYPE_Msk										0x0F
#define TAG_TYPE_NORMAL_INFO						0x00
#define TAG_TYPE_WITH_CMD								0x02
//���մ���ָʾ��1-Я�����մ���
#define TAG_WINDOWS_Pos									7
#define TAG_WINDOWS_Msk									0x80
#define TAG_WINDOWS_NOWIN								0
#define TAG_WINDOWS_WITHWIN							0x80
//��̬����ָʾ
#define TAG_JOINTYPE_Pos								5
#define TAG_JOINTYPE_Msk								0x20
#define TAG_JOINTYPE_STATIC							0x20
#define TAG_JOINTYPE_DYNAMIC						0x00
//��Ƭ�ִ�ģʽ
#define TAG_LOWPWR_Pos									4
#define TAG_LOWPWR_Msk									0x10
#define TAG_LOWPWR_ON										0x10
//�����쳣ָʾ��1-�쳣
#define TAG_ERROR_Pos										3
#define TAG_ERROR_Msk										0x08
#define TAG_ERROR_WARN									0x08
#define TAG_ERROR_NORMAL								0x00
//����ָʾ��1-��Ч
#define TAG_KEY_Pos											2
#define TAG_KEY_Msk											0x04
#define TAG_KEY_WARN										0x04
#define TAG_KEY_NORMAL									0x00
//��ָʾ��1-��
#define TAG_SHOCK_Pos										1
#define TAG_SHOCK_Msk										0x02
#define TAG_SHOCK_WARN									0x02
#define TAG_SHOCK_NORMAL								0x00
//��ѹָʾ��1-��ѹ
#define TAG_LOWVOLTAGE_Pos							0
#define TAG_LOWVOLTAGE_Msk							0x01
#define TAG_LOWVOLTAGE_WARN							0x01
#define TAG_LOWVOLTAGE_NORMAL						0x00
//�������ͣ�0-�ޣ�1-�¶ȣ�2-����
#define TAG_SENSORTYPE_Pos							4
#define TAG_SENSORTYPE_Msk							0xF0
#define TAG_SENSORTYPE_None							(0 << TAG_SENSORTYPE_Pos)
#define TAG_SENSORTYPE_temp							(1 << TAG_SENSORTYPE_Pos)
#define TAG_SENSORTYPE_heart						(2 << TAG_SENSORTYPE_Pos)
//������ֵ��0xFF��Чֵ
//���в������������ڣ���λ��0-�룬1-�֣�2-ʱ
#define TAG_UNIT_Pos										6
#define TAG_UNIT_Msk										0xC0
//�������������������ڣ�ʱ����ֵ��0-�رմ�����
#define TAG_UNIT_VALUE_Pos							0
#define TAG_UNIT_VALUE_Msk							0x3F
//��ǰʱ�������ֵ�������������ʱ��
#define TAG_TIME_Pos										0
#define TAG_TIME_Msk										0x3F
//�յ�����ָʾ��0-δ�յ���1�յ�
#define TAG_RXINDIC_Pos									7
#define TAG_RXINDIC_Msk									0x80
#define TAG_RXINDIC_VALID								0x80
#define TAG_RXINDIC_INVALID							0x00
//�յ�����RPD��Чָʾ��1-RPD��Ч
#define TAG_RXRPD_Pos										6
#define TAG_RXRPD_Msk										0x40
#define TAG_RXRPD_VALID									0x40
#define TAG_RXRPD_INVALID								0x00
/*******************************************
										�ṹ��
********************************************/
struct RADIO_FRAME {
	uint8_t XOR;													//XORУ��
	uint8_t type;													//SEQ˳��/TYP����
	uint8_t info[10];											//INFO������Ϣ
	uint8_t exinfo[20];										//EXT��չ��Ϣ	
};

typedef struct RADIO_FRAME * PRADIO_FRAME;

#define FRAME_SEQ_IDX										1
#define FRAME_DIR_IDX										1
#define FRAME_EXT_IDX										1
//˳���
#define FRAME_SEQ_Pos										4
#define FRAME_SEQ_Msk										0xF0
//���ݷ���0-���У�1-����
#define FRAME_DIR_Pos										3
#define FRAME_DIR_Msk										0x08
#define FRAME_DIR_UP										(0)
#define FRAME_DIR_DOWN									(1 << FRAME_DIR_Pos)
//��Ϣ����ָʾ��0-���棬1��չ��ϢЯ��ָ��
#define FRAME_EXT_Pos										1
#define FRAME_EXT_Msk										0x02
#define FRAME_EXT_INFO									(0)
#define FRAME_EXT_EXTCMD								(1 << FRAME_EXT_Pos)
/*
ҵ����ĺ�
*/
#define NORMAL_INFO											0
#define WITH_CMD												1

#define NO_WINDOW												0
#define WITH_WINDOW											1

#define TYPE_PARA												0
#define TYPE_RESEVE											1
#define TYPE_USER1											2
#define TYPE_USER2											3

extern uint8_t para_Record[RECORD_LEN];							//���ڶ�ȡ�ڲ�����
extern uint8_t CycleSendEn;

uint8_t Get_ValidPara(uint8_t type,uint8_t *pbuf,uint8_t isrun);
uint8_t Read_Para(uint8_t type,uint8_t idx,uint8_t *pbuf);
uint8_t Write_Para(uint8_t type,uint8_t idx,uint8_t *pbuf);
uint8_t Erase_Para(uint8_t type);
void radio_pwr(uint8_t s);
void GetDeviceID(void);
//FILE END
