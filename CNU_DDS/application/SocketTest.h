#ifndef APPLICATION_SOCKETTEST_H_
#define APPLICATION_SOCKETTEST_H_

#include "../include/Utility/PlatformOrientedInterface.h"
#include "../include/Utility/Network/UDPSocket.h"

namespace CNU_DDS
{
	void	SocketTest();
	void	_SocketAddressTest();
	void*	_SocketTestReceiver(void* arg);
}

#endif /* APPLICATION_SOCKETTEST_H_ */
