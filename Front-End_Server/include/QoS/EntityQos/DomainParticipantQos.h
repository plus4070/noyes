#ifndef DOMAINPARTICIPANTQOS_H_
#define DOMAINPARTICIPANTQOS_H_

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
	class CNU_DDS_DLL_API DomainParticipantQos : public EntityQos
	{
	public:
		UserDataQosPolicy		user_data;
		EntityFactoryQosPolicy	entity_factory;
	};
}

#ifdef __cplusplus
}
#endif
#endif
