#ifndef INCLUDE_UTILITY_NETWORK_SIMPLELINKEDLIST_H_
#define INCLUDE_UTILITY_NETWORK_SIMPLELINKEDLIST_H_

#ifdef CNU_DDS_DLL
#define CNU_DDS_DLL_API	__declspec(dllexport)	
#else
#define CNU_DDS_DLL_API	__declspec(dllimport)
#endif
#ifdef __cplusplus
extern "C" {
#endif
namespace CNU_DDS
{
	class CNU_DDS_DLL_API SimpleLinkNode
	{
	public:
		SimpleLinkNode*	rear;
		unsigned long	value;
	};

	class CNU_DDS_DLL_API SimpleLinkedList
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

#ifdef __cplusplus
}
#endif
#endif
