/*
 * IniConfigFile.h
 *
 *  Created on: Oct 18, 2018
 *      Author: jhb
 */

#ifndef SRC_INICONFIGFILE_H_
#define SRC_INICONFIGFILE_H_

#include "dp_sd_type.h"
#include "SetNetwork.h"
#include <stdio.h>
//#include "Mutex.h"
class IniConfigFile {
//	static Mutex *mutex;
//	friend class SetNetworkTerminal;
public:
	IniConfigFile(const DP_CHAR *inifile);
	~IniConfigFile() {
	}
	bool readIniConfFile(const DP_CHAR *section, const DP_CHAR *key, DP_CHAR *value,
			DP_S32 valueLen);
	DP_S32 setIniConfFile(const DP_CHAR *section, const DP_CHAR *key,
			const DP_CHAR *value);
private:
	const DP_CHAR *_iniConfigFile;
	DP_S32 load_ini_file(const DP_CHAR *file, DP_CHAR *buf, DP_S32 *file_size);
	DP_S32 parse_file(const DP_CHAR *section, const DP_CHAR *key, const DP_CHAR *buf,
			DP_S32 *sec_s, DP_S32 *sec_e, DP_S32 *key_s, DP_S32 *key_e,
			DP_S32 *value_s, DP_S32 *value_e);

	DP_S32 inline newline(DP_CHAR c) {
		return ('\n' == c || '\r' == c) ? 1 : 0;
	}
	DP_S32 inline end_of_string(DP_CHAR c) {
		return '\0' == c ? 1 : 0;
	}
	DP_S32 inline left_barce(DP_CHAR c) {
		return LEFT_BRACE == c ? 1 : 0;
	}
	DP_S32 inline isright_brace(DP_CHAR c) {
		return RIGHT_BRACE == c ? 1 : 0;
	}
};




#endif /* SRC_INICONFIGFILE_H_ */
