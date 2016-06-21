#include "dtcSocket.h"
#include <string>
#include <iostream>

using namespace std;

int main( int argc, char *argv[] )
{

	try
	{
	
		dtcSocket drew;


		cout << "Creating..." << endl;
		if( !drew.Create() )
		{
			cerr << "Could not create socket" << endl;
			return 1;
		}

		cout << "Connecting..." << endl;
		if( !drew.Connect( "google.com", 80 ) )
		{
			cerr << "Failed to connect" << endl;
			return 1;
		}

		cout << "Sending data..." << endl;
		if( !drew.Send( "GET / HTTP/1.1\n" ) )
		{
			cerr << "Failed to send data" << endl;
			return 1;
		}

		cout << "Sending data..." << endl;
		if( !drew.Send( "HOST www.google.com\n\n" ) )
		{
			cerr << "Failed to send data" << endl;
			return 1;
		}
		else
		{
			string ret;

			cout << "Waiting for reply..." << endl;

			while( 0 < drew.Receive( ret ) )
				cout << ret << endl << endl;
		}

		cout << "Closing connectiong..." << endl; 
		drew.Close();

	}
	catch( dtcSocketException &e )
	{
		cerr << "Error: " << e.Description() << endl;
	}

	return 0;	

}
