#ifndef NODE_H_
#define NODE_H_

namespace CNU_DDS
{
	template<class TempClass>
	class Node
	{
	public:
		TempClass*			value;
		Node<TempClass>*	front;
		Node<TempClass>*	rear;
	};
}

#endif /* NODE_H_ */
