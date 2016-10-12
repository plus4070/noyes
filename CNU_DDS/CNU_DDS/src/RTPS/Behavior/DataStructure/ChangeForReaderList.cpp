#include "../../../../include/RTPS/Behavior/DataStructure/ChangeForReaderList.h"
#include <stdio.h>

namespace CNU_DDS
{
	ChangeForReaderList::ChangeForReaderList(unsigned long limit)
	{
		CFRlist	= new CFRnode[limit];
		memset(CFRlist, 0, sizeof(CFRnode)*limit);

		CFR_acked		= NULL;
		CFR_requested	= NULL;
		CFR_unacked		= NULL;
		CFR_underway	= NULL;
		CFR_unsent		= NULL;

		base_number	= SequenceNumber_t(0,1);
		size		= 0;
		head_index	= 0;
		size_limit	= limit;

		create_mutex(&mutex);
	}

	ChangeForReaderList::~ChangeForReaderList()
	{
		delete(CFRlist);
		destroy_mutex(&mutex);
	}

	bool				ChangeForReaderList::insertValue(ChangeForReader* cfr)
	{
		CFRnode**			cur_node	= NULL;
		CFRnode*			new_node	= NULL;
		SequenceNumber_t	calc_seq;
		unsigned long		calc_index;

		mutex_lock(&mutex);

		if(size == size_limit)
		{
			mutex_unlock(&mutex);
			return false;
		}
		else if(size == 0)
		{
			//base_number	= cfr->cache_change->sequence_number;
		}

		calc_seq	= cfr->cache_change->sequence_number - base_number;
		calc_index	= calc_seq.low;

		if( (calc_index > size_limit) || (calc_seq.high > 0) )
		{
			mutex_unlock(&mutex);
			return false;
		}

		switch(cfr->status)
		{
		case CHANGE_FOR_READER_STATUS_KIND_ACKNOWLEDGED:
			cur_node	= &CFR_acked;
			break;
		case CHANGE_FOR_READER_STATUS_KIND_REQUESTED:
			cur_node	= &CFR_requested;
			break;
		case CHANGE_FOR_READER_STATUS_KIND_UNACKNOWLEDGED:
			cur_node	= &CFR_unacked;
			break;
		case CHANGE_FOR_READER_STATUS_KIND_UNDERWAY:
			cur_node	= &CFR_underway;
			break;
		case CHANGE_FOR_READER_STATUS_KIND_UNSENT:
			cur_node	= &CFR_unsent;
			break;
		default:
			mutex_unlock(&mutex);
			return false;
		}

		new_node	= CFRlist + ((head_index + calc_index) % size_limit);

		if(new_node->value)
		{
			mutex_unlock(&mutex);
			return false;
		}
		else
		{
			size++;
			new_node->value	= cfr;
		}

		if(*cur_node	== NULL || (*cur_node)->value == NULL)
		{
			(*cur_node)	= new_node;
			new_node->prev	= new_node;
			new_node->rear	= new_node;
		}
		else
		{
			new_node->prev	= (*cur_node)->prev;
			new_node->rear	= (*cur_node);

			new_node->prev->rear	= new_node;
			new_node->rear->prev	= new_node;
		}

		mutex_unlock(&mutex);
		return true;
	}

	ChangeForReader*	ChangeForReaderList::getValueByIndex(unsigned long index)
	{
		unsigned long	calc_index;

		mutex_lock(&mutex);
		if(index > size_limit)
		{
			mutex_unlock(&mutex);
			return NULL;
		}
		else
		{
			calc_index	= (head_index + index) % size_limit;
		}

		mutex_unlock(&mutex);
		return CFRlist[calc_index].value;
	}

	ChangeForReader*	ChangeForReaderList::getValueBySequenceNumber(SequenceNumber_t seq_num)
	{
		SequenceNumber_t	calc_seq;
		unsigned long		calc_index;

		mutex_lock(&mutex);

		calc_seq	= seq_num - base_number;
		calc_index	= calc_seq.low;

		if( (calc_index >= size_limit) || (calc_seq.high > 0) )
		{
			mutex_unlock(&mutex);
			return NULL;
		}
		else
		{
			calc_index	= (head_index + calc_index) % size_limit;
		}

		mutex_unlock(&mutex);
		return CFRlist[calc_index].value;
	}

