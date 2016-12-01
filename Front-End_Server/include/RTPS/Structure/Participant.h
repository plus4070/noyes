#ifndef PARTICIPANT_H_
#define PARTICIPANT_H_

#include "RTPSEntity.h"
#include "Endpoint.h"

#include "Type/ProtocolVersion_t.h"
#include "Type/VendorId_t.h"

#include "../Behavior/StatelessWriter.h"
#include "../Behavior/StatelessReader.h"
#include "../Discovery/ParticipantProxy.h"
#include "../Behavior/ReaderProxy.h"
#include "../Behavior/WriterProxy.h"

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
	class CNU_DDS_DLL_API Participant : public RTPSEntity
	{
	private:
		StatelessWriter*		SPDPbuiltinParticipantWriter;
		StatelessReader*		SPDPbuiltinParticipantReader;
		bool					is_created_a_pdp_message;

	public:
		ProtocolVersion_t		protocol_version;
		VendorId_t				vendor_id;
		LocatorSeq*				default_unicast_locator_list;
		LocatorSeq*				default_multicast_locator_list;
		LocatorSeq*				metatraffic_unicast_locator_list;
		LocatorSeq*				metatraffic_multicast_locator_list;
		EndpointSeq*			endpoints;
		pDomainParticipant		related_dcps_participant;
		ParticipantProxySeq*	matched_participants;

	public:
		static const unsigned long	PORT_BASE_NUMBER		= 7400;
		static const unsigned long 	DOMAIN_ID_GAIN			= 250;
		static const unsigned long	PARTICIPANT_ID_GAIN		= 2;
		static const unsigned long 	ADDITIONAL_OFFSET_D0	= 0;
		static const unsigned long 	ADDITIONAL_OFFSET_D1	= 10;
		static const unsigned long 	ADDITIONAL_OFFSET_D2	= 1;
		static const unsigned long 	ADDITIONAL_OFFSET_D3	= 11;

	public:
		Participant(GuidPrefix_t prefix, void* domain_participant);
		~Participant();

		ParticipantProxy*	get_remote_participant(GuidPrefix_t guid_prefix);

		bool				add_new_participant_proxy(ParticipantProxy* proxy);

	private:
		void				_regist_transceivers(unsigned long domain_id);
	};
}

#ifdef __cplusplus
}
#endif
#endif
