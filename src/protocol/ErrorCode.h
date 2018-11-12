/**
 * 文件: ErrorCode.h
 * 简要: 错误码
 * 详述：该头文件主要用于表示协议通信和系统运行过程中产生的错误。
 * 作者：queen
 * 版本：v00.01
 * 日期：2018/10/27
 * 备注：错误分层，当层错误返回当层错误码，下层错误返回下层错误码。
 *		 两个字节表示。 高字节为：具体功能层（0-63：系统层、64-127：硬件驱动层、128-191：服务层、192-255：应用层）
 *						低字节为：具体错误层
 */
 
#ifndef __ERRORCODE_H__
#define __ERRORCODE_H__

typedef enum _DP_ERROR_CODE_E
{
	DP_SUCCESS	= 0x0000,					//成功
	
	
	//系统层
	DP_ERR_SYS_	= 0x0001,
		//File错误
		//Socket错误
		//Net错误
		
		
	//硬件层
	DP_ERR_UDRV_INIT = 0x4001,				//底层硬件服务初始化失败
	DP_ERR_UDRV_PARMS,						//底层硬件服务参数错误
	DP_ERR_UDRV_NOT_INIT,					//底层硬件服务未初始化
	DP_ERR_UDRV_NOT_SUPPORT,				//底层硬件服务不支持此操作

	
	//服务层
	DP_ERR_VASERVER_PARAM_INVALID =	0x8001,	//音视频服务设置参数无效
	DP_ERR_VASERVER_PARAM_ILLEGAL,			//音视频服务设置参数非法
	DP_ERR_VASERVER_INIT,					//音视频服务初始化失败
	DP_ERR_VASERVER_NOT_INIT,				//音视频服务未初始化
	DP_ERR_VASERVER_NOT_SUPPORT,			//音视频服务不支持此操作
	
	DP_ERR_RTSP_OPERATE = 0x8101,			//RTSP操作失败。内部原因的操作失败
	DP_ERR_RTSP_PARAM_INVALID,				//RTSP参数不存在
	DP_ERR_RTSP_PARAM_ILLEGAL,				//RTSP参数非法
	DP_ERR_RTSP_URL_LEN_OVERFLOW,			//RTSP参数URL长度越界
	
	DP_ERR_AI_OPERATE = 0x8201,				//音频输入设备操作失败。内部原因的操作失败
	DP_ERR_AI_ID_INVALID,					//音频输入设备ID不存在
	DP_ERR_AI_PARAM_ILLEGAL,				//音频输入设置参数无效
	DP_ERR_AI_PARAM_ILLEGAL,				//音频输入设置参数非法
	
	DP_ERR_VI_OPERATE = 0x8301,				//视频输入设备操作失败。内部原因的操作失败
	DP_ERR_VI_ID_INVALID,					//视频输入设备ID不存在
	DP_ERR_VI_PARAM_ILLEGAL,				//视频输入设置参数无效
	DP_ERR_VI_PARAM_ILLEGAL,				//视频输入设置参数非法
	
	DP_ERR_AENC_OPERATE = 0x8401,			//音频编码通道操作失败。内部原因的操作失败
	DP_ERR_AENC_ID_INVALID,					//音频编码通道ID不存在
	DP_ERR_AENC_PARAM_INVALID,				//音频编码通道设置参数无效
	DP_ERR_AENC_PARAM_ILLEGAL,				//音频编码通道设置参数非法

	DP_ERR_VENC_OPERATE = 0x8501,			//视频编码通道操作失败。内部原因的操作失败
	DP_ERR_VENC_ID_INVALID,					//视频编码通道ID不存在
	DP_ERR_VENC_PARAM_INVALID,				//视频编码通道设置参数无效
	DP_ERR_VENC_PARAM_ILLEGAL,				//视频编码通道设置参数非法
	
	DP_ERR_ADEC_OPERATE = 0x8601,			//音频编码通道操作失败。内部原因的操作失败
	DP_ERR_ADEC_ID_INVALID,					//音频解码通道ID不存在
	DP_ERR_ADEC_PARAM_INVALID,				//音频解码通道设置参数无效
	DP_ERR_ADEC_PARAM_ILLEGAL,				//音频解码通道设置参数非法

	DP_ERR_VDEC_OPERATE = 0x8701,			//视频编码通道操作失败。内部原因的操作失败
	DP_ERR_VDEC_ID_INVALID,					//视频解码通道ID不存在
	DP_ERR_VDEC_PARAM_INVALID,				//视频解码通道设置参数无效
	DP_ERR_VDEC_PARAM_ILLEGAL,				//视频解码通道设置参数非法
	
	DP_ERR_AO_OPERATE = 0x8801,				//音频输出设备操作失败。内部原因的操作失败
	DP_ERR_AO_ID_INVALID,					//音频输出设备ID不存在
	DP_ERR_AO_PARAM_ILLEGAL,				//音频输出设置参数无效
	DP_ERR_AO_PARAM_ILLEGAL,				//音频输出设置参数非法
	
	DP_ERR_VO_OPERATE = 0x8901,				//视频输出设备操作失败。内部原因的操作失败
	DP_ERR_VO_ID_INVALID,					//视频输出设备ID不存在
	DP_ERR_VO_PARAM_ILLEGAL,				//视频输出设置参数无效
	DP_ERR_VO_PARAM_ILLEGAL,				//视频输出设置参数非法

	
	//应用层
	DP_ERR_PROTOCOL_PRASE =	0xC001,			//协议解析错误。包头、包格式有误。
	DP_ERR_PROTOCOL_PRASE_HEAD,				//协议包头错误
	DP_ERR_PROTOCOL_PRASE_DESTINATION,		//协议目的地址错误
	DP_ERR_PROTOCOL_PRASE_PACKET_TYPE,		//协议包类型错误
	DP_ERR_PROTOCOL_TOTALLEN,				//协议包长错误。需要长度与实际长度不匹配
	DP_ERR_PROTOCOL_CONTENT,				//协议包内容错误。协议内容校验错误。
	DP_ERR_PROTOCOL_VERSION,				//协议版本错误
	DP_ERR_PROTOCOL_PARAM_INVALID,			//协议参数设置无效。协议给出的参数不在范围内。
	DP_ERR_PROTOCOL_PARAM_ILLEGAL,			//协议参数设置非法。协议给出的参数错误。
	DP_ERR_PROTOCOL_PARAM_CMD,				//协议功能码参数不存在
	
	DP_ERR_LOGIN_USERNAME = 0xC101,			//用户名不存在
	DP_ERR_LOGIN_PASSWORD,					//密码错误
	DP_ERR_USER_PERMISSION,					//用户权限不足
	
	DP_ERR_MODE_INVALID = 0xC201,			//模式不存在
	DP_ERR_FUNCTION_INVALID,				//功能不存在
	
	DP_ERR_TIME_OPERATE = 0xC301,			//时间操作失败。内部原因的操作失败
	DP_ERR_TIME_PARAM_ILLEGAL,				//时间值非法
	DP_ERR_TIME_PARAM_OVERFLOW,				//时间值越界。月份大于12、日超出本月天数、时超出24、分超出60、秒超出60
	
	DP_ERR_NET_OPERATE = 0xC401,			//网络操作失败。内部原因的操作失败
	DP_ERR_NET_PARAM_ILLEGAL,				//网络设置参数非法。存在字母、符号、缺段或多段。
	DP_ERR_NET_PARAM_OVERFLOW,				//网络设置参数值越界。参数大于255.
	DP_ERR_NET_MAC_ILLEGAL,					//MAC地址非法。存在异常字母、缺段或多段。
	DP_ERR_NET_IP_UNREASONABLE,				//IP不合理。首段为0、全部都为255。
	DP_ERR_NET_IP_CONFLICT,					//IP冲突。
	DP_ERR_NET_MASK_UNREASONABLE,			//子网掩码不合理。格式错误、与IP地址包含关系错误。
	DP_ERR_NET_GATEWAY_UNREASONABLE,		//网关不合理。网关前三段和IP的前三段不同。
	
	DP_ERR_CONTROL_OPERATE = 0xC501,		//中控操作失败。内部原因的操作失败
	DP_ERR_CONTROL_IO_INVALID,				//IO不存在
	DP_ERR_CONTROL_IO_ILLEGAL,				//IO控制值非法
	DP_ERR_CONTROL_IR_INVALID,				//IR不存在
	DP_ERR_CONTROL_IR_ILLEGAL,				//IR控制值非法
	DP_ERR_CONTROL_UART_INVALID,			//UART不存在
	DP_ERR_CONTROL_UART_ILLEGAL,			//UART初始化参数非法
		
	DP_ERR_TASK_OPERATE = 0xC601,			//任务操作失败。内部原因的操作失败
	DP_ERR_TASK_PARAM_INVALID,				//任务参数不存在
	DP_ERR_TASK_PARAM_ILLEGAL,				//任务参数非法
	DP_ERR_TASK_ID_NOTEXIST,				//任务ID不存在
	DP_ERR_TASK_DEVID_NOTEXIST,				//任务设备ID不存在
	DP_ERR_TASK_SOURCE_NOTENOUGH,			//任务资源不足
		
} DP_ERROR_CODE_E;

#endif
