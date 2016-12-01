#ifndef GAP_H_
#define GAP_H_

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
	class CNU_DDS_DLL_API Gap : public Submessage
	{
	public:
		EntityId			reader_id;
		EntityId			writer_id;
		SequenceNumber		gap_start;
		SequenceNumberSet	gap_list;

	public:
		Gap();
		~Gap();

		ByteStream*	Serialize();
		void		Deserialize(unsigned char* data);
	};
}

#ifdef __cplusplus
}
#endif
#endif
