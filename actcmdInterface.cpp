#include "actcmdInterface.h"
#include "MyMessage.h"

CActCmdInterface::CActCmdInterface()
{
    m_pCmdList = (cmdItem_t *)(m_msg + SCMSG_HEADER_LEN);
}

// 接口初始化：每次生成新的act-cmd命令列表前调用
void CActCmdInterface::Init(const UC _uid, const UC _cmdCnt)
{
    // 数据初始化
    memset(m_msg, 0x00, sizeof(m_msg));
    memset(m_strOutput, 0x00, sizeof(m_strOutput));    
    m_msg[0] = ACT_CMD_MSG_HEAD_0;
    m_msg[1] = ACT_CMD_MSG_HEAD_1;
    m_msg[2] = 1;
    m_msg[3] = 1;
    m_nCmdCnt = _cmdCnt;
    m_nPayloadLen = _cmdCnt * sizeof(cmdItem_t);
    m_msg[4] = m_nPayloadLen >> 8;
    m_msg[5] = m_nPayloadLen & 0xFF;
    m_msg[6] = _uid;
    m_nCmdIndex = 0;
}

/***************************************************************
* @func		AddCmd_SetState
* @brief    添加命令：开关状态改变命令
* @param
*   _nid：节点ID，具体节点或者255；255表示广播，这时通过NodeType筛选目标节点
*   _type：命令类型，包括：V_STATUS, V_RELAY_ON, V_RELAY_OFF, V_RELAY_MAP等
*   _state：开关状态值
*   _delayUnit：延时操作单位：0-无，1-秒，2-分钟，3-小时；
*   _delayTime：延时操作时间。注意：目前仅对灯控节点有效
*   _nt：节点类型(Node Type)，仅当_nid=255时有效，默认为0
*   _tpNum：'tp'列表长度（NodeType 或 DevType），0 到 4，默认为0
*   _tpList：'tp'列表指针，默认为空
* @retval	
***************************************************************/
void CActCmdInterface::AddCmd_SetState(const UC _nid, const UC _sid, const UC _type, const UC _state, const UC _delayUnit, const UC _delayTime, const UC _nt, const UC _tpNum, const UC *_tpList)
{
    // 指向当前命令
    cmdItem_t *lv_cmd = m_pCmdList + m_nCmdIndex;

    lv_cmd->head.nid = _nid;                        // 节点ID
    rSet_ActCmd_cmd(lv_cmd->head, C_SET);           // RF命令
    rSet_ActCmd_rck(lv_cmd->head, 1);               // 需要回应
    lv_cmd->head.sensorType = _type;                // 命令类型
    lv_cmd->head.subID = _sid;                      // 子节点ID
    if(_type == V_STATUS) {
        lv_cmd->head.paramLen = sizeof(V_STATUS_t);     // 开关状态改变命令参数长度
        lv_cmd->body.status.sw = _state;
        lv_cmd->body.status.delayopUnit = _delayUnit;
        lv_cmd->body.status.delayopTime = _delayTime;
    } else {
        lv_cmd->head.paramLen = 1;
        lv_cmd->body.state = _state;
    }

    // 节点类型
    lv_cmd->head.nodeType = (_nid == 255 ? _nt : 0);

    // 'tp'列表
    addTpList(_nid, lv_cmd, _tpNum, _tpList);

    // 下一条
    m_nCmdIndex++;
}

/***************************************************************
* @func		AddCmd_ChangeScenario
* @brief    添加命令：场景切换命令（此时NodeID无作用）
* @param    _scenario：场景ID
* @retval	
***************************************************************/
void CActCmdInterface::AddCmd_ChangeScenario(const UC _scenario)
{
    // 指向当前命令
    cmdItem_t *lv_cmd = m_pCmdList + m_nCmdIndex;

    lv_cmd->head.nid = 0;                           // 节点ID
    rSet_ActCmd_cmd(lv_cmd->head, C_SET);           // RF命令
    rSet_ActCmd_rck(lv_cmd->head, 1);               // 需要回应
    lv_cmd->head.sensorType = V_SCENE_ON;           // 切换场景
    lv_cmd->head.subID = 0;                         // 子节点ID（目前不使用）
    lv_cmd->head.paramLen = 1;                      // 场景切换命令参数长度
    lv_cmd->body.scenario_id = _scenario;

    // 下一条
    m_nCmdIndex++;
}

