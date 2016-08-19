#include "../../../include/Utility/Network/Transceiver.h"
#include "../../../include/Utility/SystemInformation.h"
#include "../../../include/RTPS/Messages/Message.h"
#include "../../../include/RTPS/Messages/Submessage.h"
#include "../../../include/RTPS/Messages/Submessages/submessages.h"
#include "../../../include/RTPS/Structure/Type/EntityId_t.h"
#include "../../../include/RTPS/Structure/Type/EntityEvent.h"
#include "../../../include/RTPS/Behavior/Type/WriterState.h"
#include "../../../include/RTPS/Behavior/Type/ReaderState.h"
#include <stdio.h>

namespace CNU_DDS
{
	Transceiver::Transceiver(Locator_t a_locator)
	{
		locator	= a_locator;

		//test_recv_count	= 0;
		//test_proc_count	= 0;
		//test_data_count	= 0;
		//test_call_count	= 0;

		init();
	}

	Transceiver::~Transceiver()
	{
		stop();

		destroy_thread(&recv_handle);
		destroy_thread(&send_handle);
		destroy_thread(&process_handle);

		destroy_condition(&event_handle_for_send);
		destroy_condition(&event_handle_for_process);

		destroy_mutex(&critical_section_for_send_queue);
		destroy_mutex(&critical_section_for_process);

		/*delete(recv_queue_0);
		delete(recv_queue_1);*/
		//delete(recv_queue);
		delete(recv_queue_0);
		delete(recv_queue_1);
		delete(send_queue);
	}

	void		Transceiver::init()
	{
		/*
		long	addr	= 0x00;
		memcpy(&addr, this->locator.address + 12, 4);
		this->socket.setInternalAddress(addr);
		this->socket.setInternalPort(this->locator.port);
		this->socket.binding();
		this->socket.setExternal(addr, this->locator.port);
		*/
		chk_sending		= true;
		chk_processing	= true;
		chk_recv_queue_0	= true;
		recv_queue_0		= new SimpleLinkedList();
		recv_queue_1		= new SimpleLinkedList();
		/*chk_recv_queue_0	= true;
		recv_queue_0	= new DoublyLinkedList<ByteStream>();
		recv_queue_1	= new DoublyLinkedList<ByteStream>();*/
		//proc_queue	= new DoublyLinkedList<ByteStream>();
		//recv_queue	= new DoublyLinkedList<ByteStream>();
		send_queue	= new DoublyLinkedList<SendingInfo>();
		//ready_queue	= new DoublyLinkedList<SendingInfo>();

		//this->socket.setInternalAddress(this->locator.address);
		this->socket.setInternalAddressByDDSLocator(this->locator.address);
		this->socket.setInternalPort(this->locator.port);
		this->socket.binding();

		this->isRunning	= false;

		create_condition(&event_handle_for_send);
		create_condition(&event_handle_for_process);

		create_mutex(&critical_section_for_send_queue);
		create_mutex(&critical_section_for_process);

		registered_list	= new DoublyLinkedList<Endpoint>();
	}

	void					Transceiver::regist(Endpoint* a_handle)
	{
		registered_list->insertInRear(a_handle);
	}

	void					Transceiver::remove(Endpoint* a_handle)
	{
		Node<Endpoint>*	curNode;

		for(int i=1; i<=registered_list->getSize(); i++)
		{
			curNode	= registered_list->getNodeByIndex(i);

			if(curNode->value->guid == a_handle->guid)
			{
				registered_list->popByIndex(i);
				break;
			}
		}
	}

	void					Transceiver::send(ByteStream* a_payload, Locator_t a_dest)
	{
		SendingInfo*	new_data	= new SendingInfo();
		new_data->data				= a_payload;
		new_data->dest				= a_dest;

		send_queue->insertInRear(new_data);

		if(!chk_sending)
		{
			condition_signal(&event_handle_for_send);
		}

		//ready_queue->insertInRear(new_data);
	}

