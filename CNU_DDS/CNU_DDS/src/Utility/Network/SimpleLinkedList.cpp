#include <stdio.h>
#include "../../../include/Utility/Network/SimpleLinkedList.h"

namespace CNU_DDS
{

	SimpleLinkedList::SimpleLinkedList()
	{
		head	= NULL;
		last	= NULL;
	}

	SimpleLinkedList::~SimpleLinkedList()
	{

	}

	void			SimpleLinkedList::insertInRear(unsigned long new_value)
	{
		SimpleLinkNode*	new_node	= new SimpleLinkNode();

		new_node->value	= new_value;
		new_node->rear	= NULL;

		if(head == NULL)
		{
			head		= new_node;
			last		= new_node;
		}
		else
		{
			last->rear		= new_node;
			last			= new_node;
		}
	}

	unsigned long	SimpleLinkedList::popFirst()
	{
		unsigned long	ret_value;
		SimpleLinkNode*	next;

		if(head == NULL)
		{
			ret_value	= 0;
		}
		else
		{
			ret_value	= head->value;
			next		= head->rear;
			delete(head);
			head		= next;

			if(head == NULL)
			{
				last	= NULL;
			}
		}

		return ret_value;
	}
}
