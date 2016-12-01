#ifndef SUBMESSAGEELEMENT_H_
#define SUBMESSAGEELEMENT_H_

#include "../Type/ByteStream.h"

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
	class CNU_DDS_DLL_API SubmessageElement
	{
	public:
	};
}

#ifdef __cplusplus
}
#endif
#endif
