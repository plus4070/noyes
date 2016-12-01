#ifndef DATAFRAG_H_
#define DATAFRAG_H_

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
	class CNU_DDS_DLL_API DataFrag : public Submessage
	{
	public:
		unsigned char	extraFlags[2];
		EntityId		reader_id;
		EntityId		writer_id;
		SequenceNumber	writer_sn;
		FragmentNumber	fragment_starting_num;
		unsigned short	fragments_in_submessage;
		unsigned long	data_size;
		unsigned short	fragement_size;
		ParameterList	inline_qos;
		SerializedData	serialized_payload;

	public:
		DataFrag();
		~DataFrag();

		ByteStream*	Serialize();
		void		Deserialize(unsigned char* data);
	};
}

#ifdef __cplusplus
}
#endif
#endif
