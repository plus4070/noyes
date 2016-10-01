#ifndef BASEDEFINITIONS_H_
#define BASEDEFINITIONS_H_

#include "../../../Utility/PlatformOrientedInterface.h"
#include "../../../Utility/DoublyLinkedList.h"

#include <iostream>

namespace CNU_DDS
{
	typedef	DoublyLinkedList<std::string>	StringSeq;

	/********************************************************************
	 * DCPS
	 ********************************************************************/

	typedef	class Entity*						pEntity;
	typedef	class DomainParticipant*			pDomainParticipant;
	typedef	class Publisher*					pPublisher;
	typedef class Subscriber*					pSubscriber;
	typedef	class DataWriter*					pDataWriter;
	typedef	class DataReader*					pDataReader;
	typedef class Topic*						pTopic;
	typedef class TopicDescription*				pTopicDescription;
	typedef class ContentFilteredTopic*			pContentFilteredTopic;
	typedef class MultiTopic*					pMultiTopic;

	typedef	class DomainParticipantListener*	pDomainParticipantListener;
	typedef class PublisherListener*			pPublisherListener;
	typedef class SubscriberListener*			pSubscriberListener;
	typedef class DataWriterListener*			pDataWriterListener;
	typedef class DataReaderListener*			pDataReaderListener;
	typedef class TopicListener*				pTopicListener;

	typedef DoublyLinkedList<class DomainParticipant>	DomainParticipantSeq;
	typedef DoublyLinkedList<class Publisher>			PublisherSeq;
	typedef DoublyLinkedList<class Subscriber>			SubscriberSeq;
	typedef DoublyLinkedList<class DataWriter>			DataWriterSeq;
	typedef DoublyLinkedList<class DataReader>			DataReaderSeq;
	typedef DoublyLinkedList<class Topic>				TopicSeq;
	typedef DoublyLinkedList<class WaitSet>				WaitSetSeq;

	/********************************************************************
	 * RTPS
	 ********************************************************************/
	typedef class Participant*					pParticipant;
	typedef class Writer*						pWriter;
	typedef class Reader*						pReader;
}

#endif
