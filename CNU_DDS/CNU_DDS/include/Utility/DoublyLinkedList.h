/*
 * DoublyLinkedList.h
 *
 *  Created on: 2013. 3. 22.
 *      Author: MDSL-Gundoll
 */

#ifndef DOUBLYLINKEDLIST_H_
#define DOUBLYLINKEDLIST_H_

#include "PlatformOrientedInterface.h"
#include "Node.h"

namespace CNU_DDS
{
	#define	DOUBLY_LINKED_LIST_SIZE_MAX	0xFFFFFFFF

	template<class TempClass>
	class DoublyLinkedList
	{
	private:
		unsigned long		limit;
		unsigned long		size;
		Node<TempClass>		head;
		MUTEX_TYPE_NATIVE	mutex;

	private:
		void	_init(void);

	public:
		DoublyLinkedList(void);
		DoublyLinkedList(unsigned long max_size);
		~DoublyLinkedList(void);

		int					listLock(void);
		int					listUnlock(void);

		bool				insertInFront(TempClass* insert_value);
		bool				insertInRear(TempClass* insert_value);
		bool				insertAscendingInFront(TempClass* insert_value);
		bool				insertAscendingInRear(TempClass* insert_value);
		bool				insertDescendingInFront(TempClass* insert_value);
		bool				insertDescendingInRear(TempClass* insert_value);

		TempClass*			popFirst(void);
		TempClass*			popByIndex(unsigned long index);
		TempClass*			popLast(void);

		TempClass*			getFirst(void);
		TempClass*			getLast(void);

		Node<TempClass>*	getNodeByIndex(unsigned long index);
		Node<TempClass>*	getNodeByAscendingValue(TempClass* insert_value);
		Node<TempClass>*	getNodeByValue(TempClass* insert_value);
		void				cutNode(Node<TempClass>* cutting_node);
		void				relocateAll(DoublyLinkedList<TempClass>* target);

		bool				setLimitSize(unsigned long limit_size);
		unsigned long		getSize(void);

		void				deleteAll(void);
		void				deleteAllNode(void);

		void				doEach(unsigned long (*func)(TempClass* cur_value, unsigned long prev_ret_value));
	};



	template<class TempClass>
	DoublyLinkedList<TempClass>::DoublyLinkedList(void)
	{
		limit	= DOUBLY_LINKED_LIST_SIZE_MAX;
		_init();
	}



	template<class TempClass>
	DoublyLinkedList<TempClass>::DoublyLinkedList(unsigned long max_size)
	{
		limit	= max_size;
		_init();
	}



	template<class TempClass>
	DoublyLinkedList<TempClass>::~DoublyLinkedList(void)
	{
		deleteAll();
		destroy_mutex(&mutex);
	}

	template<class TempClass>
	int					DoublyLinkedList<TempClass>::listLock(void)
	{
		return mutex_lock(&mutex);
	}

	template<class TempClass>
	int					DoublyLinkedList<TempClass>::listUnlock(void)
	{
		return mutex_unlock(&mutex);
	}

	template<class TempClass>
	void			DoublyLinkedList<TempClass>::_init(void)
	{
		int	err_num	= 0;

		size		= 0;
		head.front	= &head;
		head.rear	= &head;

		err_num	= create_mutex(&mutex);
		if(err_num)
		{
			exit(0);
		}
	}

	template<class TempClass>
	bool			DoublyLinkedList<TempClass>::insertInFront(TempClass* insert_value)
	{
		if(size >= limit)
		{
			return false;
		}
		else
		{
			Node<TempClass>*	newNode	= new Node<TempClass>();

			mutex_lock(&mutex);

			newNode->front	= &head;
			newNode->rear	= head.rear;
			newNode->value	= insert_value;

			newNode->rear->front	= newNode;

			head.rear		= newNode;

			size++;

			mutex_unlock(&mutex);

			return true;
		}
	}



