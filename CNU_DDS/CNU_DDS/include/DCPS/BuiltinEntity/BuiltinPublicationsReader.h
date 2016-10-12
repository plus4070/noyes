#ifndef INCLUDE_DCPS_BUILTINENTITY_BUILTINPUBLICATIONSREADER_H_
#define INCLUDE_DCPS_BUILTINENTITY_BUILTINPUBLICATIONSREADER_H_

#include "../Subscription/DataReader.h"

namespace CNU_DDS
{
	class BuiltinPublicationsReader: public DataReader
	{
	public:
		BuiltinPublicationsReader();
		~BuiltinPublicationsReader();
	};
}


#endif /* INCLUDE_DCPS_BUILTINENTITY_BUILTINPUBLICATIONSREADER_H_ */
