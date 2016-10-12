#ifndef INCLUDE_DCPS_BUILTINENTITY_BUILTINSUBSCRIPTIONSWRITER_H_
#define INCLUDE_DCPS_BUILTINENTITY_BUILTINSUBSCRIPTIONSWRITER_H_

#include "../Publication/DataWriter.h"

namespace CNU_DDS
{
	class BuiltinSubscriptionsWriter: public DataWriter
	{
	public:
		BuiltinSubscriptionsWriter();
		~BuiltinSubscriptionsWriter();

		ReturnCode_t		write(DDS_Data* data, InstanceHandle_t handle);
	};
}

#endif /* INCLUDE_DCPS_BUILTINENTITY_BUILTINSUBSCRIPTIONSWRITER_H_ */
