#ifndef DOMAINPARTICIPANT_H_
#define DOMAINPARTICIPANT_H_

#include "Type/TopicLookupHandle.h"

#include "../Infrastructure/Type/BasicDefinitions.h"
#include "../Infrastructure/Type/DomainId_t.h"
#include "../Infrastructure/Type/Duration_t.h"
#include "../Infrastructure/Type/Time_t.h"
#include "../Infrastructure/Entity.h"
#include "../Infrastructure/DomainEntity.h"

#include "../../QoS/EntityQos/DomainParticipantQos.h"
#include "../../QoS/EntityQos/PublisherQos.h"
#include "../../QoS/EntityQos/SubscriberQos.h"
#include "../../QoS/EntityQos/TopicQos.h"

#include "../../BuiltinTopic/ParticipantBuiltinTopicData.h"
#include "../../BuiltinTopic/TopicBuiltinTopicData.h"

#include "../../RTPS/Discovery/SEDPdiscoveredPublicationData.h"
#include "../../RTPS/Discovery/SEDPdiscoveredSubscriptionData.h"
#include "../../RTPS/Discovery/SEDPdiscoveredTopicData.h"

namespace CNU_DDS
{
	class DomainParticipant : public Entity
	{
	private:
		DomainId_t				domain_id;

	private:
		PublisherQos*			default_publisher_qos;
		SubscriberQos*			default_subscriber_qos;
		TopicQos*				default_topic_qos;

		PublisherSeq*			publisher_list;
		SubscriberSeq*			subscriber_list;
		TopicSeq*				topic_list;

		Publisher*				builtin_publisher;
		pDataWriter				builtinParticipantsWriter;
		pDataWriter				builtinPublicationsWriter;
		pDataWriter				builtinSubscriptionsWriter;
		Subscriber*				builtin_subscriber;
		pDataReader				builtinParticipantsReader;
		pDataReader				builtinPublicationsReader;
		pDataReader				builtinSubscriptionsReader;

		DoublyLinkedList<SEDPdiscoveredPublicationData>*	remote_datawriters;
		DoublyLinkedList<SEDPdiscoveredSubscriptionData>*	remote_datareaders;

	private:
		TopicLookupHandleSeq	topic_find_list;

	public:
		pParticipant					related_rtps_participant;
		ParticipantBuiltinTopicDataSeq	matched_participants;

	public:	// inherited from Entity
		ReturnCode_t	set_qos(DomainParticipantQos* qos_list);
		ReturnCode_t	get_qos(DomainParticipantQos* qos_list);

		ReturnCode_t	set_listener(Listener* a_listener, StatusMask mask);
		Listener*		get_listener(void);

		ReturnCode_t	enable();

	public:
		DomainParticipant(DomainId_t a_domain_id, unsigned long count);
		~DomainParticipant();

		pPublisher				create_publisher(PublisherQos* qos_list, pPublisherListener a_listener, StatusMask mask);
		pSubscriber				create_subscriber(SubscriberQos* qos_list, pSubscriberListener a_listener, StatusMask mask);
		pTopic					create_topic(std::string topic_name, std::string type_name, TopicQos* qos_list, pTopicListener a_listener, StatusMask mask);
		pContentFilteredTopic	create_content_filtered_topic(std::string name, pTopic related_topic, std::string filter_expression, StringSeq* expression_parameteres);
		pMultiTopic				create_multi_topic(std::string name, std::string type_name, std::string subscription_expression, StringSeq* expression_parameters);

		ReturnCode_t			delete_publisher(pPublisher a_publisher);
		ReturnCode_t			delete_subscriber(pSubscriber a_subscriber);
		ReturnCode_t			delete_topic(pTopic a_topic);
		ReturnCode_t			delete_content_filtered_topic(pContentFilteredTopic a_content_filtered_topic);
		ReturnCode_t			delete_multi_topic(pMultiTopic a_multi_topic);

		ReturnCode_t			ignore_participant(InstanceHandle_t handle);
		ReturnCode_t			ignore_topic(InstanceHandle_t handle);
		ReturnCode_t			ignore_publication(InstanceHandle_t handle);
		ReturnCode_t			ignore_subscription(InstanceHandle_t handle);

		ReturnCode_t			set_default_publisher_qos(PublisherQos* qos_list);
		ReturnCode_t			set_default_subscriber_qos(SubscriberQos* qos_list);
		ReturnCode_t			set_default_topic_qos(TopicQos* qos_list);

		ReturnCode_t			get_default_publisher_qos(PublisherQos* qos_list);
		ReturnCode_t			get_default_subscriber_qos(SubscriberQos* qos_list);
		ReturnCode_t			get_default_topic_qos(TopicQos* qos_list);

		DomainId_t				get_domain_id(void);
		pPublisher				get_builtin_publisher(void);
		pSubscriber				get_builtin_subscriber(void);
		ReturnCode_t			get_current_time(Time_t* current_time);

		ReturnCode_t			get_discovered_participants(InstanceHandleSeq* participant_handles);
		ReturnCode_t			get_discovered_participant_data(ParticipantBuiltinTopicData* participant_data, InstanceHandle_t participant_handle);
		ReturnCode_t			get_discovered_topics(InstanceHandleSeq* topic_handles);
		ReturnCode_t			get_discovered_topic_data(TopicBuiltinTopicData* topic_data, InstanceHandle_t topic_handle);

		ReturnCode_t			delete_contained_entities(void);
		ReturnCode_t			assert_liveliness(void);
		pTopic					find_topic(std::string topic_name, Duration_t timeout);
		pTopicDescription		lookup_topic_description(std::string name);
		bool					contains_entity(InstanceHandle_t a_handle);

	public:
		static void				topic_find_timeout(void* arg);
		static void				pdp_resend_timeout(unsigned long arg);

	public:
		void					_send_a_pdp_message();
		void					_datawriter_is_created(pDataWriter);
		void					_datareader_is_created(pDataReader);
		void					_remote_datawriter_is_discovered(SEDPdiscoveredPublicationData* remote_writer);
		void					_remote_datareader_is_discovered(SEDPdiscoveredSubscriptionData* remote_reader);

	private:
		unsigned long			_create_PDP_message();
		unsigned long			_create_Pub_EDP_message(pDataWriter a_data_wrtier);
		unsigned long			_create_Sub_EDP_message(pDataReader a_data_reader);
		void					_create_builtin_participant_writer(unsigned long p_guid);
		void					_create_builtin_participant_reader(unsigned long p_guid);
		void					_create_builtin_publication_writer(unsigned long p_guid);
		void					_create_builtin_publication_reader(unsigned long p_guid);
		void					_create_builtin_subscription_writer(unsigned long p_guid);
		void					_create_builtin_subscription_reader(unsigned long p_guid);
		void					_create_builtin_topic_writer(unsigned long p_guid);
		void					_create_builtin_topic_reader(unsigned long p_guid);
		void					_set_default_publisher_qos();
		void					_set_default_subscriber_qos();
		void					_set_default_topic_qos();
	};
}

#endif
