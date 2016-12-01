#ifndef SUBSCRIBERLISTENER_H_
#define SUBSCRIBERLISTENER_H_

#include "../../Status/Listener.h"
#include "DataReaderListener.h"

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
	class CNU_DDS_DLL_API SubscriberListener : public DataReaderListener
	{
	public:
		virtual void	on_data_on_readers(pSubscriber the_subscriber)	= 0;
	};
}

#ifdef __cplusplus
}
#endif
#endif
