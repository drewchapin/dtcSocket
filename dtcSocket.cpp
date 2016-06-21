#include "dtcSocket.h"

dtcSocket::dtcSocket() : m_socket( -1 )
{
	memset( &m_addr, 0, sizeof(m_addr) );
}

dtcSocket::~dtcSocket()
{
	Close();
}

void dtcSocket::Close()
{
	if( IsValid() )
		::close( m_socket );
	
}

bool dtcSocket::Create()
{

	m_socket = ::socket( AF_INET, SOCK_STREAM, 0 );
	
	if( !IsValid() )
		return false;
	
	int on = 1;
	if( -1 == setsockopt(m_socket,SOL_SOCKET,SO_REUSEADDR,(const char*) &on,sizeof(on)) )
		return false;

	return true;

}

bool dtcSocket::Bind( const int port )
{

	if( !IsValid() )
		return false;

	m_addr.sin_family = AF_INET;
	m_addr.sin_addr.s_addr = INADDR_ANY;
	m_addr.sin_port = htons( port );

	if( -1 == ::bind(m_socket,(struct sockaddr*)&m_addr,sizeof(m_addr)) )
		return false;

	return true;

}

bool dtcSocket::Listen( const int max ) 
{
	if( !IsValid() )
		return false;

	if( -1 == ::listen( m_socket, max ) )
		return false;

	return true;

}

bool dtcSocket::Accept( dtcSocket &new_socket )
{
	
	int addr_len = sizeof( m_addr );

	if( 0 >= (new_socket.m_socket = ::accept(m_socket,(sockaddr*)&m_addr,(socklen_t*)&addr_len)) )
		return false;

	return true;

}


bool dtcSocket::Send( const std::string s )
{
	if( -1 == ::send(m_socket, s.c_str(), s.size(), MSG_NOSIGNAL ) )
		return false;

	return true;

}

int dtcSocket::Receive( std::string &s, const int max, const long timeout )
{

	if( max <= 0 )
		return 0;

	char buf[max+1];
	int status = 0;

	s = "";
	memset( buf, 0, max+1 );

	if( timeout >= 0 )
	{
		struct timeval tv;
		tv.tv_sec = 0;
		tv.tv_usec = timeout * 1000;
		fd_set readfds;

		FD_ZERO( &readfds );
		FD_SET( m_socket, &readfds );

		int selerr = select(m_socket+1, &readfds, 0, 0, &tv );
	
		if( FD_ISSET(m_socket, &readfds) )
			status = ::recv( m_socket, buf, max, 0 );
		else
			return -1;
	}
	else
	{
		status = ::recv( m_socket, buf, max, 0 );
	}

	if( status > 0 )
		s = buf;

	return status;

}


bool dtcSocket::Connect( const std::string host, const int port )
{

	if( !IsValid() )
		return false;

	m_addr.sin_family = AF_INET;
	m_addr.sin_port = htons( port );

	struct hostent *host_info = gethostbyname(host.c_str());
	if( NULL != host_info )
	{
		memcpy( &m_addr.sin_addr, host_info->h_addr_list[0], host_info->h_length );
	}
	else
	{
		inet_pton( AF_INET, host.c_str(), &m_addr.sin_addr );
	}

	if( EAFNOSUPPORT == errno )
		return false;

	return !(::connect(m_socket,(sockaddr*)&m_addr,sizeof(m_addr)));

}

void dtcSocket::SetBlocking( const bool block )
{

	int opts;

	if( 0 > (opts=fcntl(m_socket,F_GETFL)) )
		return;

	if( !block )
		opts |= O_NONBLOCK;
	else
		opts &= ~O_NONBLOCK;

	fcntl( m_socket, F_SETFL, opts );

}