	void					Transceiver::start()
	{
		if(!isRunning)
		{
			isRunning	= true;

			create_thread(&recv_handle, Transceiver::receiving, this);
			create_thread(&send_handle, Transceiver::sending, this);
			create_thread(&process_handle, Transceiver::processing, this);
		}
	}

	void					Transceiver::stop()
	{
		if(isRunning)
		{
			isRunning	= false;
		}
	}

	/*void					Transceiver::switching()
	{
		bool							need_sig;

		if(ready_queue->getSize() > 0)
		{
			need_sig	= false;
			if(send_queue->getSize() == 0)
			{
				need_sig	= true;
			}

			ready_queue->relocateAll(send_queue);

			if(need_sig)
			{
				condition_signal(&event_handle_for_send);
			}
		}
	}*/

	bool	Transceiver::operator	==	( const Transceiver rvalue )
	{
		return this->locator == rvalue.locator;
	}

	bool	Transceiver::operator	!=	( const Transceiver rvalue )
	{
		return !(this->locator == rvalue.locator);
	}

	void					Transceiver::send()
	{
		SendingInfo*	sending_info;
		long			dest_addr;

		if(send_queue->getSize() == 0)
		{
			chk_sending	= false;
			condition_wait(&event_handle_for_send, &critical_section_for_send_queue);
			chk_sending	= true;
		}
		else
		{
			sending_info	= send_queue->popFirst();

			memcpy(&dest_addr, sending_info->dest.address + 12, 4);

			//socket.setExternal(dest_addr, sending_info->dest.port);
			//socket.setExternal(sending_info->dest.address, sending_info->dest.port);
			socket.setExternalByDDSLocator(sending_info->dest.address, sending_info->dest.port);
			socket.send(sending_info->data->getData(), sending_info->data->getLength());

			delete(sending_info->data);
			delete(sending_info);

			//Experimental best == 20*1000
			//for(int i=0; i<10*1000; i++);
		}
	}

	void					Transceiver::receive()
	{
		ByteStream*	recv_stream;

		recv_stream	= socket.receive();
		//test_recv_count++;
/*
		recv_queue->insertInRear(recv_stream);

		if(!chk_processing)
		{
			condition_signal(&event_handle_for_process);
		}
*/

		if(chk_recv_queue_0)
		{
			recv_queue_0->insertInRear((unsigned long)recv_stream);
		}
		else
		{
			recv_queue_1->insertInRear((unsigned long)recv_stream);
		}

		if(!chk_processing)
		{
			chk_recv_queue_0	=	!chk_recv_queue_0;

			//mutex_lock(&critical_section_for_process);
			condition_signal(&event_handle_for_process);
			//mutex_unlock(&critical_section_for_process);
		}

	}

