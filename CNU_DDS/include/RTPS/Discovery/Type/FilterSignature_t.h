#ifndef FILTERSIGNATURE_T_H_
#define FILTERSIGNATURE_T_H_

#include "../../../Utility/DoublyLinkedList.h"

namespace CNU_DDS
{
	class FilterSignature_t
	{
	public:
		long	value[4];
	};

	typedef	DoublyLinkedList<FilterSignature_t>	FilterSignatureSeq;
}

#endif
