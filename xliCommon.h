//  xlCommon.h - Xlight common definitions header

#ifndef xliCommon_h
#define xliCommon_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "xliBitOperation.h"

// Here, common data types have been given alternative names through #define statements
// Common Data Type
#define BOOL                      bool
#define UC                        unsigned char
#define US                        unsigned short
#define UL                        unsigned long
#define CHAR                      char
#define SHORT                     short
#define LONG                      long

// Device Status
#define STATUS_OFF                0x00        // Power Off
#define STATUS_INIT               0x01        // Initializing
#define STATUS_BMW                0x02        // Basic Mode of Working
#define STATUS_DIS                0x03        // WAN Disconnected
#define STATUS_NWS                0x04        // Normal Working Status
#define STATUS_SLP                0x05        // Sleep
#define STATUS_RST                0x06        // Reseting
#define STATUS_ERR                0x07        // Fatal Error

// Cloud Dependency
#define CLOUD_DISABLE             0x00        // Disable Cloud
#define CLOUD_ENABLE              0x01        // Default, Enable Cloud, use it if available
#define CLOUD_MUST_CONNECT        0x02        // Enable Cloud and get connected anyhow

// Serial Port Speed
#define SERIALPORT_SPEED_LOW      9600
#define SERIALPORT_SPEED_14400    14400
#define SERIALPORT_SPEED_MID      19200
#define SERIALPORT_SPEED_57600    57600
#define SERIALPORT_SPEED_HIGH     115200
#define SERIALPORT_SPEED_DEFAULT  SERIALPORT_SPEED_HIGH

// Sensor Read Speed (in seconds)
#define SEN_DHT_SPEED_LOW         30
#define SEN_DHT_SPEED_NORMAL      5
#define SEN_DHT_SPEED_HIGH        2

#define SEN_ALS_SPEED_LOW         10
#define SEN_ALS_SPEED_NORMAL      5
#define SEN_ALS_SPEED_HIGH        2

#define SEN_PIR_SPEED_LOW         4
#define SEN_PIR_SPEED_NORMAL      2
#define SEN_PIR_SPEED_HIGH        1

// Row State Flags for Sync between Cloud, Flash, and Working Memory
enum EN_FLAG {DISABLED, ENABLED};
enum OP_FLAG {GET, POST, PUT, DELETE};
enum FLASH_FLAG {FF_EEPROM, FF_FLASH};
enum SAVE_FLAG {UNSAVED, SAVED};
enum RUN_FLAG {UNEXECUTED, EXECUTED};
enum LOAD_FLAG {UNLOADED, LOADED};

// enum values for CldJSONCommand()
typedef enum {
  CMD_SERIAL = 0,
  CMD_POWER = 1,
  CMD_COLOR = 2,
  CMD_BRIGHTNESS = 3,
  CMD_SCENARIO = 4,
  CMD_CCT = 5,
  CMD_QUERY = 6,
  CMD_EFFECT = 7,
  CMD_EXT = 8,
  CMD_INTERNAL = 9,

  CMD_MODBUS_READ = 80,
  CMD_MODBUS_WRITE
} COMMAND;

// Switch value for set power command
#define DEVICE_SW_OFF               0       // Turn Off
#define DEVICE_SW_ON                1       // Turn On
#define DEVICE_SW_TOGGLE            2       // Toggle
#define DEVICE_SW_DUMMY             3       // Don't change, detail followed

// Update operator for set brightness & CCT command
#define OPERATOR_SET                0
#define OPERATOR_ADD                1
#define OPERATOR_SUB                2
#define OPERATOR_MUL                3
#define OPERATOR_DIV                4

// Filter (special effect)
#define FILTER_SP_EF_NONE           0
#define FILTER_SP_EF_BREATH         1       // Normal breathing light
#define FILTER_SP_EF_FAST_BREATH    2       // Fast breathing light
#define FILTER_SP_EF_FLORID         3       // Randomly altering color
#define FILTER_SP_EF_FAST_FLORID    4       // Fast randomly altering color

