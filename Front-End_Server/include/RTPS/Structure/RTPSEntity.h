#ifndef RTPSENTITY_H_
#define RTPSENTITY_H_

#include "Type/GUID_t.h"

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
	class CNU_DDS_DLL_API RTPSEntity
	{
	public:
		GUID_t				guid;
	};
}

#ifdef __cplusplus
}
#endif
#endif
