#ifndef APPLICATION_RECEIVINGTEST_H_
#define APPLICATION_RECEIVINGTEST_H_

#include "TNSNheader.h"
#include "TNSTable.h"
#include <process.h>
#include <deque>

using namespace std;

namespace CNU_DDS
{
	void	StartTopicNamingService(int startOption);
	void	EndTopicNamingService();
	void	InitialDomainForTNS();
}

#endif
