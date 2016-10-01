#include "../../../include/Utility/Timer/ListHeader.h"

namespace CNU_DDS
{
	ListHeader::ListHeader()
	{
		prev = this;
		next = this;
	}

	ListHeader::ListHeader(ListHeader* a_Prev, ListHeader* a_Next)
	{
		prev = a_Prev;
		next = a_Next;
	}

	ListHeader::~ListHeader()
	{

	}

	void ListHeader::setPrev(ListHeader* a_Prev)
	{
		prev = a_Prev;
	}

	void ListHeader::setNext(ListHeader* a_Next)
	{
		next = a_Next;
	}

	ListHeader* ListHeader::getPrev()
	{
		return prev;
	}

	ListHeader* ListHeader::getNext()
	{
		return next;
	}

	void ListHeader::disconnectThis()
	{
		if(prev != 0 && next != 0)
		{
			prev->setNext(next);
			next->setPrev(prev);

			prev	= this;
			next	= this;
		}
		else
		{
			//error
		}
	}

	ListHeader*	ListHeader::disconnectMeaningfulList()
	{
		ListHeader*	returnValue	= 0;

		if(prev != 0 && next != 0)
		{
			returnValue	= next;

			next->prev	= prev;
			prev->next	= next;
			next		= this;
			prev		= this;
		}
		else
		{
			//error
		}

		return returnValue;
	}

	void ListHeader::listReplaceInit(ListHeader* oldList, ListHeader* newList)
	{
		if(oldList != oldList->next && oldList->next && oldList->prev)
		{
			newList->next		= oldList->next;
			newList->prev		= oldList->prev;
			newList->prev->next	= newList;
			newList->next->prev	= newList;

			oldList->next		= oldList;
			oldList->prev		= oldList;
		}
	}

	bool ListHeader::isEmpty()
	{
		if(prev == this && next == this)
		{
			return true;
		}
		return false;
	}

	void ListHeader::list_add_tail(ListHeader* newList)
	{
		ListHeader*	tmp;

		if(newList && this)
		{
			tmp				= newList->prev;

			newList->prev	= this->prev;
			prev->next		= newList;
			this->prev		= tmp;
			tmp->next		= this;
		}
		else
		{

		}
	}

}
