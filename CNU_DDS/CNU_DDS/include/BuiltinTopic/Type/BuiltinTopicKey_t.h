#ifndef BUILTINTOPICKEY_T_H_
#define BUILTINTOPICKEY_T_H_

#include "../../DCPS/Infrastructure/Type/BasicDefinitions.h"

namespace CNU_DDS
{
	typedef struct
	{
		BUILTIN_TOPIC_KEY_TYPE_NATIVE	value[3];
	}BuiltinTopicKey_t;
}

#endif
