/*
 * RWFile.h
 *
 *  Created on: 2017年9月9日
 *      Author: root
 */

#ifndef UTIL_RWFILE_H_
#define UTIL_RWFILE_H_

#include "Shared.h"
#include "Handle.h"
#include <stdio.h>


class RWFile :public Shared{
public:
	RWFile(const string path){
		_pFile = fopen(path.c_str(),"w+");
		if(_pFile == NULL)
			throw SystemException();
	}
	virtual ~RWFile(){
		fclose(_pFile);
		_pFile = NULL;
	}

	int read(void* pData,int len)throw(SystemException){
		return 0;
	}
	int write(const void * pData,int len)throw(SystemException){
		if(_pFile == NULL)
				throw SystemException("file is not open");
		int ret = fseek(_pFile,0,SEEK_END);
		if(ret == -1)
			throw SystemException();
		int wbytes = fwrite(pData,len,1,_pFile);
		if(wbytes == 0)
			throw SystemException("write fail");
		else
			return wbytes;
	}

private:
	FILE* _pFile;
	string _path;

};
class FileLog:public RWFile,public Logger{
	FileLog();
	~FileLog();
};
typedef Handle<RWFile> RWFilePtr;
#endif /* UTIL_RWFILE_H_ */