	void					Transceiver::process()
	{
		EntityEvent*					event;
		SimpleLinkedList*				proc_queue	= chk_recv_queue_0?recv_queue_1:recv_queue_0;
		SimpleLinkedList*				recv_queue	= chk_recv_queue_0?recv_queue_0:recv_queue_1;

		/*
		if(recv_queue->getSize() == 0)
		{
			chk_processing	= false;
			condition_wait(&event_handle_for_process, &critical_section_for_process);
			chk_processing	= true;
		}*/

		ByteStream*					recv_stream;
		Message*					recv_message		= NULL;
		Message*					message;
		DoublyLinkedList<Message>*	messages;
		int							i;

		EntityId	recv_reader_id;
		EntityId	recv_writer_id;
		Endpoint*	endpoint;
		EntityId_t	entity_id;

		bool		is_rtps_message;

		//recv_stream	= proc_queue->popFirst();
		recv_stream	= (ByteStream*)proc_queue->popFirst();

		if(recv_stream == NULL)
		{
			if(recv_queue->head == NULL)
			{
//printf("SLEEP (%08X, %d)\n", this, chk_recv_queue_0);

				chk_processing	= false;
				//mutex_lock(&critical_section_for_process);
				condition_wait(&event_handle_for_process, &critical_section_for_process);
				//mutex_unlock(&critical_section_for_process);
				chk_processing	= true;

//				native_sleep(50);
//printf("WAKE (%08X, %d)\n", this, chk_recv_queue_0);
			}
			else
			{
				chk_recv_queue_0	= !chk_recv_queue_0;
			}
		}
		else
		{
			//test_proc_count++;
			recv_message	= new Message();
			is_rtps_message	= recv_message->Deserialize(recv_stream);

			if(is_rtps_message)
			{
				messages	= message_decomposition(recv_message);

				while(messages->getSize() > 0)
				{
					message	= messages->popFirst();

					recv_reader_id.value	= ENTITYID_UNKNOWN;
					recv_writer_id.value	= ENTITYID_UNKNOWN;

					entity_id	= _chk_recv_id(message->submessages->getNodeByIndex(1)->value, &recv_reader_id, &recv_writer_id);

					for(i=1; i<=registered_list->getSize(); i++)
					{
						endpoint	= registered_list->getNodeByIndex(i)->value;

						if(endpoint->guid.entity_id == entity_id)
						{
							event			= new EntityEvent();

							event->data		= (unsigned long)message;

							switch(message->submessages->getFirst()->submessage_header.submessage_id)
							{
							case SUBMESSAGE_KIND_DATA:
							case SUBMESSAGE_KIND_DATA_FRAG:
								//printf("DATA\n");
								event->target	= (unsigned long)(&((Data*)message->submessages->getFirst())->writer_id.value);
								event->event	= (unsigned long)DATA_IS_RECEIVED;
								break;
							case SUBMESSAGE_KIND_HEARTBEAT:
							case SUBMESSAGE_KIND_HEARTBEAT_FRAG:
								//printf("Transceiver receives a HEARTBEAT message\n");
								event->target	= (unsigned long)(&((Heartbeat*)message->submessages->getFirst())->writer_id.value);
								event->event	= (unsigned long)HEARTBEAT_IS_RECEIVED;
								break;
							case SUBMESSAGE_KIND_ACKNACK:
								//printf("ACKNACK\n");
								event->target	= (unsigned long)(&((AckNack*)message->submessages->getFirst())->reader_id.value);
								event->event	= (unsigned long)ACKNACK_IS_RECEIVED;
								break;
							default:
								//printf("\tA message is not Data/Heartbeat/AckNack\n");
								break;
							}

							//printf("endpoint(%08X)\n", endpoint);
							endpoint->SetEvent(event);
							break;
						}
					}
				}

				recv_message->submessages->deleteAll();
				delete(recv_message);

			}
			else
			{
				//printf("\tERROR: A received message is not RTPS message\n");
			}
		}
	}