	template<class TempClass>
	bool			DoublyLinkedList<TempClass>::insertInRear(TempClass* insert_value)
	{
		if(size >= limit)
		{
			return false;
		}
		else
		{
			Node<TempClass>*	newNode	= new Node<TempClass>();

			mutex_lock(&mutex);

			newNode->front	= head.front;
			newNode->rear	= &head;
			newNode->value	= insert_value;

			newNode->front->rear	= newNode;

			head.front		= newNode;

			size++;

			mutex_unlock(&mutex);

			return true;
		}
	}



	template<class TempClass>
	bool	DoublyLinkedList<TempClass>::insertDescendingInFront(TempClass* insert_value)
	//bool	DoublyLinkedList<TempClass>::insertAscendingInFront(TempClass* insert_value)
	{
		int 				i			= 0;
		Node<TempClass>*	cur_node	= NULL;
		Node<TempClass>*	new_node	= NULL;

		if(size >= limit)
		{
			return false;
		}
		else
		{
			mutex_lock(&mutex);

			cur_node	= head.rear;
			i			= 1;

			while(i<=size)
			{
				if(*(cur_node->value) >= *insert_value)
				{
					break;
				}

				i++;
				cur_node	= cur_node->rear;
			}

			new_node		= new Node<TempClass>();
			new_node->value	= insert_value;
			new_node->rear	= cur_node;
			new_node->front	= cur_node->front;

			new_node->front->rear	= new_node;
			cur_node->front			= new_node;

			size++;

			mutex_unlock(&mutex);

			return true;
		}
	}



	template<class TempClass>
	bool DoublyLinkedList<TempClass>::insertDescendingInRear(TempClass* insert_value)
	//bool DoublyLinkedList<TempClass>::insertAscendingInRear(TempClass* insert_value)
	{
		int 				i			= 0;
		Node<TempClass>*	cur_node	= NULL;
		Node<TempClass>*	new_node	= NULL;

		if(size >= limit)
		{
			return false;
		}
		else
		{
			mutex_lock(&mutex);

			cur_node	= head.front;
			i			= 1;

			while(i<=size)
			{
				if(*(cur_node->value) >= *insert_value)
				{
					break;
				}

				i++;
				cur_node	= cur_node->front;
			}

			new_node		= new Node<TempClass>();
			new_node->value	= insert_value;
			new_node->front	= cur_node;
			new_node->rear	= cur_node->rear;

			new_node->rear->front	= new_node;
			cur_node->rear			= new_node;

			size++;

			mutex_unlock(&mutex);

			return true;
		}
	}



	template<class TempClass>
	bool	DoublyLinkedList<TempClass>::insertAscendingInFront(TempClass* insert_value)
	{
		int 				i			= 0;
		Node<TempClass>*	cur_node	= NULL;
		Node<TempClass>*	new_node	= NULL;

		if(size >= limit)
		{
			return false;
		}
		else
		{
			mutex_lock(&mutex);

			cur_node	= head.rear;
			i			= 1;

			while(i<=size)
			{
				if(*(cur_node->value) <= *insert_value)
				{
					break;
				}

				i++;
				cur_node	= cur_node->rear;
			}

			new_node		= new Node<TempClass>();
			new_node->value	= insert_value;
			new_node->rear	= cur_node;
			new_node->front	= cur_node->front;

			new_node->front->rear	= new_node;
			cur_node->front			= new_node;

			size++;

			mutex_unlock(&mutex);

			return true;
		}
	}



	template<class TempClass>
	bool	DoublyLinkedList<TempClass>::insertAscendingInRear(TempClass* insert_value)
	{
		unsigned int		i			= 0;
		Node<TempClass>*	cur_node	= NULL;
		Node<TempClass>*	new_node	= NULL;

		if(size >= limit)
		{
			return false;
		}
		else
		{
			mutex_lock(&mutex);

			cur_node	= head.front;
			i			= 1;

			while(i <= size)
			{
				if(*(cur_node->value) <= *insert_value)
				{
					break;
				}

				i++;
				cur_node	= cur_node->front;
			}

			new_node		= new Node<TempClass>();
			new_node->value	= insert_value;
			new_node->front	= cur_node;
			new_node->rear	= cur_node->rear;

			new_node->rear->front	= new_node;
			cur_node->rear			= new_node;

			size++;

			mutex_unlock(&mutex);

			return true;
		}
	}



