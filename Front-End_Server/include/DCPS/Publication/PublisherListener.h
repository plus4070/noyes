#ifndef PUBLISHERLISTENER_H_
#define PUBLISHERLISTENER_H_

#include "../../Status/Listener.h"
#include "DataWriterListener.h"

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
	class CNU_DDS_DLL_API PublisherListener : public DataWriterListener
	{
	public:
	};
}

#ifdef __cplusplus
}
#endif
#endif
