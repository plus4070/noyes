#ifndef ENTITYFACTORYQOSPOLICY_H_
#define ENTITYFACTORYQOSPOLICY_H_

#include "QosPolicy.h"

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
	class CNU_DDS_DLL_API EntityFactoryQosPolicy : public QosPolicy
	{
	public:
		bool	auto_enable_created_entities;

	public:
		EntityFactoryQosPolicy(void);
		~EntityFactoryQosPolicy(void);
	};
}

#ifdef __cplusplus
}
#endif
#endif