	template<class TempClass>
	TempClass*	DoublyLinkedList<TempClass>::popFirst(void)
	{
		Node<TempClass>*	cur_node	= NULL;
		TempClass*			cur_value	= NULL;

		if(size > 0)
		{
			mutex_lock(&mutex);

			cur_node	= head.rear;

			cur_node->rear->front	= cur_node->front;
			cur_node->front->rear	= cur_node->rear;

			cur_value	= cur_node->value;

			delete(cur_node);

			size--;

			mutex_unlock(&mutex);
		}

		return cur_value;
	}



	template<class TempClass>
	TempClass*	DoublyLinkedList<TempClass>::popByIndex(unsigned long index)
	{
		int					i			= 0;
		Node<TempClass>*	cur_node	= NULL;
		TempClass*			cur_value	= NULL;

		if((size > 0) && (size >= index))
		{
			mutex_lock(&mutex);

			cur_node	= head.rear;
			i			= 1;

			while((i <= size) && (i != index))
			{
				cur_node	= cur_node->rear;
				i++;
			}

			cur_node->rear->front	= cur_node->front;
			cur_node->front->rear	= cur_node->rear;

			cur_value	= cur_node->value;

			delete(cur_node);

			size--;

			mutex_unlock(&mutex);
		}

		return cur_value;
	}



	template<class TempClass>
	TempClass*	DoublyLinkedList<TempClass>::popLast(void)
	{
		Node<TempClass>*	cur_node	= NULL;
		TempClass*			cur_value	= NULL;

		if(size > 0)
		{
			mutex_lock(&mutex);

			cur_node	= head.front;

			cur_node->front->rear	= cur_node->rear;
			cur_node->rear->front	= cur_node->front;

			cur_value	= cur_node->value;

			delete(cur_node);

			size--;

			mutex_unlock(&mutex);
		}

		return cur_value;
	}



	template<class TempClass>
	TempClass*	DoublyLinkedList<TempClass>::getFirst(void)
	{
		TempClass*			cur_value	= NULL;

		if(size > 0)
		{
			mutex_lock(&mutex);

			cur_value	= head.rear->value;

			mutex_unlock(&mutex);
		}

		return cur_value;
	}



	template<class TempClass>
	TempClass*	DoublyLinkedList<TempClass>::getLast(void)
	{
		TempClass*			cur_value	= NULL;

		if(size > 0)
		{
			mutex_lock(&mutex);

			cur_value	= head.front->value;

			mutex_unlock(&mutex);
		}

		return cur_value;
	}



	template<class TempClass>
	Node<TempClass>*	DoublyLinkedList<TempClass>::getNodeByIndex(unsigned long index)
	{
		int					i			= 0;
		Node<TempClass>*	cur_node	= NULL;

		if((size > 0) && (size >= index))
		{
			mutex_lock(&mutex);

			cur_node	= head.rear;
			i			= 1;

			while((i <= size) && (i != index))
			{
				cur_node	= cur_node->rear;
				i++;
			}

			mutex_unlock(&mutex);
		}

		return cur_node;
	}


	template<class TempClass>
	Node<TempClass>*	DoublyLinkedList<TempClass>::getNodeByAscendingValue(TempClass* insert_value)
	{
		Node<TempClass>*	cur_node	= NULL;

		if((size > 0))
		{
			mutex_lock(&mutex);

			cur_node	= head.rear;

			while((cur_node != head) && (*cur_node->value < *insert_value))
			{
				cur_node	= cur_node->rear;
			}

			if(*cur_node->value == *insert_value)
			{
			}
			else
			{
				cur_node	= NULL;
			}

			mutex_unlock(&mutex);
		}

		return cur_node;
	}


