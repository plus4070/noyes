#ifndef INCLUDE_UTILITY_SHAREDPTRLIST_H_
#define INCLUDE_UTILITY_SHAREDPTRLIST_H_

#include "PlatformOrientedInterface.h"
#include "SharedPtrNode.h"

namespace CNU_DDS
{
	#define	SHARED_PTR_LIST_SIZE_MAX	0xFFFFFFFF

	template<class TempClass>
	class SharedPtrList
	{
	private:
		unsigned long				limit;
		unsigned long				size;
		SharedPtrNode<TempClass>	head;
		MUTEX_TYPE_NATIVE			mutex;

	private:
		void	_init(void);

	public:
		SharedPtrList(void);
		SharedPtrList(unsigned long max_size);
		~SharedPtrList(void);

		int					listLock(void);
		int					listUnlock(void);

		bool				insertInFront(SHARED_PTR_TYPE_NATIVE<TempClass> insert_value);
		bool				insertInRear(SHARED_PTR_TYPE_NATIVE<TempClass> insert_value);
		bool				insertAscendingInFront(SHARED_PTR_TYPE_NATIVE<TempClass> insert_value);
		bool				insertAscendingInRear(SHARED_PTR_TYPE_NATIVE<TempClass> insert_value);
		bool				insertDescendingInFront(SHARED_PTR_TYPE_NATIVE<TempClass> insert_value);
		bool				insertDescendingInRear(SHARED_PTR_TYPE_NATIVE<TempClass> insert_value);

		SHARED_PTR_TYPE_NATIVE<TempClass>	popFirst(void);
		SHARED_PTR_TYPE_NATIVE<TempClass>	popByIndex(unsigned long index);
		SHARED_PTR_TYPE_NATIVE<TempClass>	popLast(void);

		SHARED_PTR_TYPE_NATIVE<TempClass>	getFirst(void);
		SHARED_PTR_TYPE_NATIVE<TempClass>	getLast(void);

		SharedPtrNode<TempClass>*			getNodeByIndex(unsigned long index);
		void								cutNode(SharedPtrNode<TempClass>* cutting_node);

		bool				setLimitSize(unsigned long limit_size);
		unsigned long		getSize(void);

		void				deleteAll(void);
		void				deleteAllNode(void);

		void				doEach(unsigned long (*func)(SHARED_PTR_TYPE_NATIVE<TempClass> cur_value, unsigned long prev_ret_value));
	};



	template<class TempClass>
	SharedPtrList<TempClass>::SharedPtrList(void)
	{
		limit	= SHARED_PTR_LIST_SIZE_MAX;
		_init();
	}



	template<class TempClass>
	SharedPtrList<TempClass>::SharedPtrList(unsigned long max_size)
	{
		limit	= max_size;
		_init();
	}



	template<class TempClass>
	SharedPtrList<TempClass>::~SharedPtrList(void)
	{
		deleteAll();
		destroy_mutex(&mutex);
	}

	template<class TempClass>
	int					SharedPtrList<TempClass>::listLock(void)
	{
		return mutex_lock(&mutex);
	}

	template<class TempClass>
	int					SharedPtrList<TempClass>::listUnlock(void)
	{
		return mutex_unlock(&mutex);
	}

	template<class TempClass>
	void			SharedPtrList<TempClass>::_init(void)
	{
		int	err_num	= 0;

		size		= 0;
		head.front	= &head;
		head.rear	= &head;
		head.value	= 0;

		err_num	= create_mutex(&mutex);
		if(err_num)
		{
			exit(0);
		}
	}

