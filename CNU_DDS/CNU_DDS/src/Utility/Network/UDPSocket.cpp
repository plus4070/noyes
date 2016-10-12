#include "../../../include/Utility/Network/UDPSocket.h"
#include "../../../include/Utility/SystemInformation.h"
#include <iostream>

namespace CNU_DDS
{
	const char UDPSocket::yes	= 0x01;

	UDPSocket::UDPSocket()
	{
		initUDPSocket();
	}

	UDPSocket::UDPSocket(char* address, unsigned short port)
	{
		initUDPSocket();
		//setInternalAddress(address);
		//setInternalAddressByPureStr(address);
		setInternalAddressByDDSLocator(address);
		setInternalPort(port);
	}

	void UDPSocket::initUDPSocket()
	{
		bufferSize = MAX_BUFFER_SIZE;
#ifdef _WIN32
		if(WSAStartup(MAKEWORD(2,2), &wsaData) == SOCKET_ERROR_VALUE_NATIVE)
		{
			printf("ERROR : WSAStartup\n");
			WSACleanup();
			exit(0);
		}
#else
#endif

		Socket = socket(AF_INET, SOCK_DGRAM, SOCKET_OPTION_IPPROTO_UDP);
/*
		if(Socket == INVALID_SOCKET)
		{
			printf("ERROR : INVALID_SOCKET\n");
			closesocket(Socket);
			WSACleanup();
			exit(0);
		}
*/
		internalAddress.sin_family = AF_INET;
	}

	UDPSocket::~UDPSocket()
	{
#ifdef _WIN32
		closesocket(Socket);
		WSACleanup();
#else
		close(Socket);
#endif
	}

	void	UDPSocket::setInternalAddressByPureStr(char* address)
	{
		char arr[16] = {0, };
		memcpy(arr, address, 15);
		internalAddress.sin_addr.s_addr = inet_addr(arr);
	}

	void	UDPSocket::setInternalAddressByDDSLocator(char* address)
	{
		internalAddress.sin_addr.s_addr = *(unsigned long*)(address + 12);
	}

	void	UDPSocket::setInternalAddress(long address)
	{
		internalAddress.sin_addr.s_addr = address;
	}
	/*
	void UDPSocket::setInternalAddress(char* address)
	{
		char arr[16] = {0, };
		memcpy(arr, address, 15);
		internalAddress.sin_addr.s_addr = inet_addr(arr);
	}
	*/
	void UDPSocket::setInternalPort(unsigned short port)
	{
		internalPort = port;
		internalAddress.sin_port = htons(port);
	}

	void UDPSocket::binding()
	{
		setReusable();
		if(bind(Socket, (struct sockaddr*)&internalAddress, sizeof(internalAddress)) == SOCKET_ERROR_VALUE_NATIVE)
		{
			//printf("ERROR : bind\n");
			std::cout<< "ERROR : binding()" << std::endl;
#ifdef _WIN32
			closesocket(Socket);
			WSACleanup();
#else
			close(Socket);
#endif
			exit(0);
		}
		setMulticastGroup();
	}

	void		UDPSocket::setExternalByPureStr(char* address, unsigned short port)
	{
		addressSize = sizeof(externalAddress);
		externalPort = port;
		externalAddress.sin_family = AF_INET;

		if(address == NULL)	//External을 딱히 지정하지 않음
		{
			externalAddress.sin_addr.s_addr = INADDR_ANY;
		}
		else
		{
			externalAddress.sin_addr.s_addr = inet_addr(address);
		}
		externalAddress.sin_port = htons(port);

		memset(buffer, 0, sizeof(buffer));
	}

	void		UDPSocket::setExternalByDDSLocator(char* address, unsigned short port)
	{
		
		addressSize = sizeof(externalAddress);
		externalPort = port;
		externalAddress.sin_family = AF_INET;

		if(address == NULL)	//External을 딱히 지정하지 않음
		{
			externalAddress.sin_addr.s_addr = INADDR_ANY;
		}
		else
		{
			externalAddress.sin_addr.s_addr = *(unsigned long*)(address+12);
		}
		externalAddress.sin_port = htons(port);

		memset(buffer, 0, sizeof(buffer));
	}

	void	UDPSocket::setExternal(long address, unsigned short port)
	{
		addressSize = sizeof(externalAddress);
		externalPort = port;
		externalAddress.sin_family = AF_INET;

		if(address == -1)	//External을 딱히 지정하지 않음
		{
			externalAddress.sin_addr.s_addr = INADDR_ANY;
		}
		else
		{
			externalAddress.sin_addr.s_addr = address;
		}
		externalAddress.sin_port = htons(port);

		memset(buffer, 0, sizeof(buffer));
	}
	/*
	void UDPSocket::setExternal(char* address, unsigned short port)
	{
		addressSize = sizeof(externalAddress);
		externalPort = port;
		externalAddress.sin_family = AF_INET;

		if(address == NULL)	//External을 딱히 지정하지 않음
		{
			externalAddress.sin_addr.s_addr = INADDR_ANY;
		}
		else
		{
			externalAddress.sin_addr.s_addr = inet_addr(address);
		}
		externalAddress.sin_port = htons(port);

		memset(buffer, 0, sizeof(buffer));
	}
	*/
	ByteStream*	UDPSocket::receive()
	{
		SystemInformation*	si			= SystemInformation::getSystemInformationInstance();
		ByteStream*			recv_stream	= new ByteStream();

		/*recvSize = recvfrom(Socket, buffer, bufferSize, 0, NULL, 0);

		recv_stream->setData(si->getSystemByteOrderingType(), (unsigned char*)buffer, recvSize);*/

		recvSize = recvfrom(Socket, (char*)recv_stream->getData(), ByteStream::DATA_LENGTH_MAX, 0, NULL, 0);
		recv_stream->length	= recvSize;
		recv_stream->ordering	= si->getSystemByteOrderingType();

		return recv_stream;
	}

	void UDPSocket::send(void* data, int length)
	{
		//memcpy(buffer, data, length);
		//int sendSize = sendto(Socket, buffer, length, 0, (struct sockaddr *)&externalAddress, addressSize);
		sendto(Socket, (const char*)data, length, 0, (struct sockaddr *)&externalAddress, addressSize);
	}

	void UDPSocket::setReusable()
	{
		if(setsockopt(Socket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) < 0)
		{
			std::cout<< "ERROR : setReusable()" << std::endl;
#ifdef	_WIN32
			closesocket(Socket);
			WSACleanup();
#else
			close(Socket);
#endif
			exit(0);
		}
	}

	void UDPSocket::setMulticastGroup()
	{
		mreq.imr_multiaddr.s_addr = inet_addr("239.255.0.1");
		mreq.imr_interface.s_addr = internalAddress.sin_addr.s_addr;

		if(setsockopt(Socket, SOCKET_OPTION_IPPROTO_IP, SOCKET_OPTION_IP_ADD_MEMBERSHIP, (char*)&mreq, sizeof(mreq)) == SOCKET_ERROR_VALUE_NATIVE)
		{
			//printf("ERROR : setsocketopt\n");
			std::cout<< "ERROR : setMulticastGroup()" << std::endl;
			exit(0);
		}
	}

	unsigned short UDPSocket::getExternalPort()
	{
		return externalPort;
	}

	unsigned long	UDPSocket::getInternalAddressToLong()
	{
		return internalAddress.sin_addr.s_addr;
	}

	unsigned long	UDPSocket::getExternalAddressToLong()
	{
		return externalAddress.sin_addr.s_addr;
	}
}