	template<class TempClass>
	Node<TempClass>*	DoublyLinkedList<TempClass>::getNodeByValue(TempClass* insert_value)
	{
		Node<TempClass>*	cur_node	= NULL;

		if((size > 0))
		{
			mutex_lock(&mutex);

			cur_node	= head.rear;

			while((cur_node != head) && (!(*cur_node->value == *insert_value)))
			{
				cur_node	= cur_node->rear;
			}

			if(*cur_node->value == *insert_value)
			{
			}
			else
			{
				cur_node	= NULL;
			}

			mutex_unlock(&mutex);
		}

		return cur_node;
	}


	template<class TempClass>
	void				DoublyLinkedList<TempClass>::cutNode(Node<TempClass>* cutting_node)
	{
		if(cutting_node == NULL || cutting_node->front == NULL || cutting_node->rear == NULL)
		{
			return;
		}

		mutex_lock(&mutex);

		cutting_node->front->rear	= cutting_node->rear;
		cutting_node->rear->front	= cutting_node->front;

		cutting_node->front	= NULL;
		cutting_node->rear	= NULL;

		size--;

		mutex_unlock(&mutex);
	}



	template<class TempClass>
	void				DoublyLinkedList<TempClass>::relocateAll(DoublyLinkedList<TempClass>* target)
	{
		unsigned long		old_size	= 0;
		Node<TempClass>*	old_first	= NULL;
		Node<TempClass>*	old_last	= NULL;
		Node<TempClass>*	new_last	= NULL;

		if(this->size > 0)
		{
			this->listLock();

			old_size	= this->size;
			old_first	= this->head.rear;
			old_last	= this->head.front;

			this->head.front	= &this->head;
			this->head.rear		= &this->head;
			this->size			= 0;

			this->listUnlock();


			target->listLock();

			new_last	= target->head.front;

			old_first->front	= new_last;
			old_last->rear		= &target->head;

			new_last->rear		= old_first;

			target->head.front	= old_last;
			target->size		+= old_size;

			target->listUnlock();
		}
	}



	template<class TempClass>
	bool				DoublyLinkedList<TempClass>::setLimitSize(unsigned long limit_size)
	{
		if(limit_size < size)
		{
			return false;
		}
		else
		{
			limit	= limit_size;
			return true;
		}
	}

	template<class TempClass>
	unsigned long		DoublyLinkedList<TempClass>::getSize(void)
	{
		return size;
	}



	template<class TempClass>
	void				DoublyLinkedList<TempClass>::deleteAll(void)
	{
		TempClass*	delete_value;

		mutex_lock(&mutex);

		while(size>0)
		{
			mutex_unlock(&mutex);
			delete_value	= popFirst();
			mutex_lock(&mutex);
			if(delete_value)
			{
				delete(delete_value);
			}
		}

		mutex_unlock(&mutex);
	}

	template<class TempClass>
	void				DoublyLinkedList<TempClass>::deleteAllNode(void)
	{
		TempClass*	delete_value;

		mutex_lock(&mutex);

		while(size>0)
		{
			mutex_unlock(&mutex);
			delete_value	= popFirst();
			mutex_lock(&mutex);
		}

		mutex_unlock(&mutex);
	}

	template<class TempClass>
	void				DoublyLinkedList<TempClass>::doEach(unsigned long (*func)(TempClass* cur_value, unsigned long prev_ret_value))
	{
		unsigned long		index		= size;
		unsigned long		ret_value	= 0;
		Node<TempClass>*	cur_node	= this->head.rear;

		mutex_lock(&mutex);

		while(size)
		{
			ret_value	= func(cur_node->value, ret_value);

			cur_node	= cur_node->rear;
			size--;

			if(ret_value == -1)
			{
				break;
			}
		}

		mutex_unlock(&mutex);
	}
}

#endif /* DOUBLYLINKEDLIST_H_ */
