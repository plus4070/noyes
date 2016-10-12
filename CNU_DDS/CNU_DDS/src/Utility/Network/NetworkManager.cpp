#include "../../../include/Utility/Network/NetworkManager.h"

namespace CNU_DDS
{
	NetworkInfoManager*	NETWORK_INFO_MANAGER_INSTANCE;

	NetworkInfoManager::NetworkInfoManager()
	{
		IPAddresses = new DoublyLinkedList<IPAddress>();
		resetIPAddresses();
	}

	NetworkInfoManager::~NetworkInfoManager()
	{

	}

	NetworkInfoManager*		NetworkInfoManager::getNetworkInfoManagerInstance()
	{
		if(NETWORK_INFO_MANAGER_INSTANCE == 0)
		{
			NETWORK_INFO_MANAGER_INSTANCE	= new NetworkInfoManager();
		}

		return NETWORK_INFO_MANAGER_INSTANCE;
	}

	void NetworkInfoManager::resetIPAddresses()
	{
#ifdef _WIN32

		WSADATA wsa;
		IN_ADDR addr;
	
		char LocalName[256];
		char IPAddr[16];
		char i = 0;
	
		IPAddresses->deleteAll();
	
		if(WSAStartup(MAKEWORD(2,2), &wsa) != 0)
		{
			return ;
		}
	
		if(gethostname(LocalName, 256) == SOCKET_ERROR)
		{
			return ;
		}
	
		HOSTENT *ptr = gethostbyname(LocalName);
	
		if(ptr == NULL)
		{
			return ;
		}
		
		while(ptr->h_addr_list[i] != NULL)
		{
	/*
			// ReTiCom
			memcpy(&addr, ptr->h_addr_list[i], ptr->h_length);
			wsprintf(IPAddr, "%s", inet_ntoa(addr));
			IPAddresses.push_back(IPAddress(IPAddr));
	*/
			memcpy(&addr, ptr->h_addr_list[i], ptr->h_length);
			sprintf(IPAddr, "%s", inet_ntoa(addr));
			printf("IPAddr(%s)\n", IPAddr);
		
			IPAddress*	new_ip	= new IPAddress(IPAddr);
			IPAddresses->insertInRear(new_ip);

			i++;
		}
		WSACleanup();

#else
	    struct ifreq*		ifr;
	    struct sockaddr_in*	sin;
	    struct sockaddr*	sa;
	    struct ifconf 		ifcfg;
	    int 				fd;
	    int 				n;
	    int 				numreqs = 30;

	    fd = socket(AF_INET, SOCK_DGRAM, 0);
	    memset(&ifcfg, 0, sizeof(ifcfg));
	    ifcfg.ifc_buf = NULL;
	    ifcfg.ifc_len = sizeof(struct ifreq) * numreqs;
	    ifcfg.ifc_buf = (char*)malloc(ifcfg.ifc_len);

	    for(;;)
	    {
	        if (ioctl(fd, SIOCGIFCONF, (char *)&ifcfg) < 0)
	        {
	            perror("SIOCGIFCONF ");
	            exit;
	        }
	        break;
	    }

	    ifr = ifcfg.ifc_req;
	    IPAddresses.deleteAll();

	    for (n = 0; n < ifcfg.ifc_len; n+= sizeof(struct ifreq))
	    {
	        sin = (struct sockaddr_in *)&ifr->ifr_addr;
	        IPAddress*	new_ip	= new IPAddress(inet_ntoa(sin->sin_addr));
	        IPAddresses.insertInRear(new_ip);
	        ifr++;
	    }
	    close(fd);
#endif
	}

	DoublyLinkedList<IPAddress>* NetworkInfoManager::getLocalIPAddressList()
	{
		return IPAddresses;
	}
}