// Macros for UID types
#define CLS_ACTION                  'a'
#define CLS_CONFIGURATION           'c'
#define CLS_SENSOR                  'e'
#define CLS_GROUP                   'g'
#define CLS_NOTIFICATION            'i'
#define CLS_NODECONFIG              'j'
#define CLS_SCHEDULE                'm'
#define CLS_NODE_INFO               'n'
#define CLS_NODE_STATUS             'o'
#define CLS_RULE                    'r'
#define CLS_SCENARIO                's'
#define CLS_REPORT                  't'
#define CLS_WEB_RESOURCE            'u'

// Hardware: Node Type
typedef enum
{
    NODE_TYP_ALL = 0,
    NODE_TYP_LAMP = 1,              // 1
    NODE_TYP_CURTAIN,               // 2
    NODE_TYP_AC,                    // 3
    NODE_TYP_FAN,                   // 4
    NODE_TYP_AIRPURE,               // 5
    NODE_TYP_PANEL,                 // 6, Same as REMOTE
    NODE_TYP_REMOTE = 6,            // 6, Same as PANEL
    NODE_TYP_COLORBAR,              // 7, Same as Spotlight
    NODE_TYP_SPOTLIGHT = 7,         // 7, Same as Colorbar
    NODE_TYP_SWITCH,                // 8, Same as Socket
    NODE_TYP_SOCKET = 8,            // 8, Same as Smart Switch
    NODE_TYP_MODBUS,                // 9
    NODE_TYP_IR,                    // 10
    NODE_TYP_THIRDPARTY,            // 11, Same as Others
    NODE_TYP_OTHERS = 11,           // 11, Same as Thirdparty
    NODE_TYP_SENSOR,                // 12
    NODE_TYP_SPECIAL,               // 13
    NODE_TYP_GROUP,                 // 14
    NODE_TYP_GW,                    // 15
    NODE_TYP_SYSTEM,                // 16
    NODE_TYP_DUMMY = 255            // 255
} eNodeType_t;

// Hardware: Sensor Type
typedef enum
{
    SEN_TYP_ALL = 0,
    SEN_TYP_ZENSOR,                 // 1
    SEN_TYP_ALS,                    // 2
    SEN_TYP_PIR,                    // 3
    SEN_TYP_DHT,                    // 4
    SEN_TYP_SICK,                   // 5
    SEN_TYP_NOISE,                  // 6
    SEN_TYP_SMOKE,                  // 7 (actual the last one)
    SEN_TYP_DUMMY = 255             // 255
} eSensorType_t;

// Sensor scope
#define SR_SCOPE_CONTROLLER         0     // Sensor on controller
#define SR_SCOPE_NODE               1     // Sensor on specific node
#define SR_SCOPE_ANY                2     // Sensor on any node under the controller
#define SR_SCOPE_GROUP              3     // Sensor on within the same node group

// Sensor logic symbols
#define SR_SYM_EQ                   0     // Equals
#define SR_SYM_NE                   1     // Not Equal
#define SR_SYM_GT                   2     // Greater Than
#define SR_SYM_GE                   3     // Greater than or Equal to
#define SR_SYM_LT                   4     // Less Than
#define SR_SYM_LE                   5     // Less than or Equal to
#define SR_SYM_BW                   6     // Between
#define SR_SYM_NB                   7     // Not Between

// Condition logic symbols
#define COND_SYM_NOT                0     // NOT
#define COND_SYM_AND                1     // AND
#define COND_SYM_OR                 2     // OR

// Time Unit
#define TM_NONE_UNIT                0
#define TM_SECOND_UNIT              1
#define TM_MINUTE_UNIT              2
#define TM_HOUR_UNIT                3

