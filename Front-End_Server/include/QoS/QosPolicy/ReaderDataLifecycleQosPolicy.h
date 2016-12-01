#ifndef READERDATALIFECYCLEQOSPOLICY_H_
#define READERDATALIFECYCLEQOSPOLICY_H_

#include "QosPolicy.h"
#include "../../DCPS/Infrastructure/Type/Duration_t.h"

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
	class CNU_DDS_DLL_API ReaderDataLifecycleQosPolicy : public QosPolicy
	{
	public:
		Duration_t	auto_purge_no_writer_samples_delay;
		Duration_t	auto_purge_disposed_samples_delay;

	public:
		ReaderDataLifecycleQosPolicy(void);
		~ReaderDataLifecycleQosPolicy(void);
	};
}

#ifdef __cplusplus
}
#endif
#endif
