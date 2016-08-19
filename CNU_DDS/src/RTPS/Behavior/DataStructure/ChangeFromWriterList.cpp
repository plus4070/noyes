#include "../../../../include/RTPS/Behavior/DataStructure/ChangeFromWriterList.h"
#include <stdio.h>

namespace CNU_DDS
{
	ChangeFromWriterList::ChangeFromWriterList(unsigned long limit)
	{
		CFWlist	= new CFWnode[limit];
		memset(CFWlist, 0, sizeof(CFWnode)*limit);

		CFW_lost		= NULL;
		CFW_missing		= NULL;
		CFW_received	= NULL;
		CFW_unknown		= NULL;

		base_number	= SequenceNumber_t(0,1);
		size		= 0;
		head_index	= 0;
		size_limit	= limit;

		create_mutex(&mutex);
	}

	ChangeFromWriterList::~ChangeFromWriterList()
	{
		delete(CFWlist);
		destroy_mutex(&mutex);
	}

	bool				ChangeFromWriterList::insertValue(ChangeFromWriter* cfw)
	{
		CFWnode**			cur_node	= NULL;
		CFWnode*			new_node	= NULL;
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
			//base_number	= cfw->cache_change->sequence_number;
		}

		calc_seq	= cfw->cache_change->sequence_number - base_number;
		calc_index	= calc_seq.low;

		if( (calc_index > size_limit) || (calc_seq.high > 0) )
		{
			mutex_unlock(&mutex);
			return false;
		}

		switch(cfw->status)
		{
		case CHANGE_FROM_WRITER_STATUS_KIND_LOST:
			cur_node	= &CFW_lost;
			break;
		case CHANGE_FROM_WRITER_STATUS_KIND_MISSING:
			cur_node	= &CFW_missing;
			break;
		case CHANGE_FROM_WRITER_STATUS_KIND_RECEIVED:
			cur_node	= &CFW_received;
			break;
		case CHANGE_FROM_WRITER_STATUS_KIND_UNKNOWN:
			cur_node	= &CFW_unknown;
			break;
		default:
			mutex_unlock(&mutex);
			return false;
		}

		new_node	= CFWlist + ((head_index + calc_index) % size_limit);

		if((new_node->value != NULL))
		{
			mutex_unlock(&mutex);
			return false;
		}

		size++;
		new_node->value	= cfw;

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

	ChangeFromWriter*	ChangeFromWriterList::getValueByIndex(unsigned long index)
	{
		unsigned long	calc_index;

		//mutex_lock(&mutex);

		if(index > size_limit)
		{
			//mutex_unlock(&mutex);
			return NULL;
		}
		else
		{
			calc_index	= (head_index + index) % size_limit;
		}

		//mutex_unlock(&mutex);
		return CFWlist[calc_index].value;
	}

