#ifndef PARAMETER_H_
#define PARAMETER_H_

#include "SubmessageElement.h"

#include "../Type/ParameterId_t.h"

#include "../../../Utility/DoublyLinkedList.h"

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
	class CNU_DDS_DLL_API Parameter : public SubmessageElement
	{
	public:
		ParameterId_t	parameter_id;
		short			length;
		char*			value;
	};

	typedef DoublyLinkedList<Parameter>	ParameterSeq;
}

#ifdef __cplusplus
}
#endif
#endif
