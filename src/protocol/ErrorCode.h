/**
 * �ļ�: ErrorCode.h
 * ��Ҫ: ������
 * ��������ͷ�ļ���Ҫ���ڱ�ʾЭ��ͨ�ź�ϵͳ���й����в����Ĵ���
 * ���ߣ�queen
 * �汾��v00.01
 * ���ڣ�2018/10/27
 * ��ע������ֲ㣬������󷵻ص�������룬�²���󷵻��²�����롣
 *		 �����ֽڱ�ʾ�� ���ֽ�Ϊ�����幦�ܲ㣨0-63��ϵͳ�㡢64-127��Ӳ�������㡢128-191������㡢192-255��Ӧ�ò㣩	
 *						���ֽ�Ϊ����������
 */
 
#ifndef __ERRORCODE_H__
#define __ERRORCODE_H__

typedef enum _DP_ERROR_CODE_E
{
	DP_SUCCESS	= 0x0000,					//�ɹ�	
	
	
	//ϵͳ��
	DP_ERR_SYS_	= 0x0001,
		//File����
		//Socket����
		//Net����
		
		
	//Ӳ����
	DP_ERR_UDRV_INIT = 0x4001,				//�ײ�Ӳ�������ʼ��ʧ��
	DP_ERR_UDRV_PARMS,						//�ײ�Ӳ�������������
	DP_ERR_UDRV_NOT_INIT,					//�ײ�Ӳ������δ��ʼ��
	DP_ERR_UDRV_NOT_SUPPORT,				//�ײ�Ӳ������֧�ִ˲���

	
	//�����
	DP_ERR_VASERVER_PARAM_INVALID =	0x8001,	//����Ƶ�������ò�����Ч
	DP_ERR_VASERVER_PARAM_ILLEGAL,			//����Ƶ�������ò����Ƿ�
	DP_ERR_VASERVER_INIT,					//����Ƶ�����ʼ��ʧ��
	DP_ERR_VASERVER_NOT_INIT,				//����Ƶ����δ��ʼ��
	DP_ERR_VASERVER_NOT_SUPPORT,			//����Ƶ����֧�ִ˲���
	
	DP_ERR_RTSP_OPERATE = 0x8101,			//RTSP����ʧ�ܡ��ڲ�ԭ��Ĳ���ʧ��
	DP_ERR_RTSP_PARAM_INVALID,				//RTSP����������
	DP_ERR_RTSP_PARAM_ILLEGAL,				//RTSP�����Ƿ�
	DP_ERR_RTSP_URL_LEN_OVERFLOW,			//RTSP����URL����Խ��
	
	DP_ERR_AI_OPERATE = 0x8201,				//��Ƶ�����豸����ʧ�ܡ��ڲ�ԭ��Ĳ���ʧ��
	DP_ERR_AI_ID_INVALID,					//��Ƶ�����豸ID������
	DP_ERR_AI_PARAM_ILLEGAL,				//��Ƶ�������ò�����Ч
	DP_ERR_AI_PARAM_ILLEGAL,				//��Ƶ�������ò����Ƿ�
	
	DP_ERR_VI_OPERATE = 0x8301,				//��Ƶ�����豸����ʧ�ܡ��ڲ�ԭ��Ĳ���ʧ��
	DP_ERR_VI_ID_INVALID,					//��Ƶ�����豸ID������
	DP_ERR_VI_PARAM_ILLEGAL,				//��Ƶ�������ò�����Ч
	DP_ERR_VI_PARAM_ILLEGAL,				//��Ƶ�������ò����Ƿ�
	
	DP_ERR_AENC_OPERATE = 0x8401,			//��Ƶ����ͨ������ʧ�ܡ��ڲ�ԭ��Ĳ���ʧ��
	DP_ERR_AENC_ID_INVALID,					//��Ƶ����ͨ��ID������
	DP_ERR_AENC_PARAM_INVALID,				//��Ƶ����ͨ�����ò�����Ч
	DP_ERR_AENC_PARAM_ILLEGAL,				//��Ƶ����ͨ�����ò����Ƿ�

	DP_ERR_VENC_OPERATE = 0x8501,			//��Ƶ����ͨ������ʧ�ܡ��ڲ�ԭ��Ĳ���ʧ��
	DP_ERR_VENC_ID_INVALID,					//��Ƶ����ͨ��ID������
	DP_ERR_VENC_PARAM_INVALID,				//��Ƶ����ͨ�����ò�����Ч
	DP_ERR_VENC_PARAM_ILLEGAL,				//��Ƶ����ͨ�����ò����Ƿ�
	
	DP_ERR_ADEC_OPERATE = 0x8601,			//��Ƶ����ͨ������ʧ�ܡ��ڲ�ԭ��Ĳ���ʧ��
	DP_ERR_ADEC_ID_INVALID,					//��Ƶ����ͨ��ID������
	DP_ERR_ADEC_PARAM_INVALID,				//��Ƶ����ͨ�����ò�����Ч
	DP_ERR_ADEC_PARAM_ILLEGAL,				//��Ƶ����ͨ�����ò����Ƿ�

	DP_ERR_VDEC_OPERATE = 0x8701,			//��Ƶ����ͨ������ʧ�ܡ��ڲ�ԭ��Ĳ���ʧ��
	DP_ERR_VDEC_ID_INVALID,					//��Ƶ����ͨ��ID������
	DP_ERR_VDEC_PARAM_INVALID,				//��Ƶ����ͨ�����ò�����Ч
	DP_ERR_VDEC_PARAM_ILLEGAL,				//��Ƶ����ͨ�����ò����Ƿ�
	
	DP_ERR_AO_OPERATE = 0x8801,				//��Ƶ����豸����ʧ�ܡ��ڲ�ԭ��Ĳ���ʧ��
	DP_ERR_AO_ID_INVALID,					//��Ƶ����豸ID������
	DP_ERR_AO_PARAM_ILLEGAL,				//��Ƶ������ò�����Ч
	DP_ERR_AO_PARAM_ILLEGAL,				//��Ƶ������ò����Ƿ�
	
	DP_ERR_VO_OPERATE = 0x8901,				//��Ƶ����豸����ʧ�ܡ��ڲ�ԭ��Ĳ���ʧ��
	DP_ERR_VO_ID_INVALID,					//��Ƶ����豸ID������
	DP_ERR_VO_PARAM_ILLEGAL,				//��Ƶ������ò�����Ч
	DP_ERR_VO_PARAM_ILLEGAL,				//��Ƶ������ò����Ƿ�

	
	//Ӧ�ò�
	DP_ERR_PROTOCOL_PRASE =	0xC001,			//Э��������󡣰�ͷ������ʽ����
	DP_ERR_PROTOCOL_PRASE_HEAD,				//Э���ͷ����
	DP_ERR_PROTOCOL_PRASE_DESTINATION,		//Э��Ŀ�ĵ�ַ����
	DP_ERR_PROTOCOL_PRASE_PACKET_TYPE,		//Э������ʹ���
	DP_ERR_PROTOCOL_TOTALLEN,				//Э�����������Ҫ������ʵ�ʳ��Ȳ�ƥ��
	DP_ERR_PROTOCOL_CONTENT,				//Э������ݴ���Э������У�����
	DP_ERR_PROTOCOL_VERSION,				//Э��汾����	
	DP_ERR_PROTOCOL_PARAM_INVALID,			//Э�����������Ч��Э������Ĳ������ڷ�Χ�ڡ�
	DP_ERR_PROTOCOL_PARAM_ILLEGAL,			//Э��������÷Ƿ���Э������Ĳ�������
	DP_ERR_PROTOCOL_PARAM_CMD,				//Э�鹦�������������
	
	DP_ERR_LOGIN_USERNAME = 0xC101,			//�û���������
	DP_ERR_LOGIN_PASSWORD,					//�������
	DP_ERR_USER_PERMISSION,					//�û�Ȩ�޲���	
	
	DP_ERR_MODE_INVALID = 0xC201,			//ģʽ������
	DP_ERR_FUNCTION_INVALID,				//���ܲ�����
	
	DP_ERR_TIME_OPERATE = 0xC301,			//ʱ�����ʧ�ܡ��ڲ�ԭ��Ĳ���ʧ��
	DP_ERR_TIME_PARAM_ILLEGAL,				//ʱ��ֵ�Ƿ�
	DP_ERR_TIME_PARAM_OVERFLOW,				//ʱ��ֵԽ�硣�·ݴ���12���ճ�������������ʱ����24���ֳ���60���볬��60
	
	DP_ERR_NET_OPERATE = 0xC401,			//�������ʧ�ܡ��ڲ�ԭ��Ĳ���ʧ��
	DP_ERR_NET_PARAM_ILLEGAL,				//�������ò����Ƿ���������ĸ�����š�ȱ�λ��Ρ�
	DP_ERR_NET_PARAM_OVERFLOW,				//�������ò���ֵԽ�硣��������255.
	DP_ERR_NET_MAC_ILLEGAL,					//MAC��ַ�Ƿ��������쳣��ĸ��ȱ�λ��Ρ�
	DP_ERR_NET_IP_UNREASONABLE,				//IP�������׶�Ϊ0��ȫ����Ϊ255��
	DP_ERR_NET_IP_CONFLICT,					//IP��ͻ��
	DP_ERR_NET_MASK_UNREASONABLE,			//�������벻������ʽ������IP��ַ������ϵ����
	DP_ERR_NET_GATEWAY_UNREASONABLE,		//���ز���������ǰ���κ�IP��ǰ���β�ͬ��
	
	DP_ERR_CONTROL_OPERATE = 0xC501,		//�пز���ʧ�ܡ��ڲ�ԭ��Ĳ���ʧ��
	DP_ERR_CONTROL_IO_INVALID,				//IO������
	DP_ERR_CONTROL_IO_ILLEGAL,				//IO����ֵ�Ƿ�
	DP_ERR_CONTROL_IR_INVALID,				//IR������
	DP_ERR_CONTROL_IR_ILLEGAL,				//IR����ֵ�Ƿ�
	DP_ERR_CONTROL_UART_INVALID,			//UART������
	DP_ERR_CONTROL_UART_ILLEGAL,			//UART��ʼ�������Ƿ�
		
	DP_ERR_TASK_OPERATE = 0xC601,			//�������ʧ�ܡ��ڲ�ԭ��Ĳ���ʧ��
	DP_ERR_TASK_PARAM_INVALID,				//�������������
	DP_ERR_TASK_PARAM_ILLEGAL,				//��������Ƿ�
	DP_ERR_TASK_ID_NOTEXIST,				//����ID������
	DP_ERR_TASK_DEVID_NOTEXIST,				//�����豸ID������
	DP_ERR_TASK_SOURCE_NOTENOUGH,			//������Դ����
		
} DP_ERROR_CODE_E;

#endif