	ChangeFromWriter*	ChangeFromWriterList::getValueBySequenceNumber(SequenceNumber_t seq_num)
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
		return CFWlist[calc_index].value;
	}

	ChangeFromWriter*	ChangeFromWriterList::getFirstValueByStatus(ChangeFromWriterStatusKind kind)
	{
		CFWnode**	cur_node;

		mutex_lock(&mutex);

		switch(kind)
		{
		case CHANGE_FROM_WRITER_STATUS_KIND_LOST:
			cur_node	= &CFW_lost;
			break;
		case CHANGE_FROM_WRITER_STATUS_KIND_MISSING:
			cur_node	= &CFW_missing;
			break;
		case CHANGE_FROM_WRITER_STATUS_KIND_RECEIVED:
			cur_node	= &CFW_received;
			break;
		case CHANGE_FROM_WRITER_STATUS_KIND_UNKNOWN:
			cur_node	= &CFW_unknown;
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

	unsigned long		ChangeFromWriterList::getValidSize()
	{
		return size;
	}

	unsigned long		ChangeFromWriterList::getLimitSize()
	{
		return size_limit;
	}

	SequenceNumber_t	ChangeFromWriterList::getBaseNumber()
	{
		return base_number;
	}

	SequenceNumberSeq*	ChangeFromWriterList::getSeqNumListByStatus(ChangeFromWriterStatusKind kind)
	{
		SequenceNumberSeq*	sns			= NULL;
		CFWnode**			node_kind	= NULL;
		CFWnode*			head_node	= NULL;
		CFWnode*			cur_node	= NULL;

		mutex_lock(&mutex);

		switch(kind)
		{
		case CHANGE_FROM_WRITER_STATUS_KIND_LOST:
			node_kind	= &CFW_lost;
			break;
		case CHANGE_FROM_WRITER_STATUS_KIND_MISSING:
			node_kind	= &CFW_missing;
			break;
		case CHANGE_FROM_WRITER_STATUS_KIND_RECEIVED:
			node_kind	= &CFW_received;
			break;
		case CHANGE_FROM_WRITER_STATUS_KIND_UNKNOWN:
			node_kind	= &CFW_unknown;
			break;
		default:
			mutex_unlock(&mutex);
			return NULL;
		}

		cur_node	= *node_kind;
		head_node	= *node_kind;

		if( (*node_kind) != NULL )
		{
			sns	= new SequenceNumberSeq();

			do{
			sns->insertAscendingInRear(&cur_node->value->cache_change->sequence_number);
			cur_node	= cur_node->rear;
			}while(cur_node != head_node);
		}

		mutex_unlock(&mutex);
		return sns;
	}

	ChangeFromWriter*	ChangeFromWriterList::popFirstValueByStatus(ChangeFromWriterStatusKind kind)
	{
		CFWnode**			cur_node	= NULL;
		CFWnode*			del_node	= NULL;
		ChangeFromWriter*	ret_value	= NULL;

		mutex_lock(&mutex);

		switch(kind)
		{
		case CHANGE_FROM_WRITER_STATUS_KIND_LOST:
			cur_node	= &CFW_lost;
			break;
		case CHANGE_FROM_WRITER_STATUS_KIND_MISSING:
			cur_node	= &CFW_missing;
			break;
		case CHANGE_FROM_WRITER_STATUS_KIND_RECEIVED:
			cur_node	= &CFW_received;
			break;
		case CHANGE_FROM_WRITER_STATUS_KIND_UNKNOWN:
			cur_node	= &CFW_unknown;
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
			memset(*cur_node, 0, sizeof(CFWnode));
			*cur_node	= NULL;
		}
		else
		{
			(*cur_node)->prev->rear	= (*cur_node)->rear;
			(*cur_node)->rear->prev	= (*cur_node)->prev;
			del_node	= *cur_node;
			*cur_node	= (*cur_node)->rear;

			memset(del_node, 0, sizeof(CFWnode));
		}

		size--;
		mutex_unlock(&mutex);
		return ret_value;
	}

	void				ChangeFromWriterList::_changeStatus(CFWnode* node, ChangeFromWriterStatusKind kind)
	{
		CFWnode**	cur_node;
		CFWnode**	next_node;

		switch(node->value->status)
		{
		case CHANGE_FROM_WRITER_STATUS_KIND_LOST:
			cur_node	= &CFW_lost;
			return;	// lost state does not need to be changed
		case CHANGE_FROM_WRITER_STATUS_KIND_MISSING:
			cur_node	= &CFW_missing;
			break;
		case CHANGE_FROM_WRITER_STATUS_KIND_RECEIVED:
			cur_node	= &CFW_received;
			return;	// received state does not need to be changed
		case CHANGE_FROM_WRITER_STATUS_KIND_UNKNOWN:
			cur_node	= &CFW_unknown;
			break;
		default:
			return;
		}

		switch(kind)
		{
		case CHANGE_FROM_WRITER_STATUS_KIND_LOST:
			next_node	= &CFW_lost;
			break;
		case CHANGE_FROM_WRITER_STATUS_KIND_MISSING:
			next_node	= &CFW_missing;
			break;
		case CHANGE_FROM_WRITER_STATUS_KIND_RECEIVED:
			next_node	= &CFW_received;
			break;
		case CHANGE_FROM_WRITER_STATUS_KIND_UNKNOWN:
			next_node	= &CFW_unknown;
			break;
		default:
			return;
		}

		if(cur_node == next_node)
		{
			// do nothing
		}
		else if( (node->prev == node) && (node->rear == node) )
		{
			// if current list has just one node
			(*cur_node)	= NULL;
		}
		else if( ((*cur_node) == node) )
		{
			// if current node is a head node of doubly linked list
			(*cur_node)	= node->rear;
			(*cur_node)->prev	= node->prev;
			(*cur_node)->prev->rear	= node->rear;
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
			*next_node	= node;
			node->prev	= node;
			node->rear	= node;
		}
		else
		{
			node->prev	= (*next_node)->prev;
			node->rear	= (*next_node);

			node->prev->rear	= node;
			node->rear->prev	= node;
		}

		node->value->status	= kind;
	}

	bool				ChangeFromWriterList::changeStatusByIndex(unsigned long index, ChangeFromWriterStatusKind kind)
	{
		unsigned long	calc_index;
		CFWnode*		calc_node;

		mutex_lock(&mutex);

		if(index >= size_limit)
		{
			mutex_unlock(&mutex);
			return false;
		}

		calc_index	= (index + head_index) % size_limit;
		calc_node	= CFWlist + calc_index;

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

	bool				ChangeFromWriterList::changeStatusBySequenceNumber(SequenceNumber_t seq_num, ChangeFromWriterStatusKind kind)
	{
		SequenceNumber_t	calc_seq;
		unsigned long		calc_index;
		CFWnode*			calc_node;

		calc_seq	= seq_num - base_number;
		calc_index	= calc_seq.low;

		mutex_lock(&mutex);

		if(calc_index >= size_limit)
		{
			mutex_unlock(&mutex);
			return false;
		}

		calc_index	= (calc_index + head_index) % size_limit;
		calc_node	= CFWlist + calc_index;

		if(calc_node->value == NULL)
		{
			mutex_unlock(&mutex);
			return false;
		}
		else if(calc_node->value->status == kind)
		{
			return true;
		}

		_changeStatus(calc_node, kind);

		mutex_unlock(&mutex);
		return true;
	}

	bool				ChangeFromWriterList::removeByIndex(unsigned long index)
	{
		unsigned long	calc_index;
		CFWnode*		del_node	= NULL;
		CFWnode**		cur_node	= NULL;

		//mutex_lock(&mutex);

		if(index >= size_limit)
		{
			//mutex_unlock(&mutex);
			return false;
		}

		calc_index	= (index + head_index) % size_limit;

		del_node	= CFWlist + calc_index;

		if(del_node->value != NULL)
		{
			switch(del_node->value->status)
			{
			case CHANGE_FROM_WRITER_STATUS_KIND_LOST:
				cur_node	= &CFW_lost;
				break;
			case CHANGE_FROM_WRITER_STATUS_KIND_MISSING:
				cur_node	= &CFW_missing;
				break;
			case CHANGE_FROM_WRITER_STATUS_KIND_RECEIVED:
				cur_node	= &CFW_received;
				break;
			case CHANGE_FROM_WRITER_STATUS_KIND_UNKNOWN:
				cur_node	= &CFW_unknown;
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

		memset(del_node, 0, sizeof(CFWnode));

		//mutex_unlock(&mutex);
		return true;
	}

	bool				ChangeFromWriterList::removeBySequenceNumber(SequenceNumber_t seq_num)
	{
		SequenceNumber_t	calc_seq;
		unsigned long		calc_index;
		CFWnode*			del_node	= NULL;
		CFWnode**			cur_node	= NULL;

		calc_seq	= seq_num - base_number;
		calc_index	= calc_seq.low;

		mutex_lock(&mutex);

		if(calc_index >= size)
		{
			mutex_unlock(&mutex);
			return false;
		}

		calc_index	= (calc_index + head_index) % size_limit;

		del_node	= CFWlist + calc_index;
		if(del_node->value->cache_change->sequence_number == seq_num)
		{
			switch(del_node->value->status)
			{
			case CHANGE_FROM_WRITER_STATUS_KIND_LOST:
				cur_node	= &CFW_lost;
				break;
			case CHANGE_FROM_WRITER_STATUS_KIND_MISSING:
				cur_node	= &CFW_missing;
				break;
			case CHANGE_FROM_WRITER_STATUS_KIND_RECEIVED:
				cur_node	= &CFW_received;
				break;
			case CHANGE_FROM_WRITER_STATUS_KIND_UNKNOWN:
				cur_node	= &CFW_unknown;
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

			memset(del_node, 0, sizeof(CFWnode));
		}
		else
		{
			mutex_unlock(&mutex);
			return false;
		}

		mutex_unlock(&mutex);
		return true;
	}

	void				ChangeFromWriterList::refresh()
	{
		unsigned long	calc_index		= head_index;

		mutex_lock(&mutex);

		if(size > 0)
		{
			while(CFWlist[calc_index].value	== NULL)
			{
				memset(CFWlist + calc_index, 0, sizeof(CFWnode));
				calc_index	= (calc_index + 1) % size_limit;
			}
			head_index	= calc_index;
		}

		mutex_unlock(&mutex);
	}

	bool				ChangeFromWriterList::refreshByStatus()
	{
		bool	result		= false;
		int		i			= 0;
		int		index		= 0;
		int		max_count	= 0;
		ChangeFromWriter*	cfw	= NULL;

		if(size > 0)
		{
			max_count	= size;

			mutex_lock(&mutex);
			for(i=0; i<max_count ; i++)
			{				
				cfw		= this->getValueByIndex(i);

				if(cfw)
				{
					if(cfw->status == CHANGE_FROM_WRITER_STATUS_KIND_RECEIVED || cfw->status == CHANGE_FROM_WRITER_STATUS_KIND_LOST)
					{
						this->removeByIndex(i);
						delete(cfw);
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
}
