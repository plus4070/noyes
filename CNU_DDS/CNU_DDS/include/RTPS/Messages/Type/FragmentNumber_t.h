#ifndef FRAGMENTNUMBER_T_H_
#define FRAGMENTNUMBER_T_H_

#include "../../../Utility/DoublyLinkedList.h"

namespace CNU_DDS
{
	class FragmentNumber_t
	{
	public:
		unsigned long	value;

		FragmentNumber_t	operator	+ (const FragmentNumber_t& arg);
		FragmentNumber_t	operator	- (const FragmentNumber_t& arg);
		void				operator	= (const FragmentNumber_t& arg);
	};

	typedef DoublyLinkedList<FragmentNumber_t>	FragmentNumberSeq;
}

#endif
