#ifndef SUBSCRIBERQOS_H_
#define SUBSCRIBERQOS_H_

#include "EntityQos.h"
#include "../QosPolicies.h"

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
	class CNU_DDS_DLL_API SubscriberQos : public EntityQos
	{
	public:
		PresentationQosPolicy	presentation;
		PartitionQosPolicy		partition;
		GroupDataQosPolicy		group_data;
		EntityFactoryQosPolicy	entity_factory;
	};
}

#ifdef __cplusplus
}
#endif
#endif
