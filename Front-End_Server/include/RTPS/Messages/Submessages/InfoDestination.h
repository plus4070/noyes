#ifndef INFODESTINATION_H_
#define INFODESTINATION_H_

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
	class CNU_DDS_DLL_API InfoDestination : public Submessage
	{
	public:
		GuidPrefix	guid_prefix;

	public:
		InfoDestination();
		~InfoDestination();

		ByteStream*	Serialize();
		void		Deserialize(unsigned char* data);
	};
}

#ifdef __cplusplus
}
#endif
#endif
