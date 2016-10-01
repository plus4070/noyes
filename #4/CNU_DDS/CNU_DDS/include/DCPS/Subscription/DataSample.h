#ifndef DATASAMPLE_H_
#define DATASAMPLE_H_

#include "../../Utility/DoublyLinkedList.h"
#include "SampleInfo.h"
#include "../Topic/DDS_Data.h"

namespace CNU_DDS
{
	class DataSample
	{
	public:
		SampleInfo*	info;
		DDS_Data*	value;
	};

	typedef DoublyLinkedList<DataSample>	DataSampleSeq;
}

#endif
