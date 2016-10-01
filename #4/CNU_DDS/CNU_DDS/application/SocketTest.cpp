#include "SocketTest.h"
#include <iostream>

namespace CNU_DDS
{
	void	SocketTest()
	{
		/*
		UDPSocket*			socket_r;
		UDPSocket*			socket_w;
		THREAD_TYPE_NATIVE	thread;
		char				sending_data[19]	= "SocketTest succeed";

		socket_r	= new UDPSocket("168.188.128.147", 4455);
		socket_r->binding();

		socket_w	= new UDPSocket("168.188.128.147", 5544);
		socket_w->setExternal("168.188.128.255", 4455);

		create_thread(&thread, _SocketTestReceiver, socket_r);

		native_sleep(3000);

		std::cout << "start socket_w" << std::endl;
		socket_w->send(sending_data, 19);

		native_sleep(3000);
		std::cout << "socket test is over" << std::endl;
		*/
		_SocketAddressTest();
	}

	void	_SocketAddressTest()
	{
		UDPSocket*		socket	= new UDPSocket();
		unsigned long	addr;
		unsigned char*	ptr;
		unsigned long	addr2	= inet_addr("168.188.128.147");

		socket->setInternalAddressByPureStr("168.188.128.147");
		socket->setInternalPort(188);

		addr	= socket->getInternalAddressToLong();
		ptr		= (unsigned char*)&addr;

		printf("%08X\n", addr);
		printf("%d.%d.%d.%d\n", ptr[0], ptr[1], ptr[2], ptr[3]);

		ptr		= (unsigned char*)&addr2;

		printf("%08X\n", addr2);
		printf("%d.%d.%d.%d\n", ptr[0], ptr[1], ptr[2], ptr[3]);
	}

	void*	_SocketTestReceiver(void* arg)
	{
		UDPSocket*	socket		= (UDPSocket*)arg;
		ByteStream*	recv_data	= 0;

		std::cout << "start socket_r" << std::endl;

		recv_data	= socket->receive();

		if(recv_data == 0)
		{
			std::cout << "\trecv_data.size == 0" << std::endl;
		}
		else
		{
			std::cout << "\treceive : " << recv_data->getData() << std::endl;
		}

		return 0;
	}
}