/***************************************************************
* @func		AddCmd_FanControl
* @brief    添加命令：风扇控制命令
* @param
*   _nid：节点ID，具体节点或者255；255表示广播，这时通过NodeType筛选目标节点
*   _speed：风扇开关和风速
*   _nt：节点类型(Node Type)，仅当_nid=255时有效，默认为0
*   _tpNum：'tp'列表长度（NodeType 或 DevType），0 到 4，默认为0
*   _tpList：'tp'列表指针，默认为空
* @retval	
***************************************************************/
void CActCmdInterface::AddCmd_FanControl(const UC _nid, const UC _sid, const UC _speed, const UC _nt, const UC _tpNum, const UC *_tpList)
{
    // 指向当前命令
    cmdItem_t *lv_cmd = m_pCmdList + m_nCmdIndex;

    lv_cmd->head.nid = _nid;                        // 节点ID
    rSet_ActCmd_cmd(lv_cmd->head, C_SET);           // RF命令
    rSet_ActCmd_rck(lv_cmd->head, 1);               // 需要回应
    lv_cmd->head.sensorType = V_FAN;                // 命令类型
    lv_cmd->head.subID = _sid;                      // 子节点ID
    lv_cmd->head.paramLen = 1;                      // 风扇控制命令参数长度
    lv_cmd->body.fan_speed = _speed;

    // 节点类型
    lv_cmd->head.nodeType = (_nid == 255 ? _nt : 0);

    // 'tp'列表
    addTpList(_nid, lv_cmd, _tpNum, _tpList);

    // 下一条
    m_nCmdIndex++;
}

/***************************************************************
* @func		AddCmd_BRControl
* @brief    添加命令：调节灯的亮度
* @param
*   _nid：节点ID，具体节点或者255；255表示广播，这时通过NodeType筛选目标节点
*   _op：控制方式，0-设置亮度值，1-增加亮度（相对值），2-减少亮度（相对值）；
*   _br：亮度，亮度具体值10-100
*   _nt：节点类型(Node Type)，仅当_nid=255时有效，默认为0
*   _tpNum：'tp'列表长度（NodeType 或 DevType），0 到 4，默认为0
*   _tpList：'tp'列表指针，默认为空
* @retval	
***************************************************************/
void CActCmdInterface::AddCmd_BRControl(const UC _nid, const UC _sid, const UC _op, const UC _br, const UC _nt, const UC _tpNum, const UC *_tpList)
{
    // 指向当前命令
    cmdItem_t *lv_cmd = m_pCmdList + m_nCmdIndex;

    lv_cmd->head.nid = _nid;                        // 节点ID
    rSet_ActCmd_cmd(lv_cmd->head, C_SET);           // RF命令
    rSet_ActCmd_rck(lv_cmd->head, 1);               // 需要回应
    lv_cmd->head.sensorType = V_PERCENTAGE;         // 命令类型
    lv_cmd->head.subID = _sid;                      // 子节点ID
    lv_cmd->head.paramLen = sizeof(V_PERCENTAGE_t); // 亮度调节命令参数长度
    lv_cmd->body.percentage.type = _op;
    lv_cmd->body.percentage.br = _br;

    // 节点类型
    lv_cmd->head.nodeType = (_nid == 255 ? _nt : 0);

    // 'tp'列表
    addTpList(_nid, lv_cmd, _tpNum, _tpList);

    // 下一条
    m_nCmdIndex++;
}

