#ifndef QUERYCONDITION_H_
#define QUERYCONDITION_H_

#include "ReadCondition.h"

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
	class CNU_DDS_DLL_API QueryCondition : public ReadCondition
	{
	public:
		std::string		get_query_expression(void);
		ReturnCode_t	get_query_parameters(StringSeq* query_parameters);
		ReturnCode_t	set_query_parameters(StringSeq* query_parameters);
	};
}

#ifdef __cplusplus
}
#endif
#endif
