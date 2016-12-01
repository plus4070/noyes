#ifndef COUNT_H_
#define COUNT_H_

#include "SubmessageElement.h"

#include "../Type/Count_t.h"

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
	class CNU_DDS_DLL_API Count : public SubmessageElement
	{
	public:
		Count_t	value;
	};
}

#ifdef __cplusplus
}
#endif
#endif
