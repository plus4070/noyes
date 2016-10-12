#ifndef INCLUDE_DCPS_BUILTINENTITY_BUILTINPUBLICATIONSWRITER_H_
#define INCLUDE_DCPS_BUILTINENTITY_BUILTINPUBLICATIONSWRITER_H_

#include "../Publication/DataWriter.h"

namespace CNU_DDS
{
	class BuiltinPublicationsWriter: public DataWriter
	{
	public:
		BuiltinPublicationsWriter();
		~BuiltinPublicationsWriter();

		ReturnCode_t		write(DDS_Data* data, InstanceHandle_t handle);
	};
}

#endif /* INCLUDE_DCPS_BUILTINENTITY_BUILTINPUBLICATIONSWRITER_H_ */
