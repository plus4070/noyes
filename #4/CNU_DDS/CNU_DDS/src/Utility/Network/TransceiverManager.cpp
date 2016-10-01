#include "../../../include/Utility/SystemDefinition.h"
#include "../../../include/Utility/Network/TransceiverManager.h"

#include <stdio.h>

namespace CNU_DDS
{
	TransceiverManager*	TRANSCEIVER_MANAGER;

	TransceiverManager::TransceiverManager()
	{
		transceivers	= new DoublyLinkedList<TransceiverInfo>();
	}

	TransceiverManager::~TransceiverManager()
	{

	}

	TransceiverManager*	TransceiverManager::getManagerInstance()
	{
		if(TRANSCEIVER_MANAGER	== NULL)
		{
			TRANSCEIVER_MANAGER	= new TransceiverManager();
			//create_thread(&TRANSCEIVER_MANAGER->switching_handle, TransceiverManager::switching, (void*)TRANSCEIVER_MANAGER);
		}

		return TRANSCEIVER_MANAGER;
	}

	Transceiver*		TransceiverManager::registerTransceiver(Locator_t a_locator, unsigned long a_tid)
	{
		TransceiverInfo*	ti		= NULL;
		Transceiver*		result	= getTransceiverInstance(a_tid);

		if(result == NULL)
		{
			ti				= new TransceiverInfo();
			ti->transceiver	= new Transceiver(a_locator);
			ti->tid			= a_tid;
			transceivers->insertInRear(ti);
			result	= ti->transceiver;
			result->start();
		}

		return result;
	}

	Transceiver*			TransceiverManager::getTransceiverInstance(unsigned long tid)
	{
		Node<TransceiverInfo>*	head_node	= transceivers->getNodeByIndex(0);
		Node<TransceiverInfo>*	cur_node	= transceivers->getNodeByIndex(1);
		Transceiver*			transceiver	= NULL;

		transceivers->listLock();

		while(cur_node != head_node)
		{
			if(cur_node->value->tid == tid)
			{
				transceiver	= cur_node->value->transceiver;
				break;
			}

			cur_node	= cur_node->rear;
		}

		transceivers->listUnlock();

		return transceiver;
	}
/*
	void*				TransceiverManager::switching(void* arg)
	{
		TransceiverManager*	tm	= (TransceiverManager*)arg;
		Transceiver*		transceiver;
		unsigned long		i;

		while(true)
		{
			for(i=1; i<=tm->transceivers->getSize(); i++)
			{
				transceiver	= tm->transceivers->getNodeByIndex(i)->value->transceiver;

				transceiver->switching();
			}

			native_sleep(RECEIVING_QUEUE_CHECKING_DURATION);
		}
	}*/
}
