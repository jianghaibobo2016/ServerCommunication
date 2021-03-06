/*
 * md5.h
 *
 *  Created on: 2018年3月13日
 *      Author: dsppa
 */

#ifndef MD5_MD5_H_
#define MD5_MD5_H_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

namespace DSPPAUtil {

#define READ_DATA_SIZE  1024
#define MD5_SIZE        16
#define MD5_STR_LEN     (MD5_SIZE * 2)

#define F(x,y,z) ((x & y) | (~x & z))
#define G(x,y,z) ((x & z) | (y & ~z))
#define H(x,y,z) (x^y^z)
#define I(x,y,z) (y ^ (x | ~z))
#define ROTATE_LEFT(x,n) ((x << n) | (x >> (32-n)))
#define FF(a,b,c,d,x,s,ac) {a += F(b,c,d) + x + ac; a = ROTATE_LEFT(a,s); a += b;}
#define GG(a,b,c,d,x,s,ac) {a += G(b,c,d) + x + ac; a = ROTATE_LEFT(a,s); a += b;}
#define HH(a,b,c,d,x,s,ac) {a += H(b,c,d) + x + ac; a = ROTATE_LEFT(a,s); a += b;}
#define II(a,b,c,d,x,s,ac) {a += I(b,c,d) + x + ac; a = ROTATE_LEFT(a,s); a += b;}

class MD5{

public:
	MD5();
	virtual ~MD5();

	void MD5Init();
	void MD5Update(unsigned char *input, unsigned int inputlen);
	void MD5Final(unsigned char digest[16]);//digest[16] is the MD5 value
//	bool GetFileMD5(const char *file_path,unsigned char *md5_str);

private:
	void MD5Transform(unsigned int state[4], unsigned char block[64]);
	void MD5Encode(unsigned char *output, unsigned int *input, unsigned int len);
	void MD5Decode(unsigned int *output, unsigned char *input, unsigned int len);

private:
	//MD5_CTX
	unsigned int count[2];
	unsigned int state[4];
	unsigned char buffer[64];

};

} /* namespace DSPPAUtil */

#endif /* MD5_MD5_H_ */
