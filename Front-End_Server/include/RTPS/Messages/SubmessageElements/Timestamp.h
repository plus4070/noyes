#ifndef TIMESTAMP_H_
#define TIMESTAMP_H_

#include "SubmessageElement.h"

#include "../Type/Time_t.h"

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
	class CNU_DDS_DLL_API Timestamp : public SubmessageElement
	{
	public:
		Time_t	value;
	};
}

#ifdef __cplusplus
}
#endif
#endif
