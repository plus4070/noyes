#ifndef SAMPLEINFO_H_
#define SAMPLEINFO_H_

#include "../../Utility/DoublyLinkedList.h"

#include "../../Status/Type/SampleStateKind.h"
#include "../../Status/Type/ViewStateKind.h"
#include "../../Status/Type/InstanceStateKind.h"

#include "../Infrastructure/Type/InstanceHandle_t.h"
#include "../Infrastructure/Type/Time_t.h"

namespace CNU_DDS
{
	class SampleInfo
	{
	public:
		SampleStateKind		sample_state;
		ViewStateKind		view_state;
		InstanceStateKind	instance_state;
		long				disposed_generation_count;
		long				no_writers_generation_count;
		long				sample_rank;
		long				generation_rank;
		long				absolute_generation_rank;
		Time_t				source_timestamp;
		InstanceHandle_t	instance_handle;
		InstanceHandle_t	publication_handle;
		bool				valid_data;
	};

	typedef	DoublyLinkedList<SampleInfo>	SampleInfoList;
}

#endif