/***************************************************************
* @func		AddCmd_CCTControl
* @brief    添加命令：调节灯的色温
* @param
*   _nid：节点ID，具体节点或者255；255表示广播，这时通过NodeType筛选目标节点
*   _op：控制方式，0-设置色温值，1-增加色温（相对值），2-减少色温（相对值）；
*   _cct：色温，具体色温值2700-6500占2个字节，高8位在前，低8位在后
*   _nt：节点类型(Node Type)，仅当_nid=255时有效，默认为0
*   _tpNum：'tp'列表长度（NodeType 或 DevType），0 到 4，默认为0
*   _tpList：'tp'列表指针，默认为空
* @retval	
***************************************************************/
void CActCmdInterface::AddCmd_CCTControl(const UC _nid, const UC _sid, const UC _op, const US _cct, const UC _nt, const UC _tpNum, const UC *_tpList)
{
    // 指向当前命令
    cmdItem_t *lv_cmd = m_pCmdList + m_nCmdIndex;

    lv_cmd->head.nid = _nid;                        // 节点ID
    rSet_ActCmd_cmd(lv_cmd->head, C_SET);           // RF命令
    rSet_ActCmd_rck(lv_cmd->head, 1);               // 需要回应
    lv_cmd->head.sensorType = V_LEVEL;              // 命令类型
    lv_cmd->head.subID = _sid;                      // 子节点ID
    lv_cmd->head.paramLen = sizeof(V_LEVEL_t);      // 色温调节命令参数长度
    lv_cmd->body.cct.type = _op;
    lv_cmd->body.cct.cct = _cct;

    // 节点类型
    lv_cmd->head.nodeType = (_nid == 255 ? _nt : 0);

    // 'tp'列表
    addTpList(_nid, lv_cmd, _tpNum, _tpList);

    // 下一条
    m_nCmdIndex++;
}

/***************************************************************
* @func		AddCmd_LightControl
* @brief    添加命令：调节灯亮度和色温命令
* @param
*   _nid：节点ID，具体节点或者255；255表示广播，这时通过NodeType筛选目标节点
*   _sw：开关，0-关，1-开，2-切换；
*   _br：亮度，亮度具体值10-100
*   _cct：色温，具体色温值2700-6500占2个字节，高8位在前，低8位在后
*   _nt：节点类型(Node Type)，仅当_nid=255时有效，默认为0
*   _tpNum：'tp'列表长度（NodeType 或 DevType），0 到 4，默认为0
*   _tpList：'tp'列表指针，默认为空
* @retval	
***************************************************************/
void CActCmdInterface::AddCmd_LightControl(const UC _nid, const UC _sid, const UC _sw, const UC _br, const US _cct, const UC _nt, const UC _tpNum, const UC *_tpList)
{
    // 指向当前命令
    cmdItem_t *lv_cmd = m_pCmdList + m_nCmdIndex;

    lv_cmd->head.nid = _nid;                        // 节点ID
    rSet_ActCmd_cmd(lv_cmd->head, C_SET);           // RF命令
    rSet_ActCmd_rck(lv_cmd->head, 1);               // 需要回应
    lv_cmd->head.sensorType = V_RGBW;               // 命令类型
    lv_cmd->head.subID = _sid;                      // 子节点ID
    lv_cmd->head.paramLen = sizeof(V_RGBW_t);       // V_RGBW命令参数长度
    lv_cmd->body.sw_br_cct.ring = 0;
    lv_cmd->body.sw_br_cct.sw = _sw;
    lv_cmd->body.sw_br_cct.br = _br;
    lv_cmd->body.sw_br_cct.cct = _cct;

    // 节点类型
    lv_cmd->head.nodeType = (_nid == 255 ? _nt : 0);

    // 'tp'列表
    addTpList(_nid, lv_cmd, _tpNum, _tpList);

    // 下一条
    m_nCmdIndex++;
}

/***************************************************************
* @func		AddCmd_APControl
* @brief    添加命令：新风控制命令
* @param
*   _nid：节点ID，具体节点或者255；255表示广播，这时通过NodeType筛选目标节点
*   _sw：开关，0-不操作 1-关 2-开
*   _speed：档位，1-0档  2-1档  3-2档 4-3档
*   _nt：节点类型(Node Type)，仅当_nid=255时有效，默认为0
*   _tpNum：'tp'列表长度（NodeType 或 DevType），0 到 4，默认为0
*   _tpList：'tp'列表指针，默认为空
* @retval	
***************************************************************/
void CActCmdInterface::AddCmd_APControl(const UC _nid, const UC _sid, const UC _sw, const UC _speed, const UC _nt, const UC _tpNum, const UC *_tpList)
{
    // 指向当前命令
    cmdItem_t *lv_cmd = m_pCmdList + m_nCmdIndex;

    lv_cmd->head.nid = _nid;                        // 节点ID
    rSet_ActCmd_cmd(lv_cmd->head, C_SET);           // RF命令
    rSet_ActCmd_rck(lv_cmd->head, 1);               // 需要回应
    lv_cmd->head.sensorType = V_WIND;               // 命令类型
    lv_cmd->head.subID = _sid;                      // 子节点ID
    lv_cmd->head.paramLen = sizeof(V_WIND_t);       // 新风控制命令参数长度
    lv_cmd->body.wind.sw = _sw;
    lv_cmd->body.wind.speed = _speed;

    // 节点类型
    lv_cmd->head.nodeType = (_nid == 255 ? _nt : 0);

    // 'tp'列表
    addTpList(_nid, lv_cmd, _tpNum, _tpList);

    // 下一条
    m_nCmdIndex++;
}

