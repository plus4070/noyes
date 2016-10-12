#include "../../../include/DCPS/Domain/DomainParticipant.h"

#include "../../../include/RTPS/Structure/Participant.h"

#include "../../../include/Utility/Network/TransceiverManager.h"
#include "../../../include/Utility/Network/NetworkManager.h"

#include <stdio.h>

namespace CNU_DDS
{
	Participant::Participant(GuidPrefix_t prefix, void* domain_participant)
	{
		int					domain_id;

		guid.guid_prefix	= prefix;
		guid.entity_id		= ENTITYID_PARTICIPANT;

		protocol_version	= PROTOCOL_VERSION;

		vendor_id.vendor_id[0]	= 0x00;
		vendor_id.vendor_id[1]	= 0x00;

		default_unicast_locator_list	= new LocatorSeq();
		default_multicast_locator_list	= new LocatorSeq();
		metatraffic_unicast_locator_list	= new LocatorSeq();
		metatraffic_multicast_locator_list	= new LocatorSeq();

		endpoints	= new EndpointSeq();

		related_dcps_participant	= (pDomainParticipant)domain_participant;

		matched_participants	= new ParticipantProxySeq();

		is_created_a_pdp_message	= false;

		domain_id	= related_dcps_participant->get_domain_id();

		_regist_transceivers(domain_id);
	}

	Participant::~Participant()
	{
		delete(endpoints);
		delete(matched_participants);
		delete(default_unicast_locator_list);
		delete(default_multicast_locator_list);
		delete(metatraffic_unicast_locator_list);
		delete(metatraffic_multicast_locator_list);
	}

	ParticipantProxy*	Participant::get_remote_participant(GuidPrefix_t guid_prefix)
	{
		ParticipantProxy*	result	= NULL;
		int 				i		= 0;

		for(i=1; i<=matched_participants->getSize(); i++)
		{
			result	= matched_participants->getNodeByIndex(i)->value;

			if( (result != NULL) && !(memcmp(&result->guid_prefix, &guid_prefix, sizeof(GuidPrefix_t))) )
			{
				return result;
			}
		}

		return NULL;
	}

	void				Participant::_regist_transceivers(unsigned long domain_id)
	{
		TransceiverManager*	tm	= TransceiverManager::getManagerInstance();
		unsigned long		transceiver_id;
		NetworkInfoManager*	net_man	= NetworkInfoManager::getNetworkInfoManagerInstance();
		Locator_t*			locator				= NULL;
		Locator_t			transceiver_locator;//	= {LOCATOR_KIND_UDPv4, port_number, net_man->getLocalIPAddressList()->getFirst()->ipAddr};
		unsigned long		ipv4_addr;

		transceiver_locator.kind	= LOCATOR_KIND_UDPv4;

		for(int i=1; i<=net_man->getLocalIPAddressList()->getSize(); i++)
		{
			memcpy(transceiver_locator.address, net_man->getLocalIPAddressList()->getNodeByIndex(i)->value->ipAddr, 16);
			printf("Participant::regist_transceivers() address(%s)\n", transceiver_locator.address);

			if(!strncmp(transceiver_locator.address, "168", 3))
			{
				ipv4_addr	= inet_addr(transceiver_locator.address);
				memset(transceiver_locator.address, 0, 16);
				memcpy(transceiver_locator.address + 12, &ipv4_addr, 4);
				break;
			}
		}

		/* Metatraffic Multicast */
		transceiver_locator.port	= PORT_BASE_NUMBER + ( DOMAIN_ID_GAIN * domain_id ) + ADDITIONAL_OFFSET_D0;
		printf("port(%d)\n", transceiver_locator.port);
		transceiver_id				= TRANSCEIVER_ID_METATRAFFIC_MULTICAST + (domain_id * TRANSCEIVER_ID_DOMAIN_ID_GAIN);

		tm->registerTransceiver(transceiver_locator, transceiver_id);

		locator		= new Locator_t();
		*locator	= transceiver_locator;
		metatraffic_multicast_locator_list->insertInRear(locator);

		/* Metatraffic Unicast */
		transceiver_locator.port	= PORT_BASE_NUMBER + ( DOMAIN_ID_GAIN * domain_id ) + ADDITIONAL_OFFSET_D1 + ( PARTICIPANT_ID_GAIN * 0/*participantId*/ );
		printf("port(%d)\n", transceiver_locator.port);
		transceiver_id				= TRANSCEIVER_ID_METATRAFFIC_UNICAST + (domain_id * TRANSCEIVER_ID_DOMAIN_ID_GAIN);

		tm->registerTransceiver(transceiver_locator, transceiver_id);

		locator		= new Locator_t();
		*locator	= transceiver_locator;
		metatraffic_unicast_locator_list->insertInRear(locator);

		/* Default Multicast */
		//transceiver_locator.port	= PORT_BASE_NUMBER + ( DOMAIN_ID_GAIN * a_domain_id ) + ADDITIONAL_OFFSET_D2;

		/* Default Unicast */
		transceiver_locator.port	= PORT_BASE_NUMBER + ( DOMAIN_ID_GAIN * domain_id ) + ADDITIONAL_OFFSET_D3 + ( PARTICIPANT_ID_GAIN * 0/*participantId*/ );
		printf("port(%d)\n", transceiver_locator.port);
		transceiver_id				= TRANSCEIVER_ID_DEFAULT_UNICAST + (domain_id * TRANSCEIVER_ID_DOMAIN_ID_GAIN);

		tm->registerTransceiver(transceiver_locator, transceiver_id);

		locator		= new Locator_t();
		*locator	= transceiver_locator;
		default_unicast_locator_list->insertInRear(locator);
	}

	bool				Participant::add_new_participant_proxy(ParticipantProxy* proxy)
	{
		int 				i				= 0;
		ParticipantProxy*	cur_proxy		= NULL;

		if(proxy->guid_prefix == this->guid.guid_prefix)
		{
			return false;
		}

		for(i=1; i<=matched_participants->getSize(); i++)
		{
			cur_proxy	= matched_participants->getNodeByIndex(i)->value;

			if(cur_proxy->guid_prefix == proxy->guid_prefix)
			{
				return false;
			}
		}

		matched_participants->insertInRear(proxy);
		related_dcps_participant->_send_a_pdp_message();
		return true;
	}
}
