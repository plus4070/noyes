#ifndef GROUPDATAQOSPOLICY_H_
#define GROUPDATAQOSPOLICY_H_

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
	const unsigned long	MAX_SIZE_OF_GROUP_DATA_QOS_POLICY	= 256;

	class CNU_DDS_DLL_API GroupDataQosPolicy : public QosPolicy
	{
	public:
		unsigned char	value[MAX_SIZE_OF_GROUP_DATA_QOS_POLICY];

	public:
		GroupDataQosPolicy(void);
		~GroupDataQosPolicy(void);
	};
}

#ifdef __cplusplus
}
#endif
#endif
