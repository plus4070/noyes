#ifndef FILTERSIGNATURE_T_H_
#define FILTERSIGNATURE_T_H_

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
	class CNU_DDS_DLL_API FilterSignature_t
	{
	public:
		long	value[4];
	};

	typedef	DoublyLinkedList<FilterSignature_t>	FilterSignatureSeq;
}

#ifdef __cplusplus
}
#endif
#endif
