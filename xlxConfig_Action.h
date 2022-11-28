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
	UC sw;				// 开关，0-关，1-开，2-切换；
	UC delayopUnit;		// 延时操作单位：0-无，1-秒，2-分钟，3-小时；
	UC delayopTime;		// 延时操作时间
} V_STATUS_t;			// with delayed operation parameters

typedef struct			// 2 Bytes
{
	UC type;			// 控制方式，0-设置亮度值，1-增加亮度（相对值），2-减少亮度（相对值）；
	UC br;				// 亮度，亮度具体值10-100
} V_PERCENTAGE_t;

typedef struct			// 2 Bytes
{
	UC sw;				// 开关，0-不操作 1-关 2-开
	UC speed;			// 档位，1-0档  2-1档  3-2档 4-3档
} V_WIND_t;

typedef struct			// 7 Bytes
{
	US code;			// 空调代码，高位在前
	UC sw;				// 开关，0-关，1-开；
	UC mode;			// 模式，0-自动，1-制冷，2-抽湿，3-送风，4-制热；
	UC temp;			// 设定温度，16-32；
	UC speed;			// 风速，0-自动，1-低，2-中，3-高
	UC fantp_light;		// 0-3, bit0: 风扇模式: 1-自动风向，0-手动风向；bit1: 灯显: 0-关，1-开
} V_HVAC_FLOW_STATE_t;

typedef struct			// 3 Bytes
{
	UC type;			// 控制方式，0-设置色温值，1-增加色温（相对值），2-减少色温（相对值）；
	US cct;				// 色温，具体色温值2700-6500占2个字节，高8位在前，低8位在后
} V_LEVEL_t;

typedef struct			// 5 Bytes
{
	UC ring;			// ringid固定为0；
	UC sw;				// 开关，0-关，1-开，2-切换；
	UC br;				// 亮度，亮度具体值10-100
	US cct;				// 色温，具体色温值2700-6500占2个字节，高8位在前，低8位在后
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
	UC						scenario_id;	// V_SCENE_ON, 场景ID，注：此时NodeID无作用
	UC						fan_speed;		// V_FAN
	V_STATUS_t 				status;			// V_STATUS with delayed operation parameters，目前仅灯控节点支持
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