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

// Act-cmd �����������ӿ�
class CActCmdInterface
{
public:
    CActCmdInterface();

    //------------------------------------------------------------------
    // Step 1
    //------------------------------------------------------------------
    // �ӿڳ�ʼ����ÿ�������µ�act-cmd�����б�ǰ����
    void Init(const UC _uid, const UC _cmdCnt);

    //------------------------------------------------------------------
    // Step 2
    //------------------------------------------------------------------
    // ����������״̬�ı�����
    void AddCmd_SetState(const UC _nid, const UC _sid, const UC _type, const UC _state, const UC _delayUnit = 0, const UC _delayTime = 0, const UC _nt = 0, const UC _tpNum = 0, const UC *_tpList = NULL);

    // �����������л������ʱNodeID�����ã�
    void AddCmd_ChangeScenario(const UC _scenario);

    // ���������ȿ�������
    void AddCmd_FanControl(const UC _nid, const UC _sid, const UC _speed, const UC _nt = 0, const UC _tpNum = 0, const UC *_tpList = NULL);

    // ���������ڵƵ�����
    void AddCmd_BRControl(const UC _nid, const UC _sid, const UC _op, const UC _br, const UC _nt = 0, const UC _tpNum = 0, const UC *_tpList = NULL);

    // ���������ڵƵ�ɫ��
    void AddCmd_CCTControl(const UC _nid, const UC _sid, const UC _op, const US _cct, const UC _nt = 0, const UC _tpNum = 0, const UC *_tpList = NULL);

    // ���������ڵ����Ⱥ�ɫ������
    void AddCmd_LightControl(const UC _nid, const UC _sid, const UC _sw, const UC _br, const US _cct, const UC _nt = 0, const UC _tpNum = 0, const UC *_tpList = NULL);

    // �������·����
    void AddCmd_APControl(const UC _nid, const UC _sid, const UC _sw, const UC _speed, const UC _nt = 0, const UC _tpNum = 0, const UC *_tpList = NULL);

    // �������յ�����
    void AddCmd_ACControl(const UC _nid, const UC _sid, const UC _sw, const UC _mode, const UC _temp, const UC _speed, const US _code = 0, const UC _fantp_light = 0, const UC _nt = 0, const UC _tpNum = 0, const UC *_tpList = NULL);

    // �������̵���(����)�������
    void AddCmd_RelayString(const UC _nid, const UC _sid, const UC _type, const UC _strLen, const char *_pStr, const UC _nt = 0, const UC _tpNum = 0, const UC *_tpList = NULL);

    // �������ͻ�����Ϣ
    void AddCmd_CustomMessage(const UC _nid, const UC _sid, const UC _cmd, const UC _type, const UC *_pPayl, const UC _lenPayl, const UC _nt = 0, const UC _tpNum = 0, const UC *_tpList = NULL);

    //------------------------------------------------------------------
    // Step 3
    //------------------------------------------------------------------
    // ���������ַ����������ַ���ָ��ͳ���
    US Build(char **_pstrActCmd);

    //------------------------------------------------------------------
    // ����uid
    UC GetUID() { return m_msg[6]; }
    // ����act-cmd�б��е���������
    UC GetCmdCount() { return m_nCmdCnt; }

    // ����act-cmd�б���ֽ������������� * һ�������ֽ�����18�ֽڣ�
    US GetPayloadLen() { return m_nPayloadLen; }

    // ����������Ϣ�ܳ��ȣ�GetPayloadLen() + ������Ϣͷ���ȣ�9�ֽڣ�
    US GetMsgLen() { return(m_nPayloadLen + SCMSG_HEADER_LEN); }

    // �������ɵ������ַ������ȣ�������Ϣ�ܳ��� * 2
    US GetMsgStringLen() { return strlen(m_strOutput); }

private:
    // ���'tp'�б�
    BOOL addTpList(const UC _nid, cmdItem_t *_pCmd, const UC _tpNum, const UC *_tpList);

    UC m_nCmdCnt;               // act-cmd�б��е���������
    UC m_nCmdIndex;             // ��ǰ������ţ���0��ʼ����_cmdCnt - 1�����������
    US m_nPayloadLen;           // act-cmd�б���ֽ��� = m_nCmdCnt * sizeof(cmdItem_t)

    //------------------------------------------------------------------
    // ��Ϣ���ݽṹ
    UC m_msg[SCMSG_HEADER_LEN + MAX_COMMAND_PER_ACTION * sizeof(cmdItem_t)];
    cmdItem_t *m_pCmdList;
    char m_strOutput[sizeof(m_msg) * 2 + 1];    
};

#endif  // __XLX_ACT_CMD_INTERFACE_H__