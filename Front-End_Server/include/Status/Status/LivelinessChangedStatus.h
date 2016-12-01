#ifndef LIVELINESSCHANGEDSTATUS_H_
#define LIVELINESSCHANGEDSTATUS_H_

#include "Status.h"
#include "../../DCPS/Infrastructure/Type/InstanceHandle_t.h"

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
	class CNU_DDS_DLL_API LivelinessChangedStatus:public Status
	{
	public:
		long				alive_count;
		long				not_alive_count;
		long				alive_count_change;
		long				not_alive_count_change;
		InstanceHandle_t	last_publication_handle;
	};
}

#ifdef __cplusplus
}
#endif
#endif
