#ifndef USERDATAQOSPOLICY_H_
#define USERDATAQOSPOLICY_H_

#include "QosPolicy.h"

namespace CNU_DDS
{
	const unsigned long	MAX_SIZE_OF_USER_DATA_QOS_POLICY	= 256;

	class UserDataQosPolicy : public QosPolicy
	{
	public:
		unsigned char	value[MAX_SIZE_OF_USER_DATA_QOS_POLICY];

	public:
		UserDataQosPolicy(void);
		~UserDataQosPolicy(void);
	};
}

#endif
