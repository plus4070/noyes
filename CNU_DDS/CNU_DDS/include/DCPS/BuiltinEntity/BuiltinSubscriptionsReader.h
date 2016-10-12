#ifndef INCLUDE_DCPS_BUILTINENTITY_BUILTINSUBSCRIPTIONSREADER_H_
#define INCLUDE_DCPS_BUILTINENTITY_BUILTINSUBSCRIPTIONSREADER_H_

#include "../Subscription/DataReader.h"

namespace CNU_DDS
{
	class BuiltinSubscriptionsReader: public DataReader
	{
	public:
		BuiltinSubscriptionsReader();
		~BuiltinSubscriptionsReader();
	};
}

#endif /* INCLUDE_DCPS_BUILTINENTITY_BUILTINSUBSCRIPTIONSREADER_H_ */
