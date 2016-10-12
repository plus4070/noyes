#ifndef INCLUDE_UTILITY_SHAREDPTRNODE_H_
#define INCLUDE_UTILITY_SHAREDPTRNODE_H_

#include "PlatformOrientedInterface.h"

namespace CNU_DDS
{
	template<class TempClass>
	class SharedPtrNode
	{
	public:
		SHARED_PTR_TYPE_NATIVE<TempClass>	value;
		SharedPtrNode<TempClass>*	front;
		SharedPtrNode<TempClass>*	rear;
	};
}

#endif
