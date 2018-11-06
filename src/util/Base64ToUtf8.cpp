/*
 * Base64ToUtf8.cpp
 *
 *  Created on: 2018年1月16日
 *      Author: root
 */

#include "Base64ToUtf8.h"

static const char TO_BASE64[64] = {
	'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
	'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
	'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
	'w', 'x', 'y', 'z', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', '/'
};


int Base64ToUtf8::GetBase64LengthFromBinaryLength(int bytes)
{
	if (bytes <= 0) return 0;

	return ((bytes + 2) / 3) * 4;
}

int Base64ToUtf8::WriteBase64(const void *buffer, int bytes, char *encoded_buffer, int encoded_bytes)
{
	int written_bytes = ((bytes + 2) / 3) * 4;

	if (bytes <= 0 || encoded_bytes < written_bytes)
		return 0;

	const uint8_t *data = reinterpret_cast<const uint8_t*>( buffer );

	int ii, jj, end;
	for (ii = 0, jj = 0, end = bytes - 2; ii < end; ii += 3, jj += 4)
	{
		encoded_buffer[jj] = TO_BASE64[data[ii] >> 2];
		encoded_buffer[jj+1] = TO_BASE64[((data[ii] << 4) | (data[ii+1] >> 4)) & 0x3f];
		encoded_buffer[jj+2] = TO_BASE64[((data[ii+1] << 2) | (data[ii+2] >> 6)) & 0x3f];
		encoded_buffer[jj+3] = TO_BASE64[data[ii+2] & 0x3f];
	}

	switch (ii - end)
	{
	default:
	case 2: // Nothing to write
		break;

	case 1: // Need to write final 1 byte
		encoded_buffer[jj] = TO_BASE64[data[bytes-1] >> 2];
		encoded_buffer[jj+1] = TO_BASE64[(data[bytes-1] << 4) & 0x3f];
		encoded_buffer[jj+2] = '=';
		encoded_buffer[jj+3] = '=';
		break;

	case 0: // Need to write final 2 bytes
		encoded_buffer[jj] = TO_BASE64[data[bytes-2] >> 2];
		encoded_buffer[jj+1] = TO_BASE64[((data[bytes-2] << 4) | (data[bytes-1] >> 4)) & 0x3f];
		encoded_buffer[jj+2] = TO_BASE64[(data[bytes-1] << 2) & 0x3f];
		encoded_buffer[jj+3] = '=';
		break;
	}

	return written_bytes;
}

// This version writes a C string null-terminator
int Base64ToUtf8::WriteBase64Str(const void *buffer, int bytes, char *encoded_buffer, int encoded_bytes)
{
	int written = WriteBase64(buffer, bytes, encoded_buffer, encoded_bytes - 1);

	encoded_buffer[written] = '\0';

	return written;
}

int Base64ToUtf8::WriteBase64(const void *buffer, int bytes, ostream &output)
{
	if (bytes <= 0) return 0;

	const uint8_t *data = reinterpret_cast<const uint8_t*>( buffer );

	int ii, end;
	for (ii = 0, end = bytes - 2; ii < end; ii += 3)
	{
		output << TO_BASE64[data[ii] >> 2];
		output << TO_BASE64[((data[ii] << 4) | (data[ii+1] >> 4)) & 0x3f];
		output << TO_BASE64[((data[ii+1] << 2) | (data[ii+2] >> 6)) & 0x3f];
		output << TO_BASE64[data[ii+2] & 0x3f];
	}

	switch (ii - end)
	{
	default:
	case 2: // Nothing to write
		break;

	case 1: // Need to write final 1 byte
		output << TO_BASE64[data[bytes-1] >> 2];
		output << TO_BASE64[(data[bytes-1] << 4) & 0x3f];
		output << "==";
		break;

	case 0: // Need to write final 2 bytes
		output << TO_BASE64[data[bytes-2] >> 2];
		output << TO_BASE64[((data[bytes-2] << 4) | (data[bytes-1] >> 4)) & 0x3f];
		output << TO_BASE64[(data[bytes-1] << 2) & 0x3f];
		output << '=';
		break;
	}

	return ((bytes + 2) / 3) * 4;
}

void Base64ToUtf8::WriteBase64(const void *buffer, int bytes, string &output)
{
	int len = GetBase64LengthFromBinaryLength(bytes);
	output.resize(len);
	WriteBase64(buffer,bytes,(char*)output.c_str(),len);
}

//// Conversion from Base64

static const uint8_t DC = 0;

