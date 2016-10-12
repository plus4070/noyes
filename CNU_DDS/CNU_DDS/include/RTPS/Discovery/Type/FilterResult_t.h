#ifndef FILTERRESULT_T_H_
#define FILTERRESULT_T_H_

#include "../../../Utility/DoublyLinkedList.h"

namespace CNU_DDS
{
	class FilterResult_t
	{
	public:
		DoublyLinkedList<long>	value;
	};
}

#endif
