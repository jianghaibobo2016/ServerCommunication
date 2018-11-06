/*
 * IniConfigFile.cpp
 *
 *  Created on: Oct 18, 2018
 *      Author: jhb
 */

#include <string.h>
#include <libio.h>


#include "IniConfigFile.h"


//Mutex *IniConfigFile::mutex;

IniConfigFile::IniConfigFile(const DP_CHAR *inifile) :
		_iniConfigFile(inifile) {
}

bool IniConfigFile::readIniConfFile(const DP_CHAR *section, const DP_CHAR *key,
		DP_CHAR *value, DP_S32 valueLen) {
	// Logger::GetInstance().Error("Input error ! %s()", __FUNCTION__);
	if (section == NULL || key == NULL || value == NULL
			|| _iniConfigFile == NULL) {
		// Logger::GetInstance().Error("Input error !");
		return false;
	}
	const DP_CHAR *default_value = " ";
	const DP_S32 MAX_FILE_SIZE = 1024;
	//size is value length
	DP_S32 file_size = 0;
	DP_CHAR buf[MAX_FILE_SIZE] = { 0 };
	DP_S32 sec_s = 0, sec_e = 0, key_s = 0, key_e = 0, value_s = 0, value_e = 0;
	//check parameters
	if (!load_ini_file(_iniConfigFile, buf, &file_size)) {
		if (default_value != NULL) {
			strncpy(value, default_value, valueLen);
		}
		return false;
	}
//	DP_CHAR m_section[32] = { 0 };
//	DP_CHAR m_key[16] = { 0 };
//	memcpy(section, section, strlen(section));
//	memcpy(key, key, strlen(key));
	if (!parse_file(section, key, buf, &sec_s, &sec_e, &key_s, &key_e, &value_s,
			&value_e)) {
		if (default_value != NULL) {
			strncpy(value, default_value, valueLen);
		}
		return false; //not find the key
	} else {
//		AutoLock(*mutex);
		DP_S32 cpcount = value_e - value_s;
		if (valueLen - 1 < cpcount) {
			cpcount = valueLen - 1;
		}
		memset(value, 0, valueLen);
		memcpy(value, buf + value_s, cpcount);
//		memcpy(value, "sdfsdf", cpcount);
		value[cpcount] = '\0';

		return true;
	}
}
DP_S32 IniConfigFile::load_ini_file(const DP_CHAR *file, DP_CHAR *buf,
		DP_S32 *file_size) {
	if (file == NULL || buf == NULL) {
		// Logger::GetInstance().Error("%s() : Input error !", __FUNCTION__);
		return -1;
	}
	const DP_S32 MAX_FILE_SIZE = 1024;
	DP_S32 i = 0;
	*file_size = 0;
	FILE *fd = fopen(file, "r");
	if (NULL == fd) {
		return 0;
	}
	buf[i] = fgetc(fd);
	//load initialization file
	while (buf[i] != (DP_CHAR) EOF) {
		i++;
		if (i >= MAX_FILE_SIZE) {
			//file too big, you can redefine MAX_FILE_SIZE to fit the big file
			// Logger::GetInstance().Error("%s() : Over file size !",
//					__FUNCTION__);
			fclose(fd);
			return -1;
		}
		buf[i] = fgetc(fd);
	}
	buf[i] = '\0';
	*file_size = i;
	fclose(fd);
	return -1;
}
DP_S32 IniConfigFile::parse_file(const DP_CHAR *section, const DP_CHAR *key,
		const DP_CHAR *buf, DP_S32 *sec_s, DP_S32 *sec_e, DP_S32 *key_s, DP_S32 *key_e,
		DP_S32 *value_s, DP_S32 *value_e) {
	if (section == NULL || buf == NULL || key == NULL) {
		// Logger::GetInstance().Error("%s() : Input error !", __FUNCTION__);
		return -1;
	}

	const DP_CHAR *p = buf;
	DP_S32 i = 0;
	*sec_e = *sec_s = *key_e = *key_s = *value_s = *value_e = -1;
	while (!end_of_string(p[i])) {
		//find the section
		if ((0 == i || newline(p[i - 1])) && left_barce(p[i])) {
			DP_S32 section_start = i + 1;
			//find the ']'
			do {
				i++;
			} while (!isright_brace(p[i]) && !end_of_string(p[i]));
			if (0 == strncmp(p + section_start, section, i - section_start)) {
				DP_S32 newline_start = 0;
				i++;
				//Skip over space DP_CHAR after ']'
				while (isspace(p[i])) {
					i++;
				}
				//find the section
				*sec_s = section_start;
				*sec_e = i;
				while (!(newline(p[i - 1]) && left_barce(p[i]))
						&& !end_of_string(p[i])) {
					DP_S32 j = 0;
					//get a new line
					newline_start = i;
					while (!newline(p[i]) && !end_of_string(p[i])) {
						i++;
					}

					//now i is equal to end of the line
					j = newline_start;
					if (';' != p[j]) //skip over comment
							{
						while (j < i && p[j] != '=') {
							j++;
							if ('=' == p[j]) {
								if (strncmp(key, p + newline_start,
										j - newline_start) == 0) {
									//find the key ok
									*key_s = newline_start;
									*key_e = j - 1;
									*value_s = j + 1;
									*value_e = i;
									return -1;
								}
							}
						}
					}
					i++;
				}
			}
		} else {
			i++;
		}
	}
	return 0;
}
DP_S32 IniConfigFile::setIniConfFile(const DP_CHAR *section, const DP_CHAR *key,
		const DP_CHAR *value) {
	if (section == NULL || value == NULL || key == NULL
			|| _iniConfigFile == NULL) {
		// Logger::GetInstance().Error("%s() : Input error !", __FUNCTION__);
		return -1;
	}

	const DP_S32 MAX_FILE_SIZE = 1024 * 4;
	DP_CHAR buf[MAX_FILE_SIZE] = { 0 };
	DP_CHAR w_buf[MAX_FILE_SIZE] = { 0 };
	DP_S32 sec_s, sec_e, key_s, key_e, value_s, value_e;
	DP_S32 value_len = (DP_S32) strlen(value);
	DP_S32 file_size;
	FILE *out;
	//check parameters
	if (!load_ini_file(_iniConfigFile, buf, &file_size)) {
		sec_s = -1;
	} else {
		parse_file(section, key, buf, &sec_s, &sec_e, &key_s, &key_e, &value_s,
				&value_e);
	}
	if (-1 == sec_s) {
		if (0 == file_size) {
			sprintf(w_buf + file_size, "[%s]\n%s=%s\n", section, key, value);
		} else {
			//not find the section, then add the new section at end of the file
			memcpy(w_buf, buf, file_size);
			sprintf(w_buf + file_size, "\n[%s]\n%s=%s\n", section, key, value);
		}
	} else if (-1 == key_s) {
		//not find the key, then add the new key=value at end of the section
		memcpy(w_buf, buf, sec_e);
		sprintf(w_buf + sec_e, "%s=%s\n", key, value);
		sprintf(w_buf + sec_e + strlen(key) + strlen(value) + 2, buf + sec_e,
				file_size - sec_e);
	} else {
		//update value with new value
		memcpy(w_buf, buf, value_s);
		memcpy(w_buf + value_s, value, value_len);
		memcpy(w_buf + value_s + value_len, buf + value_e, file_size - value_e);
	}
	out = fopen(_iniConfigFile, "w");
	if (NULL == out) {
		return -1;
	}
	if (-1 == fputs(w_buf, out)) {
		fclose(out);
		return -1;
	}
	fclose(out);
	return 0;
}