/***************************************************************
* @func		AddCmd_ACControl
* @note     注意：空调代码需要事先设定好
* @brief    添加命令：空调控制命令
* @param
*   _nid：节点ID，具体节点或者255；255表示广播，这时通过NodeType筛选目标节点
*   _sw：开关，0-关，1-开；
*   _mode：模式，0-自动，1-制冷，2-抽湿，3-送风，4-制热；
*   _temp：设定温度，16-32；
*   _speed：风速，0-自动，1-低，2-中，3-高
*   _code： 空调代码，高位在前
*   _fantp_light：0-3, bit0: 风扇模式: 1-自动风向，0-手动风向；bit1: 灯显: 0-关，1-开
*   _nt：节点类型(Node Type)，仅当_nid=255时有效，默认为0
*   _tpNum：'tp'列表长度（NodeType 或 DevType），0 到 4，默认为0
*   _tpList：'tp'列表指针，默认为空
* @retval	
***************************************************************/
void CActCmdInterface::AddCmd_ACControl(const UC _nid, const UC _sid, const UC _sw, const UC _mode, const UC _temp, const UC _speed, const US _code, const UC _fantp_light, const UC _nt, const UC _tpNum, const UC *_tpList)
{
    // 指向当前命令
    cmdItem_t *lv_cmd = m_pCmdList + m_nCmdIndex;

    lv_cmd->head.nid = _nid;                        // 节点ID
    rSet_ActCmd_cmd(lv_cmd->head, C_SET);           // RF命令
    rSet_ActCmd_rck(lv_cmd->head, 1);               // 需要回应
    lv_cmd->head.sensorType = V_HVAC_FLOW_STATE;    // 命令类型
    lv_cmd->head.subID = _sid;                      // 子节点ID
    lv_cmd->head.paramLen = sizeof(V_HVAC_FLOW_STATE_t); // 空调控制命令参数长度
    lv_cmd->body.hvac.code = _code;
    lv_cmd->body.hvac.sw = _sw;
    lv_cmd->body.hvac.mode = _mode;
    lv_cmd->body.hvac.temp = _temp;
    lv_cmd->body.hvac.speed = _speed;
    lv_cmd->body.hvac.fantp_light = _fantp_light;

    // 节点类型
    lv_cmd->head.nodeType = (_nid == 255 ? _nt : 0);

    // 'tp'列表
    addTpList(_nid, lv_cmd, _tpNum, _tpList);

    // 下一条
    m_nCmdIndex++;
}

/***************************************************************
* @func		AddCmd_RelayString
* @brief    添加命令：继电器(窗帘)操作组合
* @param
*   _nid：节点ID，具体节点或者255；255表示广播，这时通过NodeType筛选目标节点
*   _type：命令类型，包括：V_UP, V_DOWN, V_STOP
*   _strLen：继电器操作组合字符串长度，存放在Payload第一个字节
*   _pStr：继电器操作组合字符串，从Payload的第二个字节开始，参见：继电器操作组合字符串说明
*   _nt：节点类型(Node Type)，仅当_nid=255时有效，默认为0
*   _tpNum：'tp'列表长度（NodeType 或 DevType），0 到 4，默认为0
*   _tpList：'tp'列表指针，默认为空
*  
*  继电器操作组合字符串说明：list of key pairs. One pair contains two charactors.
*  The first character stands for key operation style, e.g. press, quick-press, long-press, hold, release, double-click.
*  The second one is the key id ('0'-'9', 'a'-'z', 'A'-'Z'). 
*  If there are more than one key, the delimitator determines the interval between keys.
*  "+":no delay; ",":small pause; ".": normal pause; "=": long pause;  "~": very long pause; "_": same time
*
* @retval	
***************************************************************/
void CActCmdInterface::AddCmd_RelayString(const UC _nid, const UC _sid, const UC _type, const UC _strLen, const char *_pStr, const UC _nt, const UC _tpNum, const UC *_tpList )
{
    // 指向当前命令
    cmdItem_t *lv_cmd = m_pCmdList + m_nCmdIndex;

    lv_cmd->head.nid = _nid;                        // 节点ID
    rSet_ActCmd_cmd(lv_cmd->head, C_SET);           // RF命令
    rSet_ActCmd_rck(lv_cmd->head, 1);               // 需要回应
    lv_cmd->head.sensorType = _type;                // 命令类型
    lv_cmd->head.subID = _sid;                      // 子节点ID
    lv_cmd->head.paramLen = _strLen + 1;            // 字符串长度加上长度字节
    lv_cmd->body.data[0] = _strLen;
    memcpy(lv_cmd->body.data + 1, _pStr, _strLen);

    // 节点类型
    lv_cmd->head.nodeType = (_nid == 255 ? _nt : 0);

    // 'tp'列表
    addTpList(_nid, lv_cmd, _tpNum, _tpList);

    // 下一条
    m_nCmdIndex++;
}

