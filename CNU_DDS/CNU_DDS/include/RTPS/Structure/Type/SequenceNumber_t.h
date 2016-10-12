#ifndef SEQUENCENUMBER_T_H_
#define SEQUENCENUMBER_T_H_

#include "../../../Utility/DoublyLinkedList.h"

namespace CNU_DDS
{
	class SequenceNumber_t
	{
	public:
		long			high;
		unsigned long	low;

	public:
		SequenceNumber_t();
		SequenceNumber_t(long a_high, unsigned long a_low);

		SequenceNumber_t	operator	- (const SequenceNumber_t& arg);
		SequenceNumber_t	operator	+ (const SequenceNumber_t& arg);
		void				operator	+=(const SequenceNumber_t& arg);
		void				operator	= (const SequenceNumber_t& arg);
		bool				operator	==(const SequenceNumber_t& arg);
		bool				operator	<=(const SequenceNumber_t& arg);
		bool				operator	>=(const SequenceNumber_t& arg);
		bool				operator	< (const SequenceNumber_t& arg);
	};

	typedef DoublyLinkedList<SequenceNumber_t>	SequenceNumberSeq;

	const SequenceNumber_t	SEQUENCE_NUMBER_UNKNOWN	= SequenceNumber_t(-1, 0);
}

#endif
