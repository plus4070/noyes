#ifndef PARAMETERLIST_H_
#define PARAMETERLIST_H_

#include "SubmessageElement.h"
#include "Parameter.h"

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
	class CNU_DDS_DLL_API ParameterList : public SubmessageElement
	{
	public:
		ParameterSeq	parameters;
	};
}

#ifdef __cplusplus
}
#endif
#endif
