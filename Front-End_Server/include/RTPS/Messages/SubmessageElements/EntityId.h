#ifndef ENTITYID_H_
#define ENTITYID_H_

#include "SubmessageElement.h"

#include "../../Structure/Type/EntityId_t.h"

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
	class CNU_DDS_DLL_API EntityId : public SubmessageElement
	{
	public:
		EntityId_t	value;
	};
}

#ifdef __cplusplus
}
#endif
#endif
