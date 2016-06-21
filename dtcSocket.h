#ifndef DTCSOCKET_H
#define DTCSOCKET_H

#include <string>
#include <sys/socket.h>	//included by arpa/inet.h but, just to be sure...
#include <fcntl.h>		//required by FD_* functions for timeout
#include <arpa/inet.h>	//required by inet_pton()
#include <cstring>		//required by memset() for some odd reason...
#include <errno.h>		//required by errno
#include <netdb.h>		//required for gethostbyname()


#define dtcMAXCONNECTIONS		200
#define dtcMAXRECEIVE			512
#define dtcTIMEOUT				10000 
#define dtcNOTIMEOUT			-1

class dtcSocketException
{

	private: 

		std::string desc;

	public:

		dtcSocketException( std::string s ) : desc( s ) {};
		~dtcSocketException() {};

		std::string Description() { return desc; }

};

class dtcSocket
{

	private:
		sockaddr_in m_addr;
		int m_socket;

	public:
		dtcSocket();
		~dtcSocket();

		bool Create();
		bool Bind( const int );
		bool Listen( const int max = dtcMAXCONNECTIONS );
		bool Accept( dtcSocket& );

		bool Connect( const std::string, const int );
		void Close();
		bool Send( const std::string );
		int Receive( std::string &s, const int max = dtcMAXRECEIVE, const long timeout = dtcTIMEOUT);
	
		void SetBlocking( const bool );
		bool IsValid() { return -1 != m_socket; }

};
	

#endif
		
