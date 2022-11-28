#ifndef __XLX_ACT_CMD_INTERFACE_H__
#define __XLX_ACT_CMD_INTERFACE_H__

#include "xlxConfig_Action.h"

#define VERSION_ACTION_COMMAND_API      0x12        // V1.2

// Message Format:
//   0    1        2           3          4       5      6        7           8     9 -
// ['C']['F'][total_pack][current_pack][len_h][len_low][uid][crc16_high][crc16_low][cmdlist data: sizeof(cmdItem_t) * _cmdCnt]
#define SCMSG_HEADER_LEN                9
#define ACT_CMD_MSG_HEAD_0              'C'
#define ACT_CMD_MSG_HEAD_1              'F'

// Act-cmd 命令生成器接口
class CActCmdInterface
{
public:
    CActCmdInterface();

    //------------------------------------------------------------------
    // Step 1
    //------------------------------------------------------------------
    // 接口初始化：每次生成新的act-cmd命令列表前调用
    void Init(const UC _uid, const UC _cmdCnt);

    //------------------------------------------------------------------
    // Step 2
    //------------------------------------------------------------------
    // 添加命令：开关状态改变命令
    void AddCmd_SetState(const UC _nid, const UC _sid, const UC _type, const UC _state, const UC _delayUnit = 0, const UC _delayTime = 0, const UC _nt = 0, const UC _tpNum = 0, const UC *_tpList = NULL);

    // 添加命令：场景切换命令（此时NodeID无作用）
    void AddCmd_ChangeScenario(const UC _scenario);

    // 添加命令：风扇控制命令
    void AddCmd_FanControl(const UC _nid, const UC _sid, const UC _speed, const UC _nt = 0, const UC _tpNum = 0, const UC *_tpList = NULL);

    // 添加命令：调节灯的亮度
    void AddCmd_BRControl(const UC _nid, const UC _sid, const UC _op, const UC _br, const UC _nt = 0, const UC _tpNum = 0, const UC *_tpList = NULL);

    // 添加命令：调节灯的色温
    void AddCmd_CCTControl(const UC _nid, const UC _sid, const UC _op, const US _cct, const UC _nt = 0, const UC _tpNum = 0, const UC *_tpList = NULL);

    // 添加命令：调节灯亮度和色温命令
    void AddCmd_LightControl(const UC _nid, const UC _sid, const UC _sw, const UC _br, const US _cct, const UC _nt = 0, const UC _tpNum = 0, const UC *_tpList = NULL);

    // 添加命令：新风控制
    void AddCmd_APControl(const UC _nid, const UC _sid, const UC _sw, const UC _speed, const UC _nt = 0, const UC _tpNum = 0, const UC *_tpList = NULL);

    // 添加命令：空调控制
    void AddCmd_ACControl(const UC _nid, const UC _sid, const UC _sw, const UC _mode, const UC _temp, const UC _speed, const US _code = 0, const UC _fantp_light = 0, const UC _nt = 0, const UC _tpNum = 0, const UC *_tpList = NULL);

    // 添加命令：继电器(窗帘)操作组合
    void AddCmd_RelayString(const UC _nid, const UC _sid, const UC _type, const UC _strLen, const char *_pStr, const UC _nt = 0, const UC _tpNum = 0, const UC *_tpList = NULL);

    // 添加命令：客户化消息
    void AddCmd_CustomMessage(const UC _nid, const UC _sid, const UC _cmd, const UC _type, const UC *_pPayl, const UC _lenPayl, const UC _nt = 0, const UC _tpNum = 0, const UC *_tpList = NULL);

    //------------------------------------------------------------------
    // Step 3
    //------------------------------------------------------------------
    // 生成命令字符串：返回字符串指针和长度
    US Build(char **_pstrActCmd);

    //------------------------------------------------------------------
    // 返回uid
    UC GetUID() { return m_msg[6]; }
    // 返回act-cmd列表中的命令数量
    UC GetCmdCount() { return m_nCmdCnt; }

    // 返回act-cmd列表的字节数：命令数量 * 一条命令字节数（18字节）
    US GetPayloadLen() { return m_nPayloadLen; }

    // 返回命令消息总长度：GetPayloadLen() + 命令消息头长度（9字节）
    US GetMsgLen() { return(m_nPayloadLen + SCMSG_HEADER_LEN); }

    // 返回生成的命令字符串长度：命令消息总长度 * 2
    US GetMsgStringLen() { return strlen(m_strOutput); }

private:
    // 添加'tp'列表
    BOOL addTpList(const UC _nid, cmdItem_t *_pCmd, const UC _tpNum, const UC *_tpList);

    UC m_nCmdCnt;               // act-cmd列表中的命令数量
    UC m_nCmdIndex;             // 当前命令序号，从0开始，到_cmdCnt - 1，请依次添加
    US m_nPayloadLen;           // act-cmd列表的字节数 = m_nCmdCnt * sizeof(cmdItem_t)

    //------------------------------------------------------------------
    // 消息数据结构
    UC m_msg[SCMSG_HEADER_LEN + MAX_COMMAND_PER_ACTION * sizeof(cmdItem_t)];
    cmdItem_t *m_pCmdList;
    char m_strOutput[sizeof(m_msg) * 2 + 1];    
};

#endif  // __XLX_ACT_CMD_INTERFACE_H__