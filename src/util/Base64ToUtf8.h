/*
 * Base64ToUtf8.h
 *
 *  Created on: 2018年1月14日
 *      Author: root
 */

#ifndef UTIL_BASE64TOUTF8_H_
#define UTIL_BASE64TOUTF8_H_

#include <stdint.h>
#include <ostream>
using namespace std;
class Base64ToUtf8 {

public:
// Write buffer to output stream in Base64 encoding
static int GetBase64LengthFromBinaryLength(int bytes);

// Returns number of bytes written, or 0 for error
static int WriteBase64(const void *buffer, int bytes, char *encoded_buffer, int encoded_bytes);
static int WriteBase64Str(const void *buffer, int bytes, char *encoded_buffer, int encoded_bytes); // This version writes a null-terminator
static int WriteBase64(const void *buffer, int bytes, std::ostream &output);
static void WriteBase64(const void *buffer, int bytes, string &output);

// Read buffer in Base64 encoding
static int GetBinaryLengthFromBase64Length(const char *encoded_buffer, int bytes);

// Returns number of bytes read, or 0 for error
static int ReadBase64(const char *encoded_buffer, int encoded_bytes, void *decoded_buffer, int decoded_bytes);
static int ReadBase64(const char *encoded_buffer, int encoded_bytes, std::ostream &output);
static void ReadBase64(const char *encoded_buffer, int encoded_bytes,string &decout);


};

#endif /* UTIL_BASE64TOUTF8_H_ */