/***************************************************************
* @func		AddCmd_CustomMessage
* @brief    添加命令：客户化消息命令，用户可自由设定消息内容，以支持各种非常规用途
* @param
*   _nid：节点ID，具体节点或者255；255表示广播，这时通过NodeType筛选目标节点
*   _sid：子节点ID
*   _cmd：RF命令
*   _type：命令类型
*   _pPayl：payload内容（字节数组）
*   _lenPayl：payload字节数
*   _nt：节点类型(Node Type)，仅当_nid=255时有效，默认为0
*   _tpNum：'tp'列表长度（NodeType 或 DevType），0 到 4，默认为0
*   _tpList：'tp'列表指针，默认为空
* @retval	
***************************************************************/
void CActCmdInterface::AddCmd_CustomMessage(const UC _nid, const UC _sid, const UC _cmd, const UC _type, const UC *_pPayl, const UC _lenPayl, const UC _nt, const UC _tpNum, const UC *_tpList)
{
    // 指向当前命令
    cmdItem_t *lv_cmd = m_pCmdList + m_nCmdIndex;

    lv_cmd->head.nid = _nid;                        // 节点ID
    rSet_ActCmd_cmd(lv_cmd->head, _cmd);            // RF命令
    rSet_ActCmd_rck(lv_cmd->head, 1);               // 需要回应
    lv_cmd->head.sensorType = _type;                // 命令类型
    lv_cmd->head.subID = _sid;                      // 子节点ID
    lv_cmd->head.paramLen = (_lenPayl < 12 ? _lenPayl : 12);    // 指定长度
    // 复制payload内容
    memcpy(lv_cmd->body.data, _pPayl, lv_cmd->head.paramLen);

    // 节点类型
    lv_cmd->head.nodeType = (_nid == 255 ? _nt : 0);

    // 'tp'列表
    addTpList(_nid, lv_cmd, _tpNum, _tpList);

    // 下一条
    m_nCmdIndex++;
}

// 私有函数：添加'tp'列表
BOOL CActCmdInterface::addTpList(const UC _nid, cmdItem_t *_pCmd, const UC _tpNum, const UC *_tpList)
{
    if( _nid == 255 || IS_GROUP_NODEID(_nid) ) {    // 广播或组播消息才使用'tp'列表
        if( _tpNum <= 4 ) {                         // 'tp'列表长度（0-4）
            rSet_ActCmd_ldt(_pCmd->head, _tpNum);
            if( _tpNum > 0 ) {
                memcpy(_pCmd->body.data + _pCmd->head.paramLen, _tpList, _tpNum);
                _pCmd->head.paramLen += _tpNum;
            }
            return true;
        }
    }
    return false;
}

// 生成命令字符串：返回字符串指针和长度
US CActCmdInterface::Build(char **_pstrActCmd)
{
    // 计算crc16
    US lv_crc16 = usCRC16(m_msg + SCMSG_HEADER_LEN, m_nPayloadLen);
    m_msg[7] = lv_crc16 >> 8;
    m_msg[8] = lv_crc16 & 0xFF;

    // 字节数组 转 Hex字符串（注意：长度翻倍）
    ByteToString(m_msg, m_strOutput, GetMsgLen());

    // 返回字符串指针和长度
    *_pstrActCmd = m_strOutput;
    return(GetMsgStringLen());
}