#ifndef SERIALIZEDDATAFRAGMENT_H_
#define SERIALIZEDDATAFRAGMENT_H_

#include "SubmessageElement.h"

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
	class CNU_DDS_DLL_API SerializedDataFrag: public SubmessageElement
	{
	public:
		char*	value;
	};
}

#ifdef __cplusplus
}
#endif
#endif