	template<class TempClass>
	bool			SharedPtrList<TempClass>::insertInFront(SHARED_PTR_TYPE_NATIVE<TempClass> insert_value)
	{
		if(size >= limit)
		{
			return false;
		}
		else
		{
			SharedPtrNode<TempClass>*	newNode	= new SharedPtrNode<TempClass>();

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
	bool			SharedPtrList<TempClass>::insertInRear(SHARED_PTR_TYPE_NATIVE<TempClass> insert_value)
	{
		if(size >= limit)
		{
			return false;
		}
		else
		{
			SharedPtrNode<TempClass>*	newNode	= new SharedPtrNode<TempClass>();

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
	bool	SharedPtrList<TempClass>::insertDescendingInFront(SHARED_PTR_TYPE_NATIVE<TempClass> insert_value)
	{
		int 				i			= 0;
		SharedPtrNode<TempClass>*	cur_node	= NULL;
		SharedPtrNode<TempClass>*	new_node	= NULL;

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

			new_node		= new SharedPtrNode<TempClass>();
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
	bool SharedPtrList<TempClass>::insertDescendingInRear(SHARED_PTR_TYPE_NATIVE<TempClass> insert_value)
	{
		int 				i			= 0;
		SharedPtrNode<TempClass>*	cur_node	= NULL;
		SharedPtrNode<TempClass>*	new_node	= NULL;

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

			new_node		= new SharedPtrNode<TempClass>();
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
	bool	SharedPtrList<TempClass>::insertAscendingInFront(SHARED_PTR_TYPE_NATIVE<TempClass> insert_value)
	{
		int 				i			= 0;
		SharedPtrNode<TempClass>*	cur_node	= NULL;
		SharedPtrNode<TempClass>*	new_node	= NULL;

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

			new_node		= new SharedPtrNode<TempClass>();
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
	bool	SharedPtrList<TempClass>::insertAscendingInRear(SHARED_PTR_TYPE_NATIVE<TempClass> insert_value)
	{
		int 				i			= 0;
		SharedPtrNode<TempClass>*	cur_node	= NULL;
		SharedPtrNode<TempClass>*	new_node	= NULL;

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

			new_node		= new SharedPtrNode<TempClass>();
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
	SHARED_PTR_TYPE_NATIVE<TempClass>	SharedPtrList<TempClass>::popFirst(void)
	{
		SharedPtrNode<TempClass>*			cur_node	= NULL;
		SHARED_PTR_TYPE_NATIVE<TempClass>	cur_value	= NULL;

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
	SHARED_PTR_TYPE_NATIVE<TempClass>	SharedPtrList<TempClass>::popByIndex(unsigned long index)
	{
		int									i			= 0;
		SharedPtrNode<TempClass>*			cur_node	= NULL;
		SHARED_PTR_TYPE_NATIVE<TempClass>	cur_value	= NULL;

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
	SHARED_PTR_TYPE_NATIVE<TempClass>	SharedPtrList<TempClass>::popLast(void)
	{
		SharedPtrNode<TempClass>*			cur_node	= NULL;
		SHARED_PTR_TYPE_NATIVE<TempClass>	cur_value	= NULL;

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
	SHARED_PTR_TYPE_NATIVE<TempClass>	SharedPtrList<TempClass>::getFirst(void)
	{
		SHARED_PTR_TYPE_NATIVE<TempClass>			cur_value	= NULL;

		if(size > 0)
		{
			mutex_lock(&mutex);

			cur_value	= head.rear->value;

			mutex_unlock(&mutex);
		}

		return cur_value;
	}



	template<class TempClass>
	SHARED_PTR_TYPE_NATIVE<TempClass>	SharedPtrList<TempClass>::getLast(void)
	{
		SHARED_PTR_TYPE_NATIVE<TempClass>			cur_value	= NULL;

		if(size > 0)
		{
			mutex_lock(&mutex);

			cur_value	= head.front->value;

			mutex_unlock(&mutex);
		}

		return cur_value;
	}



	template<class TempClass>
	SharedPtrNode<TempClass>*	SharedPtrList<TempClass>::getNodeByIndex(unsigned long index)
	{
		int					i			= 0;
		SharedPtrNode<TempClass>*	cur_node	= NULL;

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
	void				SharedPtrList<TempClass>::cutNode(SharedPtrNode<TempClass>* cutting_node)
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
	bool				SharedPtrList<TempClass>::setLimitSize(unsigned long limit_size)
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
	unsigned long		SharedPtrList<TempClass>::getSize(void)
	{
		return size;
	}



	template<class TempClass>
	void				SharedPtrList<TempClass>::deleteAll(void)
	{
		SHARED_PTR_TYPE_NATIVE<TempClass>	delete_value;

		mutex_lock(&mutex);

		while(size>0)
		{
			mutex_unlock(&mutex);
			delete_value	= popFirst();
			mutex_lock(&mutex);
			if(delete_value)
			{
				delete_value.reset();
			}
		}

		mutex_unlock(&mutex);
	}

	template<class TempClass>
	void				SharedPtrList<TempClass>::deleteAllNode(void)
	{
		SHARED_PTR_TYPE_NATIVE<TempClass>	delete_value;

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
	void				SharedPtrList<TempClass>::doEach(unsigned long (*func)(SHARED_PTR_TYPE_NATIVE<TempClass> cur_value, unsigned long prev_ret_value))
	{
		unsigned long				index		= size;
		unsigned long				ret_value	= 0;
		SharedPtrNode<TempClass>*	cur_node	= this->head.rear;

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

#endif
