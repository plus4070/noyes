#ifndef INCLUDE_UTILITY_NETWORK_TRANSCEIVERMANAGER_H_
#define INCLUDE_UTILITY_NETWORK_TRANSCEIVERMANAGER_H_

#include "../DoublyLinkedList.h"
#include "TransceiverInfo.h"

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
	class CNU_DDS_DLL_API TransceiverManager
	{
	private:
		DoublyLinkedList<TransceiverInfo>*	transceivers;
		THREAD_TYPE_NATIVE					switching_handle;

	private:
		TransceiverManager();

	public:
		~TransceiverManager();

		static TransceiverManager*	getManagerInstance();
		Transceiver*				registerTransceiver(Locator_t a_locator, unsigned long a_tid);
		Transceiver*				getTransceiverInstance(unsigned long a_tid);

		//static void*				switching(void* arg);
	};

	const static unsigned long	TRANSCEIVER_ID_DOMAIN_ID_GAIN			= 0x00000100;
	const static unsigned char	TRANSCEIVER_ID_METATRAFFIC_MULTICAST	= 0x01;
	const static unsigned char	TRANSCEIVER_ID_METATRAFFIC_UNICAST		= 0x02;
	const static unsigned char	TRANSCEIVER_ID_DEFAULT_MULTICAST		= 0x04;
	const static unsigned char	TRANSCEIVER_ID_DEFAULT_UNICAST			= 0x08;
}

#ifdef __cplusplus
}
#endif
#endif /* INCLUDE_UTILITY_NETWORK_TRANSCEIVERMANAGER_H_ */
