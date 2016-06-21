#include "dtcSocket.h"
#include <iostream>
#include <string>
#include <errno.h>
#include <pthread.h>
#include <stdlib.h>

using namespace std;

void *serv( void* );

int main( int argc, char *argv[] )
{

	dtcSocket drew;

	cout << "Creating socket" << endl;
	if( !drew.Create() )
		cerr << "Failed to create socket..." << endl;

	cout << "Binding to port 1337...\n" << endl;
	if( !drew.Bind( 1337 ) )
		cerr << strerror(errno) << endl;

	cout << "Listening..." << endl;
	if( !drew.Listen() )
			cerr << "Failed to begin listening..." << endl;

	while( true )
	{

		dtcSocket *test = new dtcSocket(); //this MUST be dynamically allocated or it does not work... why? I don't know... seems to reuse the socket from the previous iteration... I think my understanding of declaring classes in a loop is flawed... 

		drew.Accept( *test );

		pthread_t thread_id;
		switch( pthread_create( &thread_id, NULL, &serv, test ) )
		{
			case 0: break;
			case EAGAIN: cerr << "System did not have the necessary resources to create another thread." << endl; break;
			case EINVAL: cerr << "The value specified by attr is invalid." << endl; break;
			case EPERM: cerr << "The caller does not have appropriate permission to set the required scheduling parameters or scheduling policy." << endl; break;
			default: cerr << "An unknown error occured while creating the thread." << endl; break;
			
		}
	}

	drew.Close();


	return 0;

}

void *serv( void *ptr )
{

	dtcSocket *sock = (dtcSocket*)ptr;

	string data;
	string buffer;

	while( 0 < sock->Receive(buffer,24,60*1000) )
	{

		for( string::iterator p = buffer.begin(); p < buffer.end(); p++ )
		{

			if( *p == '\n' )
			{
				if( !data.compare("exit") )
				{
					sock->Close();
					return NULL;
				}
				else
					cout << "Command (" << data.length() << "):  \"" << data << "\"" << endl;
				data = "";
			}
			else if( *p != '\r' )
				data = data + *p;
		}

	}

	sock->Close();

	return NULL;
}
