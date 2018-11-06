#ifndef NETTRANS_H
#define NETTRANS_H
#include <sys/socket.h>
#include <string>
#include <exception>
#include "DevStaticConfigure.h"
#include "dp_sd_type.h"


//! Signals a problem with the execution of a socket call
class SocketException: public std::exception {
public:
	/*!
	 * Construct a SocketException with a explanatory message.
	 * \param message explanatory message
	 * \param inclSysMsg true if system message from strerror(errno)
	 * should be postfixed to the user provided message
	 */
	SocketException(const std::string &message, bool inclSysMsg = true) throw ();

	//! Provided just to guarantee that no exceptions are thrown.
	~SocketException() throw () {
	}
	;

	//! Returns a C-string describing the cause of the current error.
	const char *what() const throw () {
		return m_message.c_str();
	}

	//! Returns the glibc errno code of the current error.
	int errorCode() const throw () {
		return m_errorcode;
	}

private:
	std::string m_message;
	int m_errorcode;
};

class NetTrans /*private Uncopyable,*/{
public:

	~NetTrans();
	DP_S32 getSockfd() {
		return m_socket;
	}
	DP_S32 socketBind(unsigned short localPort);

	void static printBufferByHex(const void *note, void *buff, DP_U32 len);

//	virtual void OnTimer(void) {
//	}

protected:
	enum SocketDomain {
		INTERNET = PF_INET, UNIX = PF_LOCAL, UNIXAF = AF_INET
	};
	enum SocketType {
		STREAM = SOCK_STREAM, DATAGRAM = SOCK_DGRAM // | SOCK_NONBLOCK //
	};

	NetTrans(int domain, int type, int protocol);
	NetTrans(const NetTrans &netTrans);
//	NetTrans &operator=(const NetTrans &netT);

	// socket descriptor
	DP_S32 m_socket;
	DP_S32 port;

protected:
	NetTrans();
//	}
};

#endif /* NETTRANS_H */
