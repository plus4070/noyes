#ifndef WRITERDATALIFECYCLEQOSPOLICY_H_
#define WRITERDATALIFECYCLEQOSPOLICY_H_

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
	class CNU_DDS_DLL_API WriterDataLifecycleQosPolicy : public QosPolicy
	{
	public:
		bool	auto_dispose_unregistered_instances;

	public:
		WriterDataLifecycleQosPolicy(void);
		~WriterDataLifecycleQosPolicy(void);
	};
}

#ifdef __cplusplus
}
#endif
#endif
