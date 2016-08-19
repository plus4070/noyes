#ifndef APPLICATION_RECEIVINGTEST_H_
#define APPLICATION_RECEIVINGTEST_H_

#include <vector>

using namespace std;

namespace CNU_DDS
{
	void	ErrorHandling(char *message);
	SOCKET	CreateSocket();
	void	BindingSocket(SOCKET servSocket) ;
	void	LinkingEvents(SOCKET servSock, int* sockNum, vector<SOCKET> * sockArray, vector<WSAEVENT> * eventArray);
	void	StartReceving();
}
#endif