	ChangeForReader*	ChangeForReaderList::getFirstValueByStatus(ChangeForReaderStatusKind kind)
	{
		CFRnode**	cur_node;

		mutex_lock(&mutex);

		switch(kind)
		{
		case CHANGE_FOR_READER_STATUS_KIND_ACKNOWLEDGED:
			cur_node	= &CFR_acked;
			break;
		case CHANGE_FOR_READER_STATUS_KIND_REQUESTED:
			cur_node	= &CFR_requested;
			break;
		case CHANGE_FOR_READER_STATUS_KIND_UNACKNOWLEDGED:
			cur_node	= &CFR_unacked;
			break;
		case CHANGE_FOR_READER_STATUS_KIND_UNDERWAY:
			cur_node	= &CFR_underway;
			break;
		case CHANGE_FOR_READER_STATUS_KIND_UNSENT:
			cur_node	= &CFR_unsent;
			break;
		default:
			mutex_unlock(&mutex);
			return NULL;
		}

		if(*cur_node == NULL || (*cur_node)->value == NULL)
		{
			mutex_unlock(&mutex);
			return NULL;
		}

		mutex_unlock(&mutex);
		return (*cur_node)->value;
	}

	unsigned long		ChangeForReaderList::getValidSize()
	{
		return size;
	}

	unsigned long		ChangeForReaderList::getLimitSize()
	{
		return size_limit;
	}

	SequenceNumber_t	ChangeForReaderList::getBaseNumber()
	{
		return base_number;
	}

	ChangeForReader*	ChangeForReaderList::popFirstValueByStatus(ChangeForReaderStatusKind kind)
	{
		CFRnode**			cur_node	= NULL;
		CFRnode*			del_node	= NULL;
		ChangeForReader*	ret_value	= NULL;

		mutex_lock(&mutex);

		switch(kind)
		{
		case CHANGE_FOR_READER_STATUS_KIND_ACKNOWLEDGED:
			cur_node	= &CFR_acked;
			break;
		case CHANGE_FOR_READER_STATUS_KIND_REQUESTED:
			cur_node	= &CFR_requested;
			break;
		case CHANGE_FOR_READER_STATUS_KIND_UNACKNOWLEDGED:
			cur_node	= &CFR_unacked;
			break;
		case CHANGE_FOR_READER_STATUS_KIND_UNDERWAY:
			cur_node	= &CFR_underway;
			break;
		case CHANGE_FOR_READER_STATUS_KIND_UNSENT:
			cur_node	= &CFR_unsent;
			break;
		default:
			mutex_unlock(&mutex);
			return NULL;
		}

		if(*cur_node == NULL)
		{
			mutex_unlock(&mutex);
			return NULL;
		}

		ret_value	= (*cur_node)->value;

		if( ((*cur_node)->prev == (*cur_node)) && ((*cur_node)->rear == (*cur_node)) )
		{
			memset(*cur_node, 0, sizeof(CFRnode));
			*cur_node	= NULL;
		}
		else
		{
			(*cur_node)->prev->rear	= (*cur_node)->rear;
			(*cur_node)->rear->prev	= (*cur_node)->prev;
			del_node	= *cur_node;
			*cur_node	= (*cur_node)->rear;

			memset(del_node, 0, sizeof(CFRnode));
		}

		size--;
		mutex_unlock(&mutex);
		return ret_value;
	}

	void				ChangeForReaderList::_changeStatus(CFRnode* node, ChangeForReaderStatusKind kind)
	{
		CFRnode**	cur_node;
		CFRnode**	next_node;

		switch(node->value->status)
		{
		case CHANGE_FOR_READER_STATUS_KIND_ACKNOWLEDGED:
			cur_node	= &CFR_acked;
			return;		//acknowledged state does not need to be changed?
		case CHANGE_FOR_READER_STATUS_KIND_REQUESTED:
			cur_node	= &CFR_requested;
			break;
		case CHANGE_FOR_READER_STATUS_KIND_UNACKNOWLEDGED:
			cur_node	= &CFR_unacked;
			break;
		case CHANGE_FOR_READER_STATUS_KIND_UNDERWAY:
			cur_node	= &CFR_underway;
			break;
		case CHANGE_FOR_READER_STATUS_KIND_UNSENT:
			cur_node	= &CFR_unsent;
			break;
		default:
			return;
		}

		switch(kind)
		{
		case CHANGE_FOR_READER_STATUS_KIND_ACKNOWLEDGED:
			next_node	= &CFR_acked;
			break;
		case CHANGE_FOR_READER_STATUS_KIND_REQUESTED:
			next_node	= &CFR_requested;
			break;
		case CHANGE_FOR_READER_STATUS_KIND_UNACKNOWLEDGED:
			next_node	= &CFR_unacked;
			break;
		case CHANGE_FOR_READER_STATUS_KIND_UNDERWAY:
			next_node	= &CFR_underway;
			break;
		case CHANGE_FOR_READER_STATUS_KIND_UNSENT:
			next_node	= &CFR_unsent;
			break;
		default:
			return;
		}

		if(cur_node == next_node)
		{
			// do nothing
			//printf("1");
		}
		else if( (node->prev == node) && (node->rear == node) )
		{
			// if current list has just one node
			(*cur_node)	= NULL;
			//printf("2");
		}
		else if( ((*cur_node) == node) && ((*cur_node) != (*cur_node)->prev) )
		{
			// if current node is a first node of doubly linked list
			(*cur_node)	= node->rear;
			(*cur_node)->prev	= node->prev;
			(*cur_node)->prev->rear	= node->rear;
			//printf("3");
		}
		else
		{
			// current node is not a first node of doubly linked list
			node->prev->rear	= node->rear;
			node->rear->prev	= node->prev;
			//printf("\n????\n");
		}

		if(*next_node == NULL)
		{
			// target status list is empty
			*next_node	= node;
			node->prev	= node;
			node->rear	= node;
			//printf("4");
		}
		else
		{
			// target status list is not empty
			node->prev	= (*next_node)->prev;
			node->rear	= (*next_node);

			node->prev->rear	= node;
			node->rear->prev	= node;
			//printf("5");
		}

		node->value->status	= kind;
	}

