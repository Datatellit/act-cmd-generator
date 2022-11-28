#include "actcmdInterface.h"
#include "MyMessage.h"

CActCmdInterface::CActCmdInterface()
{
    m_pCmdList = (cmdItem_t *)(m_msg + SCMSG_HEADER_LEN);
}

// �ӿڳ�ʼ����ÿ�������µ�act-cmd�����б�ǰ����
void CActCmdInterface::Init(const UC _uid, const UC _cmdCnt)
{
    // ���ݳ�ʼ��
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
* @brief    ����������״̬�ı�����
* @param
*   _nid���ڵ�ID������ڵ����255��255��ʾ�㲥����ʱͨ��NodeTypeɸѡĿ��ڵ�
*   _type���������ͣ�������V_STATUS, V_RELAY_ON, V_RELAY_OFF, V_RELAY_MAP��
*   _state������״ֵ̬
*   _delayUnit����ʱ������λ��0-�ޣ�1-�룬2-���ӣ�3-Сʱ��
*   _delayTime����ʱ����ʱ�䡣ע�⣺Ŀǰ���Եƿؽڵ���Ч
*   _nt���ڵ�����(Node Type)������_nid=255ʱ��Ч��Ĭ��Ϊ0
*   _tpNum��'tp'�б��ȣ�NodeType �� DevType����0 �� 4��Ĭ��Ϊ0
*   _tpList��'tp'�б�ָ�룬Ĭ��Ϊ��
* @retval	
***************************************************************/
void CActCmdInterface::AddCmd_SetState(const UC _nid, const UC _sid, const UC _type, const UC _state, const UC _delayUnit, const UC _delayTime, const UC _nt, const UC _tpNum, const UC *_tpList)
{
    // ָ��ǰ����
    cmdItem_t *lv_cmd = m_pCmdList + m_nCmdIndex;

    lv_cmd->head.nid = _nid;                        // �ڵ�ID
    rSet_ActCmd_cmd(lv_cmd->head, C_SET);           // RF����
    rSet_ActCmd_rck(lv_cmd->head, 1);               // ��Ҫ��Ӧ
    lv_cmd->head.sensorType = _type;                // ��������
    lv_cmd->head.subID = _sid;                      // �ӽڵ�ID
    if(_type == V_STATUS) {
        lv_cmd->head.paramLen = sizeof(V_STATUS_t);     // ����״̬�ı������������
        lv_cmd->body.status.sw = _state;
        lv_cmd->body.status.delayopUnit = _delayUnit;
        lv_cmd->body.status.delayopTime = _delayTime;
    } else {
        lv_cmd->head.paramLen = 1;
        lv_cmd->body.state = _state;
    }

    // �ڵ�����
    lv_cmd->head.nodeType = (_nid == 255 ? _nt : 0);

    // 'tp'�б�
    addTpList(_nid, lv_cmd, _tpNum, _tpList);

    // ��һ��
    m_nCmdIndex++;
}

/***************************************************************
* @func		AddCmd_ChangeScenario
* @brief    �����������л������ʱNodeID�����ã�
* @param    _scenario������ID
* @retval	
***************************************************************/
void CActCmdInterface::AddCmd_ChangeScenario(const UC _scenario)
{
    // ָ��ǰ����
    cmdItem_t *lv_cmd = m_pCmdList + m_nCmdIndex;

    lv_cmd->head.nid = 0;                           // �ڵ�ID
    rSet_ActCmd_cmd(lv_cmd->head, C_SET);           // RF����
    rSet_ActCmd_rck(lv_cmd->head, 1);               // ��Ҫ��Ӧ
    lv_cmd->head.sensorType = V_SCENE_ON;           // �л�����
    lv_cmd->head.subID = 0;                         // �ӽڵ�ID��Ŀǰ��ʹ�ã�
    lv_cmd->head.paramLen = 1;                      // �����л������������
    lv_cmd->body.scenario_id = _scenario;

    // ��һ��
    m_nCmdIndex++;
}

/***************************************************************
* @func		AddCmd_FanControl
* @brief    ���������ȿ�������
* @param
*   _nid���ڵ�ID������ڵ����255��255��ʾ�㲥����ʱͨ��NodeTypeɸѡĿ��ڵ�
*   _speed�����ȿ��غͷ���
*   _nt���ڵ�����(Node Type)������_nid=255ʱ��Ч��Ĭ��Ϊ0
*   _tpNum��'tp'�б��ȣ�NodeType �� DevType����0 �� 4��Ĭ��Ϊ0
*   _tpList��'tp'�б�ָ�룬Ĭ��Ϊ��
* @retval	
***************************************************************/
void CActCmdInterface::AddCmd_FanControl(const UC _nid, const UC _sid, const UC _speed, const UC _nt, const UC _tpNum, const UC *_tpList)
{
    // ָ��ǰ����
    cmdItem_t *lv_cmd = m_pCmdList + m_nCmdIndex;

    lv_cmd->head.nid = _nid;                        // �ڵ�ID
    rSet_ActCmd_cmd(lv_cmd->head, C_SET);           // RF����
    rSet_ActCmd_rck(lv_cmd->head, 1);               // ��Ҫ��Ӧ
    lv_cmd->head.sensorType = V_FAN;                // ��������
    lv_cmd->head.subID = _sid;                      // �ӽڵ�ID
    lv_cmd->head.paramLen = 1;                      // ���ȿ��������������
    lv_cmd->body.fan_speed = _speed;

    // �ڵ�����
    lv_cmd->head.nodeType = (_nid == 255 ? _nt : 0);

    // 'tp'�б�
    addTpList(_nid, lv_cmd, _tpNum, _tpList);

    // ��һ��
    m_nCmdIndex++;
}

/***************************************************************
* @func		AddCmd_BRControl
* @brief    ���������ڵƵ�����
* @param
*   _nid���ڵ�ID������ڵ����255��255��ʾ�㲥����ʱͨ��NodeTypeɸѡĿ��ڵ�
*   _op�����Ʒ�ʽ��0-��������ֵ��1-�������ȣ����ֵ����2-�������ȣ����ֵ����
*   _br�����ȣ����Ⱦ���ֵ10-100
*   _nt���ڵ�����(Node Type)������_nid=255ʱ��Ч��Ĭ��Ϊ0
*   _tpNum��'tp'�б��ȣ�NodeType �� DevType����0 �� 4��Ĭ��Ϊ0
*   _tpList��'tp'�б�ָ�룬Ĭ��Ϊ��
* @retval	
***************************************************************/
void CActCmdInterface::AddCmd_BRControl(const UC _nid, const UC _sid, const UC _op, const UC _br, const UC _nt, const UC _tpNum, const UC *_tpList)
{
    // ָ��ǰ����
    cmdItem_t *lv_cmd = m_pCmdList + m_nCmdIndex;

    lv_cmd->head.nid = _nid;                        // �ڵ�ID
    rSet_ActCmd_cmd(lv_cmd->head, C_SET);           // RF����
    rSet_ActCmd_rck(lv_cmd->head, 1);               // ��Ҫ��Ӧ
    lv_cmd->head.sensorType = V_PERCENTAGE;         // ��������
    lv_cmd->head.subID = _sid;                      // �ӽڵ�ID
    lv_cmd->head.paramLen = sizeof(V_PERCENTAGE_t); // ���ȵ��������������
    lv_cmd->body.percentage.type = _op;
    lv_cmd->body.percentage.br = _br;

    // �ڵ�����
    lv_cmd->head.nodeType = (_nid == 255 ? _nt : 0);

    // 'tp'�б�
    addTpList(_nid, lv_cmd, _tpNum, _tpList);

    // ��һ��
    m_nCmdIndex++;
}

/***************************************************************
* @func		AddCmd_CCTControl
* @brief    ���������ڵƵ�ɫ��
* @param
*   _nid���ڵ�ID������ڵ����255��255��ʾ�㲥����ʱͨ��NodeTypeɸѡĿ��ڵ�
*   _op�����Ʒ�ʽ��0-����ɫ��ֵ��1-����ɫ�£����ֵ����2-����ɫ�£����ֵ����
*   _cct��ɫ�£�����ɫ��ֵ2700-6500ռ2���ֽڣ���8λ��ǰ����8λ�ں�
*   _nt���ڵ�����(Node Type)������_nid=255ʱ��Ч��Ĭ��Ϊ0
*   _tpNum��'tp'�б��ȣ�NodeType �� DevType����0 �� 4��Ĭ��Ϊ0
*   _tpList��'tp'�б�ָ�룬Ĭ��Ϊ��
* @retval	
***************************************************************/
void CActCmdInterface::AddCmd_CCTControl(const UC _nid, const UC _sid, const UC _op, const US _cct, const UC _nt, const UC _tpNum, const UC *_tpList)
{
    // ָ��ǰ����
    cmdItem_t *lv_cmd = m_pCmdList + m_nCmdIndex;

    lv_cmd->head.nid = _nid;                        // �ڵ�ID
    rSet_ActCmd_cmd(lv_cmd->head, C_SET);           // RF����
    rSet_ActCmd_rck(lv_cmd->head, 1);               // ��Ҫ��Ӧ
    lv_cmd->head.sensorType = V_LEVEL;              // ��������
    lv_cmd->head.subID = _sid;                      // �ӽڵ�ID
    lv_cmd->head.paramLen = sizeof(V_LEVEL_t);      // ɫ�µ��������������
    lv_cmd->body.cct.type = _op;
    lv_cmd->body.cct.cct = _cct;

    // �ڵ�����
    lv_cmd->head.nodeType = (_nid == 255 ? _nt : 0);

    // 'tp'�б�
    addTpList(_nid, lv_cmd, _tpNum, _tpList);

    // ��һ��
    m_nCmdIndex++;
}

/***************************************************************
* @func		AddCmd_LightControl
* @brief    ���������ڵ����Ⱥ�ɫ������
* @param
*   _nid���ڵ�ID������ڵ����255��255��ʾ�㲥����ʱͨ��NodeTypeɸѡĿ��ڵ�
*   _sw�����أ�0-�أ�1-����2-�л���
*   _br�����ȣ����Ⱦ���ֵ10-100
*   _cct��ɫ�£�����ɫ��ֵ2700-6500ռ2���ֽڣ���8λ��ǰ����8λ�ں�
*   _nt���ڵ�����(Node Type)������_nid=255ʱ��Ч��Ĭ��Ϊ0
*   _tpNum��'tp'�б��ȣ�NodeType �� DevType����0 �� 4��Ĭ��Ϊ0
*   _tpList��'tp'�б�ָ�룬Ĭ��Ϊ��
* @retval	
***************************************************************/
void CActCmdInterface::AddCmd_LightControl(const UC _nid, const UC _sid, const UC _sw, const UC _br, const US _cct, const UC _nt, const UC _tpNum, const UC *_tpList)
{
    // ָ��ǰ����
    cmdItem_t *lv_cmd = m_pCmdList + m_nCmdIndex;

    lv_cmd->head.nid = _nid;                        // �ڵ�ID
    rSet_ActCmd_cmd(lv_cmd->head, C_SET);           // RF����
    rSet_ActCmd_rck(lv_cmd->head, 1);               // ��Ҫ��Ӧ
    lv_cmd->head.sensorType = V_RGBW;               // ��������
    lv_cmd->head.subID = _sid;                      // �ӽڵ�ID
    lv_cmd->head.paramLen = sizeof(V_RGBW_t);       // V_RGBW�����������
    lv_cmd->body.sw_br_cct.ring = 0;
    lv_cmd->body.sw_br_cct.sw = _sw;
    lv_cmd->body.sw_br_cct.br = _br;
    lv_cmd->body.sw_br_cct.cct = _cct;

    // �ڵ�����
    lv_cmd->head.nodeType = (_nid == 255 ? _nt : 0);

    // 'tp'�б�
    addTpList(_nid, lv_cmd, _tpNum, _tpList);

    // ��һ��
    m_nCmdIndex++;
}

/***************************************************************
* @func		AddCmd_APControl
* @brief    �������·��������
* @param
*   _nid���ڵ�ID������ڵ����255��255��ʾ�㲥����ʱͨ��NodeTypeɸѡĿ��ڵ�
*   _sw�����أ�0-������ 1-�� 2-��
*   _speed����λ��1-0��  2-1��  3-2�� 4-3��
*   _nt���ڵ�����(Node Type)������_nid=255ʱ��Ч��Ĭ��Ϊ0
*   _tpNum��'tp'�б��ȣ�NodeType �� DevType����0 �� 4��Ĭ��Ϊ0
*   _tpList��'tp'�б�ָ�룬Ĭ��Ϊ��
* @retval	
***************************************************************/
void CActCmdInterface::AddCmd_APControl(const UC _nid, const UC _sid, const UC _sw, const UC _speed, const UC _nt, const UC _tpNum, const UC *_tpList)
{
    // ָ��ǰ����
    cmdItem_t *lv_cmd = m_pCmdList + m_nCmdIndex;

    lv_cmd->head.nid = _nid;                        // �ڵ�ID
    rSet_ActCmd_cmd(lv_cmd->head, C_SET);           // RF����
    rSet_ActCmd_rck(lv_cmd->head, 1);               // ��Ҫ��Ӧ
    lv_cmd->head.sensorType = V_WIND;               // ��������
    lv_cmd->head.subID = _sid;                      // �ӽڵ�ID
    lv_cmd->head.paramLen = sizeof(V_WIND_t);       // �·���������������
    lv_cmd->body.wind.sw = _sw;
    lv_cmd->body.wind.speed = _speed;

    // �ڵ�����
    lv_cmd->head.nodeType = (_nid == 255 ? _nt : 0);

    // 'tp'�б�
    addTpList(_nid, lv_cmd, _tpNum, _tpList);

    // ��һ��
    m_nCmdIndex++;
}

/***************************************************************
* @func		AddCmd_ACControl
* @note     ע�⣺�յ�������Ҫ�����趨��
* @brief    �������յ���������
* @param
*   _nid���ڵ�ID������ڵ����255��255��ʾ�㲥����ʱͨ��NodeTypeɸѡĿ��ڵ�
*   _sw�����أ�0-�أ�1-����
*   _mode��ģʽ��0-�Զ���1-���䣬2-��ʪ��3-�ͷ磬4-���ȣ�
*   _temp���趨�¶ȣ�16-32��
*   _speed�����٣�0-�Զ���1-�ͣ�2-�У�3-��
*   _code�� �յ����룬��λ��ǰ
*   _fantp_light��0-3, bit0: ����ģʽ: 1-�Զ�����0-�ֶ�����bit1: ����: 0-�أ�1-��
*   _nt���ڵ�����(Node Type)������_nid=255ʱ��Ч��Ĭ��Ϊ0
*   _tpNum��'tp'�б��ȣ�NodeType �� DevType����0 �� 4��Ĭ��Ϊ0
*   _tpList��'tp'�б�ָ�룬Ĭ��Ϊ��
* @retval	
***************************************************************/
void CActCmdInterface::AddCmd_ACControl(const UC _nid, const UC _sid, const UC _sw, const UC _mode, const UC _temp, const UC _speed, const US _code, const UC _fantp_light, const UC _nt, const UC _tpNum, const UC *_tpList)
{
    // ָ��ǰ����
    cmdItem_t *lv_cmd = m_pCmdList + m_nCmdIndex;

    lv_cmd->head.nid = _nid;                        // �ڵ�ID
    rSet_ActCmd_cmd(lv_cmd->head, C_SET);           // RF����
    rSet_ActCmd_rck(lv_cmd->head, 1);               // ��Ҫ��Ӧ
    lv_cmd->head.sensorType = V_HVAC_FLOW_STATE;    // ��������
    lv_cmd->head.subID = _sid;                      // �ӽڵ�ID
    lv_cmd->head.paramLen = sizeof(V_HVAC_FLOW_STATE_t); // �յ����������������
    lv_cmd->body.hvac.code = _code;
    lv_cmd->body.hvac.sw = _sw;
    lv_cmd->body.hvac.mode = _mode;
    lv_cmd->body.hvac.temp = _temp;
    lv_cmd->body.hvac.speed = _speed;
    lv_cmd->body.hvac.fantp_light = _fantp_light;

    // �ڵ�����
    lv_cmd->head.nodeType = (_nid == 255 ? _nt : 0);

    // 'tp'�б�
    addTpList(_nid, lv_cmd, _tpNum, _tpList);

    // ��һ��
    m_nCmdIndex++;
}

/***************************************************************
* @func		AddCmd_RelayString
* @brief    �������̵���(����)�������
* @param
*   _nid���ڵ�ID������ڵ����255��255��ʾ�㲥����ʱͨ��NodeTypeɸѡĿ��ڵ�
*   _type���������ͣ�������V_UP, V_DOWN, V_STOP
*   _strLen���̵�����������ַ������ȣ������Payload��һ���ֽ�
*   _pStr���̵�����������ַ�������Payload�ĵڶ����ֽڿ�ʼ���μ����̵�����������ַ���˵��
*   _nt���ڵ�����(Node Type)������_nid=255ʱ��Ч��Ĭ��Ϊ0
*   _tpNum��'tp'�б��ȣ�NodeType �� DevType����0 �� 4��Ĭ��Ϊ0
*   _tpList��'tp'�б�ָ�룬Ĭ��Ϊ��
*  
*  �̵�����������ַ���˵����list of key pairs. One pair contains two charactors.
*  The first character stands for key operation style, e.g. press, quick-press, long-press, hold, release, double-click.
*  The second one is the key id ('0'-'9', 'a'-'z', 'A'-'Z'). 
*  If there are more than one key, the delimitator determines the interval between keys.
*  "+":no delay; ",":small pause; ".": normal pause; "=": long pause;  "~": very long pause; "_": same time
*
* @retval	
***************************************************************/
void CActCmdInterface::AddCmd_RelayString(const UC _nid, const UC _sid, const UC _type, const UC _strLen, const char *_pStr, const UC _nt, const UC _tpNum, const UC *_tpList )
{
    // ָ��ǰ����
    cmdItem_t *lv_cmd = m_pCmdList + m_nCmdIndex;

    lv_cmd->head.nid = _nid;                        // �ڵ�ID
    rSet_ActCmd_cmd(lv_cmd->head, C_SET);           // RF����
    rSet_ActCmd_rck(lv_cmd->head, 1);               // ��Ҫ��Ӧ
    lv_cmd->head.sensorType = _type;                // ��������
    lv_cmd->head.subID = _sid;                      // �ӽڵ�ID
    lv_cmd->head.paramLen = _strLen + 1;            // �ַ������ȼ��ϳ����ֽ�
    lv_cmd->body.data[0] = _strLen;
    memcpy(lv_cmd->body.data + 1, _pStr, _strLen);

    // �ڵ�����
    lv_cmd->head.nodeType = (_nid == 255 ? _nt : 0);

    // 'tp'�б�
    addTpList(_nid, lv_cmd, _tpNum, _tpList);

    // ��һ��
    m_nCmdIndex++;
}

/***************************************************************
* @func		AddCmd_CustomMessage
* @brief    �������ͻ�����Ϣ����û��������趨��Ϣ���ݣ���֧�ָ��ַǳ�����;
* @param
*   _nid���ڵ�ID������ڵ����255��255��ʾ�㲥����ʱͨ��NodeTypeɸѡĿ��ڵ�
*   _sid���ӽڵ�ID
*   _cmd��RF����
*   _type����������
*   _pPayl��payload���ݣ��ֽ����飩
*   _lenPayl��payload�ֽ���
*   _nt���ڵ�����(Node Type)������_nid=255ʱ��Ч��Ĭ��Ϊ0
*   _tpNum��'tp'�б��ȣ�NodeType �� DevType����0 �� 4��Ĭ��Ϊ0
*   _tpList��'tp'�б�ָ�룬Ĭ��Ϊ��
* @retval	
***************************************************************/
void CActCmdInterface::AddCmd_CustomMessage(const UC _nid, const UC _sid, const UC _cmd, const UC _type, const UC *_pPayl, const UC _lenPayl, const UC _nt, const UC _tpNum, const UC *_tpList)
{
    // ָ��ǰ����
    cmdItem_t *lv_cmd = m_pCmdList + m_nCmdIndex;

    lv_cmd->head.nid = _nid;                        // �ڵ�ID
    rSet_ActCmd_cmd(lv_cmd->head, _cmd);            // RF����
    rSet_ActCmd_rck(lv_cmd->head, 1);               // ��Ҫ��Ӧ
    lv_cmd->head.sensorType = _type;                // ��������
    lv_cmd->head.subID = _sid;                      // �ӽڵ�ID
    lv_cmd->head.paramLen = (_lenPayl < 12 ? _lenPayl : 12);    // ָ������
    // ����payload����
    memcpy(lv_cmd->body.data, _pPayl, lv_cmd->head.paramLen);

    // �ڵ�����
    lv_cmd->head.nodeType = (_nid == 255 ? _nt : 0);

    // 'tp'�б�
    addTpList(_nid, lv_cmd, _tpNum, _tpList);

    // ��һ��
    m_nCmdIndex++;
}

// ˽�к��������'tp'�б�
BOOL CActCmdInterface::addTpList(const UC _nid, cmdItem_t *_pCmd, const UC _tpNum, const UC *_tpList)
{
    if( _nid == 255 || IS_GROUP_NODEID(_nid) ) {    // �㲥���鲥��Ϣ��ʹ��'tp'�б�
        if( _tpNum <= 4 ) {                         // 'tp'�б��ȣ�0-4��
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

// ���������ַ����������ַ���ָ��ͳ���
US CActCmdInterface::Build(char **_pstrActCmd)
{
    // ����crc16
    US lv_crc16 = usCRC16(m_msg + SCMSG_HEADER_LEN, m_nPayloadLen);
    m_msg[7] = lv_crc16 >> 8;
    m_msg[8] = lv_crc16 & 0xFF;

    // �ֽ����� ת Hex�ַ�����ע�⣺���ȷ�����
    ByteToString(m_msg, m_strOutput, GetMsgLen());

    // �����ַ���ָ��ͳ���
    *_pstrActCmd = m_strOutput;
    return(GetMsgStringLen());
}