static const uint8_t FROM_BASE64[256] = {
	DC, DC, DC, DC, DC, DC, DC, DC, DC, DC, DC, DC, DC, DC, DC, DC, // 0-15
	DC, DC, DC, DC, DC, DC, DC, DC, DC, DC, DC, DC, DC, DC, DC, DC, // 16-31
	DC, DC, DC, DC, DC, DC, DC, DC, DC, DC, DC, 62, DC, DC, DC, 63, // 32-47
	52, 53, 54, 55, 56, 57, 58, 59, 60, 61, DC, DC, DC, DC, DC, DC, // 48-63
	DC, 0 , 1 , 2 , 3 , 4 , 5 , 6 , 7 , 8 , 9 , 10, 11, 12, 13, 14, // 64-79
	15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, DC, DC, DC, DC, DC, // 80-95
	DC, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, // 96-111
	41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, DC, DC, DC, DC, DC, // 112-127
	DC, DC, DC, DC, DC, DC, DC, DC, DC, DC, DC, DC, DC, DC, DC, DC, // 128-
	DC, DC, DC, DC, DC, DC, DC, DC, DC, DC, DC, DC, DC, DC, DC, DC, // Extended
	DC, DC, DC, DC, DC, DC, DC, DC, DC, DC, DC, DC, DC, DC, DC, DC, // ASCII
	DC, DC, DC, DC, DC, DC, DC, DC, DC, DC, DC, DC, DC, DC, DC, DC, // Extended
	DC, DC, DC, DC, DC, DC, DC, DC, DC, DC, DC, DC, DC, DC, DC, DC, // ASCII
	DC, DC, DC, DC, DC, DC, DC, DC, DC, DC, DC, DC, DC, DC, DC, DC, // Extended
	DC, DC, DC, DC, DC, DC, DC, DC, DC, DC, DC, DC, DC, DC, DC, DC, // ASCII
	DC, DC, DC, DC, DC, DC, DC, DC, DC, DC, DC, DC, DC, DC, DC, DC
};


int Base64ToUtf8::GetBinaryLengthFromBase64Length(const char *encoded_buffer, int bytes)
{
	if (bytes <= 0) return 0;

	while (bytes >= 1 && encoded_buffer[bytes-1] == '=')
		--bytes;

	return (bytes * 3) / 4;
}

int Base64ToUtf8::ReadBase64(const char *encoded_buffer, int encoded_bytes, void *decoded_buffer, int decoded_bytes)
{
	// Skip '=' characters at the end
	while (encoded_bytes >= 1 && encoded_buffer[encoded_bytes-1] == '=')
		--encoded_bytes;

	if (encoded_bytes <= 0 || decoded_bytes <= 0 ||
		decoded_bytes < (encoded_bytes * 3) / 4)
	{
		return 0;
	}

	const uint8_t *from = reinterpret_cast<const uint8_t*>( encoded_buffer );
	uint8_t *to = reinterpret_cast<uint8_t*>( decoded_buffer );

	uint8_t a, b, c, d;

	int ii, jj, end;
	for (ii = 0, jj = 0, end = encoded_bytes - 3; ii < end; ii += 4, jj += 3)
	{
		a = FROM_BASE64[from[ii]];
		b = FROM_BASE64[from[ii+1]];
		c = FROM_BASE64[from[ii+2]];
		d = FROM_BASE64[from[ii+3]];

		to[jj] = (a << 2) | (b >> 4);
		to[jj+1] = (b << 4) | (c >> 2);
		to[jj+2] = (c << 6) | d;
	}

	switch (encoded_bytes & 3)
	{
	case 3: // 3 characters left
		a = FROM_BASE64[from[ii]];
		b = FROM_BASE64[from[ii+1]];
		c = FROM_BASE64[from[ii+2]];

		to[jj] = (a << 2) | (b >> 4);
		to[jj+1] = (b << 4) | (c >> 2);
		return jj+2;

	case 2: // 2 characters left
		a = FROM_BASE64[from[ii]];
		b = FROM_BASE64[from[ii+1]];

		to[jj] = (a << 2) | (b >> 4);
		return jj+1;
	}

	return jj;
}

int Base64ToUtf8::ReadBase64(const char *encoded_buffer, int encoded_bytes, std::ostream &output)
{
	// Skip '=' characters at the end
	while (encoded_bytes >= 1 && encoded_buffer[encoded_bytes-1] == '=')
		--encoded_bytes;

	if (encoded_bytes <= 0) return 0;

	const uint8_t *from = reinterpret_cast<const uint8_t*>( encoded_buffer );

	uint8_t a, b, c, d;

	int ii, end;
	for (ii = 0, end = encoded_bytes - 3; ii < end; ii += 4)
	{
		a = FROM_BASE64[from[ii]];
		b = FROM_BASE64[from[ii+1]];
		c = FROM_BASE64[from[ii+2]];
		d = FROM_BASE64[from[ii+3]];

		output << (uint8_t)((a << 2) | (b >> 4));
		output << (uint8_t)((b << 4) | (c >> 2));
		output << (uint8_t)((c << 6) | d);
	}

	switch (encoded_bytes & 3)
	{
	case 3: // 3 characters left
		a = FROM_BASE64[from[ii]];
		b = FROM_BASE64[from[ii+1]];
		c = FROM_BASE64[from[ii+2]];

		output << (uint8_t)((a << 2) | (b >> 4));
		output << (uint8_t)((b << 4) | (c >> 2));
		break;

	case 2: // 2 characters left
		a = FROM_BASE64[from[ii]];
		b = FROM_BASE64[from[ii+1]];

		output << (uint8_t)((a << 2) | (b >> 4));
		break;
	}

	return (encoded_bytes * 3) / 4;
}

void Base64ToUtf8::ReadBase64(const char *encoded_buffer, int encoded_bytes,string &decout){
	int len = GetBinaryLengthFromBase64Length(encoded_buffer,encoded_bytes);
	decout.resize(len);
	ReadBase64(encoded_buffer,encoded_bytes,(void*)decout.c_str(),len);
}
