#ifndef LOCATORLIST_H_
#define LOCATORLIST_H_

#include "SubmessageElement.h"

#include "../../Structure/Type/Locator_t.h"

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
	class CNU_DDS_DLL_API LocatorList : public SubmessageElement
	{
	public:
		LocatorSeq	value;
	};
}

#ifdef __cplusplus
}
#endif
#endif