	bool				ChangeForReaderList::changeStatusByIndex(unsigned long index, ChangeForReaderStatusKind kind)
	{
		unsigned long	calc_index;
		CFRnode*		calc_node;

		mutex_lock(&mutex);

		if(index >= size_limit)
		{
			mutex_unlock(&mutex);
			return false;
		}

		calc_index	= (index + head_index) % size_limit;
		calc_node	= CFRlist + calc_index;

		if(calc_node->value == NULL)
		{
			mutex_unlock(&mutex);
			return false;
		}
		else if(calc_node->value->status == kind)
		{
			mutex_unlock(&mutex);
			return true;
		}

		_changeStatus(calc_node, kind);

		mutex_unlock(&mutex);
		return true;
	}

	bool				ChangeForReaderList::changeStatusBySequenceNumber(SequenceNumber_t seq_num, ChangeForReaderStatusKind kind)
	{
		SequenceNumber_t	calc_seq;
		unsigned long		calc_index;
		CFRnode*			calc_node;

		calc_seq	= seq_num - base_number;
		calc_index	= calc_seq.low;

		mutex_lock(&mutex);

		if(calc_index >= size_limit)
		{
printf("ERROR: ChangeForReaderList::changeStatusBySequenceNumber() - E001\n");
			mutex_unlock(&mutex);
			return false;
		}

		calc_index	= (calc_index + head_index) % size_limit;
		calc_node	= CFRlist + calc_index;

		if(calc_node->value == NULL)
		{
printf("ERROR: ChangeForReaderList::changeStatusBySequenceNumber() - E002\n");
			mutex_unlock(&mutex);
			return false;
		}
		else if((calc_node->value->status == kind))
		{
			mutex_unlock(&mutex);
			return true;
		}

		_changeStatus(calc_node, kind);

		mutex_unlock(&mutex);
		return true;
	}

	bool				ChangeForReaderList::removeByIndex(unsigned long index)
	{
		unsigned long	calc_index;
		CFRnode*		del_node	= NULL;
		CFRnode**		cur_node	= NULL;

		//mutex_lock(&mutex);

		if(index >= size_limit)
		{
			//mutex_unlock(&mutex);
			return false;
		}

		calc_index	= (index + head_index) % size_limit;

		del_node	= CFRlist + calc_index;

		if(del_node->value != NULL)
		{
			switch(del_node->value->status)
			{
			case CHANGE_FOR_READER_STATUS_KIND_ACKNOWLEDGED:
				cur_node	= &CFR_acked;
				break;
			case CHANGE_FOR_READER_STATUS_KIND_REQUESTED:
				cur_node	= &CFR_requested;
				break;
			case CHANGE_FOR_READER_STATUS_KIND_UNACKNOWLEDGED:
				cur_node	= &CFR_unacked;
				break;
			case CHANGE_FOR_READER_STATUS_KIND_UNDERWAY:
				cur_node	= &CFR_underway;
				break;
			case CHANGE_FOR_READER_STATUS_KIND_UNSENT:
				cur_node	= &CFR_unsent;
				break;
			default:
				//mutex_unlock(&mutex);
				return false;
			}

			if(((*cur_node)->prev	== *cur_node))
			{
				*cur_node	= NULL;
			}
			else if((*cur_node) == del_node)
			{
				*cur_node	= del_node->rear;
				
				del_node->prev->rear	= del_node->rear;
				del_node->rear->prev	= del_node->prev;
			}
			else
			{
				del_node->prev->rear	= del_node->rear;
				del_node->rear->prev	= del_node->prev;
			}

			size--;
		}

		memset(del_node, 0, sizeof(CFRnode));

		//mutex_unlock(&mutex);
		return true;
	}