// Device (lamp) type
#define MAX_RING_NUM                3
typedef enum
{
    devtypUnknown = 0,
    // General Lightings
    devtypRelaySwitch = 1, // Any type that only controls switch
    devtypRelayBR10V = 2,  // Any type that supports both relay switch
                          // and 0-10V BR control
    devtypBR10VOnly = 4,   // Any type that only controls Brightness

    // Color ring - Rainbow (PWM: BR & WRGB)
    devtypCRing3 = 8,
    devtypCRing2,
    devtypCBar,
    devtypCFrame,
    devtypCWave,
    devtypCRing1 = 31,

    // White ring - Sunny
    devtypWRing3 = 32,
    devtypWRing2,
    devtypWBar,
    devtypWFrame,
    devtypWWave,
    devtypWSquare60,      // 60 * 60
    devtypWPanel120_30,   // 120 * 30
    devtypWBlackboard,    // Blackboard lamp
    devtypWRing1 = 95,

    // Color & Motion ring - Mirage
    devtypMRing3 = 96,
    devtypMRing2,
    devtypMBar,
    devtypMFrame,
    devtypMWave,
    devtypMRing1 = 127,

    devtypDummy = 255
} devicetype_t;

// Target Type (for group objects)
#define ZEN_TARGET_CURTAIN      0x80
#define ZEN_TARGET_AIRPURIFIER  0x88
#define ZEN_TARGET_AIRCONDITION 0x8C
#define ZEN_TARGET_SWITCH       0x90
#define ZEN_TARGET_SMARTSOCKET  0xA0
#define ZEN_TARGET_FAN          0xB0
#define ZEN_TARGET_COLORFULBAR  0xB8
#define ZEN_TARGET_SUPERSENSOR  0xC0

#define RING_ID_ALL                 0
#define RING_ID_1                   1
#define RING_ID_2                   2
#define RING_ID_3                   3

// NodeID Convention
#define NODEID_GATEWAY          0
#define NODEID_MAINDEVICE       1
#define NODEID_MIN_LAMP         8
#define NODEID_MAX_LAMP         31
#define NODEID_MIN_COLORBAR     32
#define NODEID_MAX_COLORBAR     39
#define NODEID_MIN_SWITCH       40
#define NODEID_MAX_SWITCH       63
#define NODEID_MIN_REMOTE       64
#define NODEID_MAX_REMOTE       79
#define NODEID_MIN_FAN          80
#define NODEID_MAX_FAN          95
#define NODEID_MIN_AC           96
#define NODEID_MAX_AC           111
#define NODEID_MIN_IR           112
#define NODEID_MAX_IR           123
#define NODEID_MIN_SUPERSENSOR  124
#define NODEID_MAX_SUPERSENSOR  139
#define NODEID_MIN_AIRPURE      140
#define NODEID_MAX_AIRPURE      147
#define NODEID_MIN_CURTAIN      148
#define NODEID_MAX_CURTAIN      179
#define NODEID_MIN_MODBUS       180
#define NODEID_MAX_MODBUS       185
#define NODEID_PROJECTOR        186
#define NODEID_KEYSIMULATOR     187
#define NODEID_SMARTPHONE       188
//  188-191: reserved for special devices
//  192-207: Node Groups
#define NODEID_MIN_GROUP        192
#define NODEID_MAX_GROUP        207
//  208-249: reserved for future extension
#define NODEID_RF_SCANNER       250
#define NODEID_DUMMY            255

