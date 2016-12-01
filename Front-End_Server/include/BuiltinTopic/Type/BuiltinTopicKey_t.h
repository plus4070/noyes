#ifndef BUILTINTOPICKEY_T_H_
#define BUILTINTOPICKEY_T_H_

#include "../../DCPS/Infrastructure/Type/BasicDefinitions.h"

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
	typedef struct
	{
		BUILTIN_TOPIC_KEY_TYPE_NATIVE	value[3];
	}BuiltinTopicKey_t;
}

#ifdef __cplusplus
}
#endif
#endif
