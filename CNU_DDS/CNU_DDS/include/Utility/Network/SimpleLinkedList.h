#ifndef INCLUDE_UTILITY_NETWORK_SIMPLELINKEDLIST_H_
#define INCLUDE_UTILITY_NETWORK_SIMPLELINKEDLIST_H_

namespace CNU_DDS
{
	class SimpleLinkNode
	{
	public:
		SimpleLinkNode*	rear;
		unsigned long	value;
	};

	class SimpleLinkedList
	{
	public:
		SimpleLinkNode*	head;
		SimpleLinkNode*	last;

	public:
		SimpleLinkedList();
		~SimpleLinkedList();

		void			insertInRear(unsigned long new_value);

		unsigned long	popFirst();
	};
}

#endif
