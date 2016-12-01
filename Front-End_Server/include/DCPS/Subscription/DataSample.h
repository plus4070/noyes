#ifndef DATASAMPLE_H_
#define DATASAMPLE_H_

#include "../../Utility/DoublyLinkedList.h"
#include "SampleInfo.h"
#include "../Topic/DDS_Data.h"

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
	class CNU_DDS_DLL_API DataSample
	{
	public:
		SampleInfo*	info;
		DDS_Data*	value;
	};

	typedef DoublyLinkedList<DataSample>	DataSampleSeq;
}

#ifdef __cplusplus
}
#endif
#endif