	EntityId_t				Transceiver::_chk_recv_id(Submessage*	submessage, EntityId* reader_id, EntityId* writer_id)
	{
		EntityId_t	entity_id;

		switch(submessage->submessage_header.submessage_id)
		{
		case SUBMESSAGE_KIND_DATA:
//test_data_count++;
			reader_id->value	= (((Data*)submessage)->reader_id.value);
			writer_id->value	= (((Data*)submessage)->writer_id.value);
			break;

		case SUBMESSAGE_KIND_ACKNACK:
			//printf("Transceiver::_chk_recv_id::ACK_NACK\n");
			reader_id->value	= (((AckNack*)submessage)->reader_id.value);
			writer_id->value	= (((AckNack*)submessage)->writer_id.value);
			break;

		case SUBMESSAGE_KIND_HEARTBEAT:
//printf("\n");
//printf("DATA(%d)\n", test_data_count);
//printf("PROC(%d)\n", test_proc_count);
//printf("CALL(%d)\n", test_call_count);
//printf("RECV(%d)\n", test_recv_count);
			reader_id->value	= (((Heartbeat*)submessage)->reader_id.value);
			writer_id->value	= (((Heartbeat*)submessage)->writer_id.value);
			break;

		default:
			return entity_id;
		}

if(submessage->submessage_header.submessage_id == SUBMESSAGE_KIND_DATA && writer_id->value == ENTITYID_SEDP_BUILTIN_SUBSCRIPTIONS_WRITER)
{
	int jkd = 0;
	jkd++;
}

		if(writer_id->value == ENTITYID_SPDP_BUILTIN_PARTICIPANT_WRITER)
		{
			//printf("WRITER == PDP_WRITER\n");
			entity_id	= ENTITYID_SPDP_BUILTIN_PARTICIPANT_READER;
		}
		else if(writer_id->value == ENTITYID_SEDP_BUILTIN_PUBLICATIONS_WRITER)
		{
			//printf("WRITER == PUB_WRITER\n");
			if(submessage->submessage_header.submessage_id == SUBMESSAGE_KIND_ACKNACK)
			{
				entity_id	= ENTITYID_SEDP_BUILTIN_PUBLICATIONS_WRITER;
				//printf("ACK_NACK_PUBLICATION\n");
			}
			else
			{
				entity_id	= ENTITYID_SEDP_BUILTIN_PUBLICATIONS_READER;
			}
		}
		else if(writer_id->value == ENTITYID_SEDP_BUILTIN_SUBSCRIPTIONS_WRITER)
		{
			//printf("WRITER == SUB_WRITER\n");
			if(submessage->submessage_header.submessage_id == SUBMESSAGE_KIND_ACKNACK)
			{
				entity_id	= ENTITYID_SEDP_BUILTIN_SUBSCRIPTIONS_WRITER;
				//printf("ACK_NACK_SUBSCRIPTION\n");
			}
			else
			{
				entity_id	= ENTITYID_SEDP_BUILTIN_SUBSCRIPTIONS_READER;
			}
		}
		else
		{
			//printf("WRITER == USER_WRITER\n");
			if(submessage->submessage_header.submessage_id == SUBMESSAGE_KIND_ACKNACK)
			{
				entity_id	= writer_id->value;
			}
			else
			{
				entity_id	= reader_id->value;
			}
		}

		return entity_id;
	}

	void*					Transceiver::sending(void* arg)
	{
		Transceiver*	transceiver	= (Transceiver*)arg;

		while(transceiver->isRunning)
		{
			transceiver->send();
		}

		printf("ERROR: Transceiver::sending\n");

		return 0;
	}

	void*					Transceiver::receiving(void* arg)
	{
		Transceiver*	transceiver	= (Transceiver*)arg;

		while(transceiver->isRunning)
		{
			transceiver->receive();
		}

		printf("ERROR: Transceiver::receiving\n");

		return 0;
	}

	void*					Transceiver::processing(void* arg)
	{
		Transceiver*	transceiver	= (Transceiver*)arg;

		while(transceiver->isRunning)
		{
			//transceiver->test_call_count++;
			transceiver->process();
		}

		printf("ERROR: Transceiver::processing\n");

		return 0;
	}

	DoublyLinkedList<Message>*	Transceiver::message_decomposition(Message* recv_message)
	{
		DoublyLinkedList<Message>*	list	= new DoublyLinkedList<Message>();
		Node<Submessage>*			head	= recv_message->submessages->getNodeByIndex(0);
		Node<Submessage>*			next	= NULL;
		Node<Submessage>*			node	= recv_message->submessages->getNodeByIndex(1);

		while(node != head)
		{
			next	= node->rear;

			switch(node->value->submessage_header.submessage_id)
			{
			case SUBMESSAGE_KIND_DATA:
			case SUBMESSAGE_KIND_DATA_FRAG:
			case SUBMESSAGE_KIND_HEARTBEAT:
			case SUBMESSAGE_KIND_HEARTBEAT_FRAG:
			case SUBMESSAGE_KIND_ACKNACK:
				list->insertInRear(create_a_message(recv_message, node->value));
				recv_message->submessages->cutNode(node);
				break;
			default:
				break;
			}

			node	= next;
		}

		return list;
	}

	Message*					Transceiver::create_a_message(Message* recv_message, Submessage* recv_submessage)
	{
		Message*	new_message	= new Message();

		memcpy(&new_message->header, &recv_message->header, sizeof(Header));
		new_message->submessages->insertInRear(recv_submessage);

		return new_message;
	}
}