#define IS_VALID_NODEID(nID)        ((nID) > NODEID_GATEWAY && (nID) < NODEID_RF_SCANNER)
#define IS_LAMP_NODEID(nID)         (((nID) >= NODEID_MIN_LAMP && (nID) <= NODEID_MAX_LAMP) || (nID) == NODEID_MAINDEVICE)
#define IS_COLORBAR_NODEID(nID)     ((nID) >= NODEID_MIN_COLORBAR && (nID) <= NODEID_MAX_COLORBAR)
#define IS_SWITCH_NODEID(nID)       ((nID) >= NODEID_MIN_SWITCH && (nID) <= NODEID_MAX_SWITCH)
#define IS_REMOTE_NODEID(nID)       ((nID) >= NODEID_MIN_REMOTE && (nID) <= NODEID_MAX_REMOTE)
#define IS_NOT_REMOTE_NODEID(nID)   ((nID) < NODEID_MIN_REMOTE || (nID) > NODEID_MAX_REMOTE)
#define IS_FAN_NODEID(nID)          ((nID) >= NODEID_MIN_FAN && (nID) <= NODEID_MAX_FAN)
#define IS_AC_NODEID(nID)           ((nID) >= NODEID_MIN_AC && (nID) <= NODEID_MAX_AC)
#define IS_NOT_AC_NODEID(nID)       ((nID) < NODEID_MIN_AC || (nID) > NODEID_MAX_AC)
#define IS_IR_NODEID(nID)           ((nID) >= NODEID_MIN_IR && (nID) <= NODEID_MAX_IR)
#define IS_SENSOR_NODEID(nID)       ((nID) >= NODEID_MIN_SUPERSENSOR && (nID) <= NODEID_MAX_SUPERSENSOR)
#define IS_AIRPURE_NODEID(nID)      ((nID) >= NODEID_MIN_AIRPURE && (nID) <= NODEID_MAX_AIRPURE)
#define IS_CURTAIN_NODEID(nID)      ((nID) >= NODEID_MIN_CURTAIN && (nID) <= NODEID_MAX_CURTAIN)
#define IS_MODBUS_NODEID(nID)       ((nID) >= NODEID_MIN_MODBUS && (nID) <= NODEID_MAX_MODBUS)
#define IS_SPECIAL_NODEID(nID)      ((nID) >= NODEID_PROJECTOR && (nID) <= NODEID_SMARTPHONE)
#define IS_GROUP_NODEID(nID)        ((nID) >= NODEID_MIN_GROUP && (nID) <= NODEID_MAX_GROUP)
#define IS_SYSTEM_NODEID(nID)       (((nID) > NODEID_MAINDEVICE && (nID) < NODEID_MIN_LAMP) || ((nID) >= NODEID_RF_SCANNER && (nID) <= NODEID_DUMMY))
#define IS_NOT_DEVICE_NODEID(nID)   (IS_SYSTEM_NODEID(nID) || IS_SENSOR_NODEID(nID) || IS_REMOTE_NODEID(nID) || IS_GROUP_NODEID(nID))


// Device ID structure
typedef struct
{
    UC nid;                                   // NodeID
    UC subID;                                 // SubID
} DeviceID_t;

// Get & Set Working Object Flag
/// EN_FLAG en_flag             : 1;
/// OP_FLAG op_flag			    : 2;
/// FLASH_FLAG flash_flag       : 1;
/// SAVE_FLAG save_flag			: 1;
/// RUN_FLAG run_flag           : 1;
/// LOAD_FLAG load_flag			: 1;
#define SET_FLAG_EN(_flag, _val)    BF_SET(_flag, _val, 0, 1)
#define GET_FLAG_EN(_flag)          (EN_FLAG)BF_GET(_flag, 0, 1)
#define SET_FLAG_OP(_flag, _val)    BF_SET(_flag, _val, 1, 2)
#define GET_FLAG_OP(_flag)          (OP_FLAG)BF_GET(_flag, 1, 2)
#define SET_FLAG_FLASH(_flag, _val) BF_SET(_flag, _val, 3, 1)
#define GET_FLAG_FLASH(_flag)       (FLASH_FLAG)BF_GET(_flag, 3, 1)
#define SET_FLAG_SAVE(_flag, _val)  BF_SET(_flag, _val, 4, 1)
#define GET_FLAG_SAVE(_flag)        (SAVE_FLAG)BF_GET(_flag, 4, 1)
#define SET_FLAG_RUN(_flag, _val)   BF_SET(_flag, _val, 5, 1)
#define GET_FLAG_RUN(_flag)         (RUN_FLAG)BF_GET(_flag, 5, 1)
#define SET_FLAG_LOAD(_flag, _val)  BF_SET(_flag, _val, 6, 1)
#define GET_FLAG_LOAD(_flag)        (LOAD_FLAG)BF_GET(_flag, 6, 1)

//--------------------------------------------------
// Tools & Helpers
//--------------------------------------------------
UC h2i(const char c);
UC *StringToByte(char *pString, UC *pByte);
char *ByteToString(UC *pByte, char *pString, const US len);
US usCRC16(UC * pucFrame, US usLen);

// Return NodeType by giving NodeID
UC NodeID2Type(const UC _nid);

#endif /* xliCommon_h */
