#ifndef QUERYCONDITION_H_
#define QUERYCONDITION_H_

#include "ReadCondition.h"

namespace CNU_DDS
{
	class QueryCondition : public ReadCondition
	{
	public:
		std::string		get_query_expression(void);
		ReturnCode_t	get_query_parameters(StringSeq* query_parameters);
		ReturnCode_t	set_query_parameters(StringSeq* query_parameters);
	};
}

#endif
