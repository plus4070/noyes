#ifndef FRAGMENTNUMBER_T_H_
#define FRAGMENTNUMBER_T_H_

#include "../../../Utility/DoublyLinkedList.h"

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
	class CNU_DDS_DLL_API FragmentNumber_t
	{
	public:
		unsigned long	value;

		FragmentNumber_t	operator	+ (const FragmentNumber_t& arg);
		FragmentNumber_t	operator	- (const FragmentNumber_t& arg);
		void				operator	= (const FragmentNumber_t& arg);
	};

	typedef DoublyLinkedList<FragmentNumber_t>	FragmentNumberSeq;
}

#ifdef __cplusplus
}
#endif
#endif
