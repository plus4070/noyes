#ifndef BASEDEFINITIONS_H_
#define BASEDEFINITIONS_H_

#include "../../../Utility/PlatformOrientedInterface.h"
#include "../../../Utility/DoublyLinkedList.h"

#include <iostream>

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
	typedef	DoublyLinkedList<std::string>	StringSeq;

	/********************************************************************
	 * DCPS
	 ********************************************************************/

	typedef	class CNU_DDS_DLL_API Entity*						pEntity;
	typedef	class CNU_DDS_DLL_API DomainParticipant*			pDomainParticipant;
	typedef	class CNU_DDS_DLL_API Publisher*					pPublisher;
	typedef class CNU_DDS_DLL_API Subscriber*					pSubscriber;
	typedef	class CNU_DDS_DLL_API DataWriter*					pDataWriter;
	typedef	class CNU_DDS_DLL_API DataReader*					pDataReader;
	typedef class CNU_DDS_DLL_API Topic*						pTopic;
	typedef class CNU_DDS_DLL_API TopicDescription*				pTopicDescription;
	typedef class CNU_DDS_DLL_API ContentFilteredTopic*			pContentFilteredTopic;
	typedef class CNU_DDS_DLL_API MultiTopic*					pMultiTopic;

	typedef	class CNU_DDS_DLL_API DomainParticipantListener*	pDomainParticipantListener;
	typedef class CNU_DDS_DLL_API PublisherListener*			pPublisherListener;
	typedef class CNU_DDS_DLL_API SubscriberListener*			pSubscriberListener;
	typedef class CNU_DDS_DLL_API DataWriterListener*			pDataWriterListener;
	typedef class CNU_DDS_DLL_API DataReaderListener*			pDataReaderListener;
	typedef class CNU_DDS_DLL_API TopicListener*				pTopicListener;

	typedef DoublyLinkedList<class CNU_DDS_DLL_API DomainParticipant>	DomainParticipantSeq;
	typedef DoublyLinkedList<class CNU_DDS_DLL_API Publisher>			PublisherSeq;
	typedef DoublyLinkedList<class CNU_DDS_DLL_API Subscriber>			SubscriberSeq;
	typedef DoublyLinkedList<class CNU_DDS_DLL_API DataWriter>			DataWriterSeq;
	typedef DoublyLinkedList<class CNU_DDS_DLL_API DataReader>			DataReaderSeq;
	typedef DoublyLinkedList<class CNU_DDS_DLL_API Topic>				TopicSeq;
	typedef DoublyLinkedList<class CNU_DDS_DLL_API WaitSet>				WaitSetSeq;

	/********************************************************************
	 * RTPS
	 ********************************************************************/
	typedef class CNU_DDS_DLL_API Participant*					pParticipant;
	typedef class CNU_DDS_DLL_API Writer*						pWriter;
	typedef class CNU_DDS_DLL_API Reader*						pReader;
}

#ifdef __cplusplus
}
#endif
#endif