	bool				ChangeForReaderList::removeBySequenceNumber(SequenceNumber_t seq_num)
	{
		SequenceNumber_t	calc_seq;
		unsigned long		calc_index;
		CFRnode*			del_node	= NULL;
		CFRnode**			cur_node	= NULL;

		mutex_lock(&mutex);

		calc_seq	= seq_num - base_number;
		calc_index	= calc_seq.low;

		if(calc_index >= size)
		{
			mutex_unlock(&mutex);
			return false;
		}

		calc_index	= (calc_index + head_index) % size_limit;

		del_node	= CFRlist + calc_index;
		if(del_node->value->cache_change->sequence_number == seq_num)
		{
			
			switch(del_node->value->status)
			{
			case CHANGE_FOR_READER_STATUS_KIND_ACKNOWLEDGED:
				cur_node	= &CFR_acked;
				break;;
			case CHANGE_FOR_READER_STATUS_KIND_REQUESTED:
				cur_node	= &CFR_requested;
				break;
			case CHANGE_FOR_READER_STATUS_KIND_UNACKNOWLEDGED:
				cur_node	= &CFR_unacked;
				break;
			case CHANGE_FOR_READER_STATUS_KIND_UNDERWAY:
				cur_node	= &CFR_underway;
				break;
			case CHANGE_FOR_READER_STATUS_KIND_UNSENT:
				cur_node	= &CFR_unsent;
				break;
			default:
				mutex_unlock(&mutex);
				return false;
			}

			if(((*cur_node)->prev	== *cur_node))
			{
				*cur_node	= NULL;
			}
			else if((*cur_node) == del_node)
			{
				*cur_node	= del_node->rear;
				
				del_node->prev->rear	= del_node->rear;
				del_node->rear->prev	= del_node->prev;
			}
			else
			{
				del_node->prev->rear	= del_node->rear;
				del_node->rear->prev	= del_node->prev;
			}

			size--;

			memset(del_node, 0, sizeof(CFRnode));
		}
		else
		{
			mutex_unlock(&mutex);
			return false;
		}

		mutex_unlock(&mutex);
		return true;
	}

	void				ChangeForReaderList::refresh()
	{
		unsigned long	calc_index		= head_index;

		mutex_lock(&mutex);
		if(size > 0)
		{
			while(CFRlist[calc_index].value	== NULL)
			{
				memset(CFRlist + calc_index, 0, sizeof(CFRnode));
				calc_index	= (calc_index + 1) % size_limit;
			}
			head_index	= calc_index;
			base_number	= CFRlist[head_index].value->cache_change->sequence_number;
		}
		mutex_unlock(&mutex);
	}
	
	bool				ChangeForReaderList::refreshByStatus()
	{
		bool	result		= false;
		int		i			= 0;
		int		index		= 0;
		int		max_count	= 0;
		ChangeForReader*	cfr	= NULL;

		if(size > 0)
		{
			max_count	= size;

			mutex_lock(&mutex);
			for(i=0; i<max_count ; i++)
			{				
				cfr		= this->getValueByIndex(i);

				if(cfr)
				{
					if(cfr->status == CHANGE_FOR_READER_STATUS_KIND_ACKNOWLEDGED)
					{
						this->removeByIndex(i);
						delete(cfr);
						//CFWlist[index].value	= NULL;
						//size--;

						result	= true;
					}
					else if(result)
					{
						base_number	+= SequenceNumber_t(0, i);
						head_index	= index;
						break;
					}
					else
					{
						break;
					}
				}
				else if(i>0)
				{
					//printf("ERROR: ChangeFromWriterList::refreshByStatus()\n");
					//result	= false;
					base_number	+= SequenceNumber_t(0, i);
					head_index	= index;
					break;
				}
				else
				{
					return false;
				}
			}

			if(result && i == max_count)
			{
				base_number	+= SequenceNumber_t(0, i);
				head_index	= index;
			}
			mutex_unlock(&mutex);
		}
		else
		{
			result	= true;
		}

		return result;
	}

	/*bool				ChangeForReaderList::refreshByStatus()
	{
		bool	result		= false;
		int		i			= 0;
		int		index		= 0;
		int		max_count	= 0;

		if(size > 0)
		{
			max_count	= size;

			mutex_lock(&mutex);
			for(i=0; i<max_count ; i++)
			{
				index	= (head_index + i) % size_limit;

				if(CFRlist[index].value)
				{
					if(CFRlist[index].value->status == CHANGE_FOR_READER_STATUS_KIND_ACKNOWLEDGED)
					{
						delete(CFRlist[index].value);
						CFRlist[index].value	= NULL;
						size--;
						
						base_number	+= SequenceNumber_t(0, 1);

						result	= true;
					}
					else if(result)
					{
						head_index	= index;
						break;
					}
					else
					{
						break;
					}
				}
				else
				{
					printf("ERROR: ChangeForReaderList::refreshByStatus()\n");
					result	= false;
				}
			}
			mutex_unlock(&mutex);
		}
		else
		{
			result	= true;
		}

		return result;
	}*/
}
