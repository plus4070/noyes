#ifndef INCLUDE_UTILITY_NETWORK_UDPSOCKET_H_
#define INCLUDE_UTILITY_NETWORK_UDPSOCKET_H_

#include "../PlatformOrientedInterface.h"
#include "../../RTPS/Messages/Type/ByteStream.h"

#ifdef _WIN32
	#pragma comment(lib, "ws2_32.lib")
	#include <winsock2.h>
	#include <ws2tcpip.h>
	#include <stdio.h>
	#include <stdlib.h>
#else
	#include <sys/types.h>
	#include <sys/stat.h>
	#include <sys/socket.h>
	#include <unistd.h>
	#include <netinet/in.h>
	#include <arpa/inet.h>
#endif

#ifdef CNU_DDS_DLL
#define CNU_DDS_DLL_API	__declspec(dllexport)	
#else
#define CNU_DDS_DLL_API	__declspec(dllimport)
#endif
#ifdef __cplusplus
extern "C" {
#endif
namespace CNU_DDS
{
	#define MAX_BUFFER_SIZE 2048
	//#define MAX_BUFFER_SIZE 1024
	//#define MAX_BUFFER_SIZE 512

#ifdef _WIN32	// Windows platform
	#define	SOCKET_TYPE_NATIVE			SOCKET
	#define	SOCKET_ADDRESS_TYPE_NATIVE	SOCKADDR_IN
	#define	SOCKET_ERROR_VALUE_NATIVE	SOCKET_ERROR
	#define	WSADATA_TYPE_NATIVE			WSADATA
#else	// Linux, Unix platform
	#define	SOCKET_TYPE_NATIVE			int
	#define	SOCKET_ADDRESS_TYPE_NATIVE	struct sockaddr_in
	#define	SOCKET_ERROR_VALUE_NATIVE	-1
	#define	WSADATA_TYPE_NATIVE			int
#endif
	#define	SOCKET_OPTION_IPPROTO_IP		0
	#define	SOCKET_OPTION_IP_ADD_MEMBERSHIP	12
	#define	SOCKET_OPTION_IPPROTO_UDP		17


	class CNU_DDS_DLL_API UDPSocket
	{
	//private:
	public:
		WSADATA_TYPE_NATIVE			wsaData;
		SOCKET_TYPE_NATIVE			Socket;
		SOCKET_ADDRESS_TYPE_NATIVE	internalAddress;
		SOCKET_ADDRESS_TYPE_NATIVE	externalAddress;
		unsigned short				internalPort;
		unsigned short				externalPort;

		int							addressSize;

		char				buffer[MAX_BUFFER_SIZE];
		int					bufferSize;
		int					recvSize;
		static const char	yes;
		struct ip_mreq		mreq;

	private:
		void	initUDPSocket();

	public:
		UDPSocket();
		UDPSocket(char* address, unsigned short port);
		~UDPSocket();

		void		setInternalAddressByPureStr(char* address);
		void		setInternalAddressByDDSLocator(char* address);
		void		setInternalAddress(long address);
		//void		setInternalAddress(char* address);
		void		setInternalPort(unsigned short port);
		void		binding();

		void		setExternalByPureStr(char* address, unsigned short port);
		void		setExternalByDDSLocator(char* address, unsigned short port);
		void		setExternal(long address, unsigned short port);
		//void		setExternal(char* address, unsigned short port);
		ByteStream*	receive();

		void		send(void* data, int length);

		void		setReusable();
		void		setMulticastGroup();

		unsigned short	getExternalPort();
		unsigned long	getInternalAddressToLong();
		unsigned long	getExternalAddressToLong();
	};
}

#ifdef __cplusplus
}
#endif
#endif /* INCLUDE_UTILITY_NETWORK_UDPSOCKET_H_ */
