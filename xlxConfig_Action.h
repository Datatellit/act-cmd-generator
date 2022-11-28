// xlxConfig_Action.h - Xlight Data Structure for Action

#ifndef __XLX_CONFIG_ACTION_H__
#define __XLX_CONFIG_ACTION_H__

#include "xliCommon.h"

#pragma pack(1)

// Maximum commands within an action
#define MAX_COMMAND_PER_ACTION      	16

//------------------------------------------------------------------
// Xlight Action Table Structures
//------------------------------------------------------------------
typedef struct			// 3 Bytes
{
	UC sw;				// ���أ�0-�أ�1-����2-�л���
	UC delayopUnit;		// ��ʱ������λ��0-�ޣ�1-�룬2-���ӣ�3-Сʱ��
	UC delayopTime;		// ��ʱ����ʱ��
} V_STATUS_t;			// with delayed operation parameters

typedef struct			// 2 Bytes
{
	UC type;			// ���Ʒ�ʽ��0-��������ֵ��1-�������ȣ����ֵ����2-�������ȣ����ֵ����
	UC br;				// ���ȣ����Ⱦ���ֵ10-100
} V_PERCENTAGE_t;

typedef struct			// 2 Bytes
{
	UC sw;				// ���أ�0-������ 1-�� 2-��
	UC speed;			// ��λ��1-0��  2-1��  3-2�� 4-3��
} V_WIND_t;

typedef struct			// 7 Bytes
{
	US code;			// �յ����룬��λ��ǰ
	UC sw;				// ���أ�0-�أ�1-����
	UC mode;			// ģʽ��0-�Զ���1-���䣬2-��ʪ��3-�ͷ磬4-���ȣ�
	UC temp;			// �趨�¶ȣ�16-32��
	UC speed;			// ���٣�0-�Զ���1-�ͣ�2-�У�3-��
	UC fantp_light;		// 0-3, bit0: ����ģʽ: 1-�Զ�����0-�ֶ�����bit1: ����: 0-�أ�1-��
} V_HVAC_FLOW_STATE_t;

typedef struct			// 3 Bytes
{
	UC type;			// ���Ʒ�ʽ��0-����ɫ��ֵ��1-����ɫ�£����ֵ����2-����ɫ�£����ֵ����
	US cct;				// ɫ�£�����ɫ��ֵ2700-6500ռ2���ֽڣ���8λ��ǰ����8λ�ں�
} V_LEVEL_t;

typedef struct			// 5 Bytes
{
	UC ring;			// ringid�̶�Ϊ0��
	UC sw;				// ���أ�0-�أ�1-����2-�л���
	UC br;				// ���ȣ����Ⱦ���ֵ10-100
	US cct;				// ɫ�£�����ɫ��ֵ2700-6500ռ2���ֽڣ���8λ��ǰ����8λ�ں�
} V_RGBW_t;

#define rSet_ActCmd_cmd(_msg, _cmd)       BF_SET((_msg).cmd_ack_ldt, _cmd, 0, 3)
#define mGet_ActCmd_cmd(_msg)             BF_GET((_msg).cmd_ack_ldt, 0, 3)
#define rSet_ActCmd_rck(_msg, _rck)       BF_SET((_msg).cmd_ack_ldt, _rck, 3, 1)
#define mGet_ActCmd_rck(_msg)             BF_GET((_msg).cmd_ack_ldt, 3, 1)
#define rSet_ActCmd_ack(_msg, _ack)       BF_SET((_msg).cmd_ack_ldt, _ack, 4, 1)
#define mGet_ActCmd_ack(_msg)             BF_GET((_msg).cmd_ack_ldt, 4, 1)
#define rSet_ActCmd_ldt(_msg, _ldt)       BF_SET((_msg).cmd_ack_ldt, _ldt, 5, 3)
#define mGet_ActCmd_ldt(_msg)             BF_GET((_msg).cmd_ack_ldt, 5, 3)

typedef struct			// Exact 6 bytes, compatible with ModBus control message
{
	UC nid;             // NodeID
	UC cmd_ack_ldt;		// 3 bit - Command type
						// 1 bit - Request an ack - Indicator that receiver should send an ack back.
						// 1 bit - Is ack messsage - Indicator that this is the actual ack message.
						// 3 bit - Length of devType (0 to 4)
	UC sensorType;   	// tag or command code
	UC subID;           // SubID
	UC nodeType;		// Node type, only valid when nid = 255
	UC paramLen;		// Length (bytes) of parameters (exact length of payload, must shorter than 12 bytes)
} cmdItem_head_t;

typedef union       // Exact 12 bytes
{
	UC 						state;			// V_UP, V_DOWN, V_STOP, V_RELAY_ON, V_RELAY_OFF, V_RELAY_MAP
	UC						scenario_id;	// V_SCENE_ON, ����ID��ע����ʱNodeID������
	UC						fan_speed;		// V_FAN
	V_STATUS_t 				status;			// V_STATUS with delayed operation parameters��Ŀǰ���ƿؽڵ�֧��
	V_PERCENTAGE_t 			percentage;
	V_WIND_t				wind;
	V_HVAC_FLOW_STATE_t		hvac;
	V_LEVEL_t				cct;
	V_RGBW_t				sw_br_cct;
	UC data[12];							// may include devType upto 4 bytes
} cmdItem_body_t;

// 18 Bytes
typedef struct
{
	cmdItem_head_t head;       	// Exact 6 bytes
	cmdItem_body_t body;       	// Exact 12 bytes
} cmdItem_t;

// Action Commands
typedef struct
{
	UC uid                   : 8;		// 1-based index for physical storage, not necessary, only used for validation
	US crc16;
	cmdItem_t list[MAX_COMMAND_PER_ACTION];		// 16 * 18 = 288
} ActionCmd_t;

// Information of an action object
typedef struct			// Exact 4 bytes
{
	UC uid                   : 8;		// 1-based index for physical storage, not necessary, only used for validation
	UC cmdCount;
	UC calledTimes;
	UC reserved;
} ActionInfo_t;

#define ACT_ROW_SIZE		512			// Size of each action record
#define MAX_ACT_ROWS		64			// 64 * 512 bytes = 32KB

// Action List Type
// 64 * 4 Bytes = 256 Bytes
typedef struct
{
	ActionInfo_t list[MAX_ACT_ROWS];
} ActionList_t;

#endif /* __XLX_CONFIG_ACTION_H__ */