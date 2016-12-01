#ifndef SUBMESSAGEHEADER_H_
#define SUBMESSAGEHEADER_H_

#include "Type/SubmessageKind.h"
#include "Type/SubmessageFlag.h"


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
	class CNU_DDS_DLL_API SubmessageHeader
	{
	public:
		SubmessageKind	submessage_id;
		SubmessageFlag	flags;
		unsigned short	submessage_length;
	};
}

#ifdef __cplusplus
}
#endif
#endif
