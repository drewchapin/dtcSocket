dtcSocket:
	$(CXX) $(CFLAGS) -c dtcSocket.cpp

server_test: dtcSocket
	$(CXX) $(CFLAGS) -c server_test.cpp
	$(CXX) $(CFLAGS) -o server_test server_test.o dtcSocket.o -lpthread

client_test: dtcSocket
	$(CXX) $(CFLAGS) -c client_test.cpp
	$(CXX) $(CFLAGS) -o client_test client_test.o dtcSocket.o

all: client_test server_test

clean:
	$(RM) client_test server_test *.o

