#ifndef HEADER_H_
#define HEADER_H_

#include "Type/ProtocolId_t.h"

#include "../Structure/Type/ProtocolVersion_t.h"
#include "../Structure/Type/VendorId_t.h"
#include "../Structure/Type/GuidPrefix_t.h"

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
	class CNU_DDS_DLL_API Header
	{
	public:
		ProtocolId_t		protocol;
		ProtocolVersion_t	version;
		VendorId_t			vendor_id;
		GuidPrefix_t		guid_prefix;
	};
}

#ifdef __cplusplus
}
#endif
#endif
