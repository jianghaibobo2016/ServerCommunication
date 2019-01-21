/*
 * DataPackageParser.h
 *
 *  Created on: Oct 17, 2018
 *      Author: jhb
 */

#ifndef SRC_DATAPACKAGEPARSER_H_
#define SRC_DATAPACKAGEPARSER_H_

//#include <string.h>
#include "dp_sd_type.h"
//#include "interactivepro.h"

#include <muduo/base/Logging.h>
#pragma pack(push)
#pragma pack(1)
typedef struct _S_DataHeadInfo_ {
	DP_U32 u32SrcIP;     ///<源IP地址，小端模式数据
	DP_U8 u8DeviceType; ///<源设备类型
	DP_U32 u32RequestID; ///<命令请求ID，自增字段
	DP_U8 u8PackageType; ///<0:request data 1:reply data
	DP_U8 u8CommandID;
	DP_U32 u32PackLen;
} _S_DataHeadInfo;
#pragma pack(pop)

typedef enum _E_RemoteCMD_ {
	_eSearch = 0x01,
	_eGetInfo,
	_eSetInfo,
	_eCreateWindow,
	_eMoveWindow,
	_eCloseWindow,
	_eOpenAudio,
	_eCloseAudio,
	_eSetAudio,
	_eClearTask = 0x0b,
	_eGetJsonInfo = 0x20,
	_eSetJsonInfo = 0x21,
	_eUnicastSearch = 0xf1,
	_eOpenAndMoveWindow = 0xf2,
	_eWindowBatchControl = 0xf3,
	_eUpdateBatch = 0xfa,
	_eButt
} _E_RemoteCMD;

class DataPackageParser {
public:
	DataPackageParser();
	~DataPackageParser();

	static _E_RemoteCMD parserDataPackage(DP_U32 &count, std::string data);

private:

	static bool parserDataHead(_S_DataHeadInfo *dataHeadInfo, std::string data);
};

#endif /* SRC_DATAPACKAGEPARSER_H_ */
