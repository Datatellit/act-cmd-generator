#include "actcmdInterface.h"
#include "MyMessage.h"

int main()
{
    printf("\r\n------------ act-cmd-generator ----------------\r\n");
    CActCmdInterface lv_actCmd;
    UC lv_tp[4] = {0};
    char *lv_str;

    //------------------------------------------------------------------
    // Step 1: 初始化，请输入参数
    // --> 要生成Act-cmd命令组的uid，需要与Action uid对应(1到64之间)
    // --> 要生成Act-cmd命令组包含的命令数量(1至16之间)
    // --> 示例：uid = 1, cmdCount = 3
    //lv_actCmd.Init(1, 3);

    //------------------------------------------------------------------
    // Step 2: 依次添加命令，总数就是上面初始化时输入的
    // --> 示例：第1条命令，调节灯节点8亮度和色温
    //lv_actCmd.AddCmd_LightControl(8, 0, DEVICE_SW_ON, 65, 3500);
    
    lv_actCmd.Init(1, 1);
    //lv_actCmd.AddCmd_LightControl(9, 0, DEVICE_SW_ON, 10, 2700);
    //43460101001201C63709092900000500010A8C0A00000000000000
    //lv_actCmd.AddCmd_LightControl(9, 0, DEVICE_SW_ON, 80, 6500);
    //43460101001201FEF4090929000005000250641900000000000000
    //lv_actCmd.AddCmd_LightControl(255, 0, DEVICE_SW_ON, 80, 5500, NODE_TYP_LAMP);
    // 434601010012019C8EFF29290000060001507C1501000000000000

    //lv_actCmd.Init(2, 2);
    //lv_actCmd.AddCmd_BRControl(9, 0, OPERATOR_ADD, 50);
    //lv_actCmd.AddCmd_CCTControl(9, 0, OPERATOR_ADD, 3800);
    //4346010100240232A109090300000201320000000000000000000009092500000301D80E000000000000000000

    //lv_actCmd.Init(3, 2);
    //lv_actCmd.AddCmd_BRControl(9, 0, OPERATOR_ADD, 40);
    //lv_actCmd.AddCmd_CCTControl(9, 0, OPERATOR_SUB, 3800);
    //43460101002403272509090300000201280000000000000000000009092500000302D80E000000000000000000

    //lv_actCmd.Init(4, 2);
    //lv_actCmd.AddCmd_BRControl(9, 0, OPERATOR_SUB, 40);
    //lv_actCmd.AddCmd_CCTControl(9, 0, OPERATOR_ADD, 3800);
    //43460101002404D32209090300000202280000000000000000000009092500000301D80E000000000000000000

    //lv_actCmd.Init(5, 3);
    //lv_actCmd.AddCmd_BRControl(9, 0, OPERATOR_SUB, 50);
    //lv_actCmd.AddCmd_CCTControl(9, 0, OPERATOR_SUB, 3800);
    //43460101002405C6A609090300000202320000000000000000000009092500000302D80E000000000000000000
    //lv_actCmd.AddCmd_BRControl(8, 0, OPERATOR_SET, 50);
    //lv_actCmd.AddCmd_CCTControl(8, 0, OPERATOR_SET, 3800);
    //lv_actCmd.AddCmd_BRControl(9, 0, OPERATOR_SET, 65);
    //4346010100240580CD08090300000200320000000000000000000008092500000300D80E000000000000000000
    //4346010100360539DC08090300000200320000000000000000000008092500000300D80E000000000000000000090903000002004100000000000000000000

    //lv_actCmd.Init(6, 10);
    //lv_actCmd.AddCmd_SetState(9, 0, V_STATUS, DEVICE_SW_OFF);
    //43460101001206C8F7090902000001000000000000000000000000
    /*
    lv_actCmd.AddCmd_LightControl(8, 0, DEVICE_SW_ON, 80, 6500);
    lv_actCmd.AddCmd_LightControl(9, 0, DEVICE_SW_ON, 80, 6500);
    lv_actCmd.AddCmd_LightControl(10, 0, DEVICE_SW_ON, 80, 6500);
    lv_actCmd.AddCmd_LightControl(11, 0, DEVICE_SW_ON, 80, 6500);
    lv_actCmd.AddCmd_LightControl(12, 0, DEVICE_SW_ON, 80, 6500);
    lv_actCmd.AddCmd_LightControl(13, 0, DEVICE_SW_ON, 80, 6500);
    lv_actCmd.AddCmd_LightControl(14, 0, DEVICE_SW_ON, 80, 6500);
    lv_actCmd.AddCmd_LightControl(15, 0, DEVICE_SW_ON, 80, 6500);
    lv_actCmd.AddCmd_LightControl(16, 0, DEVICE_SW_ON, 80, 6500);
    lv_actCmd.AddCmd_LightControl(17, 0, DEVICE_SW_ON, 80, 6500);
    */

    // --> 示例：延时15分钟关灯
    lv_actCmd.AddCmd_SetState(9, 0, V_STATUS, DEVICE_SW_OFF, TM_MINUTE_UNIT, 15);

    // --> 示例：第2条命令，调节全部空调节点
    //lv_actCmd.AddCmd_ACControl(255, 0, 1, 1, 23, 1, 237, 0, NODE_TYP_AC);

    // --> 示例：第3条命令，关闭全部风扇、新风节点
    //lv_tp[0] = NODE_TYP_FAN;
    //lv_tp[1] = NODE_TYP_AIRPURE;
    //lv_actCmd.AddCmd_SetState(255, 0, V_STATUS, DEVICE_SW_OFF, 0, 0, 0, 2, lv_tp);

    // --> 示例：客户化命令，继电器控制序列（e.g. 窗帘控制）
    //const UC lv_payload[] = {5, 'p', '2', '_', 'p', '3'};
    //const UC lv_payLoadLen = sizeof(lv_payload);
    //lv_actCmd.AddCmd_CustomMessage(148, 0, C_SET, V_DOWN, lv_payload, lv_payLoadLen);

    //...更多命令...
    //
    
    //------------------------------------------------------------------
    // Step 3: 生成消息
    lv_actCmd.Build(&lv_str);

    //------------------------------------------------------------------
    // 结果输出、显示
    printf("uid: %d, cmd-count: %d, msg-len: %d, payload-len: %d, streamStr-len: %d\r\n", lv_actCmd.GetUID(), 
        lv_actCmd.GetCmdCount(), lv_actCmd.GetMsgLen(), lv_actCmd.GetPayloadLen(), lv_actCmd.GetMsgStringLen());
    printf("streamStr: %s\r\n", lv_str);
    //getchar();
}
