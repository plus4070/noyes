#ifndef PUBLISHERSTATUS_H_
#define PUBLISHERSTATUS_H_

#include "EntityStatus.h"

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
	class CNU_DDS_DLL_API PublisherStatus : public EntityStatus
	{
	public:

	};
}

#ifdef __cplusplus
}
#endif
#endif