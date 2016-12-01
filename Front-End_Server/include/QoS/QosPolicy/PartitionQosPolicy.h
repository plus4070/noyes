#ifndef PARTITIONQOSPOLICY_H_
#define PARTITIONQOSPOLICY_H_

#include "QosPolicy.h"
#include "../../DCPS/Infrastructure/Type/BasicDefinitions.h"

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
	class CNU_DDS_DLL_API PartitionQosPolicy : public QosPolicy
	{
	public:
		StringSeq	partition_name;

	public:
		PartitionQosPolicy(void);
		~PartitionQosPolicy(void);
	};
}

#ifdef __cplusplus
}
#endif
#endif
