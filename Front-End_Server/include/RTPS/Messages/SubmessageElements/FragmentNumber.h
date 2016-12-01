#ifndef FRAGMENTNUMBER_H_
#define FRAGMENTNUMBER_H_

#include "SubmessageElement.h"

#include "../Type/FragmentNumber_t.h"

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
	class CNU_DDS_DLL_API FragmentNumber : public SubmessageElement
	{
	public:
		FragmentNumber_t	value;

	};
}

#ifdef __cplusplus
}
#endif
#endif
