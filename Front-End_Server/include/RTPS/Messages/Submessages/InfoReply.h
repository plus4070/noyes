#ifndef INFOREPLY_H_
#define INFOREPLY_H_

#include "../Submessage.h"

#include "../SubmessageElements/InclusionsOfSubmessageElement.h"

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
	class CNU_DDS_DLL_API InfoReply : public Submessage
	{
	public:
		LocatorList	unicast_locator_list;
		LocatorList	multicast_locator_list;

	public:
		InfoReply();
		~InfoReply();

		ByteStream*	Serialize();
		void		Deserialize(unsigned char* data);
	};
}

#ifdef __cplusplus
}
#endif
#endif
