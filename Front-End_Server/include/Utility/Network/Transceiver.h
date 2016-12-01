#ifndef INCLUDE_UTILITY_NETWORK_TRANSCEIVER_H_
#define INCLUDE_UTILITY_NETWORK_TRANSCEIVER_H_

#include "../PlatformOrientedInterface.h"
#include "../../RTPS/Messages/Type/ByteStream.h"
#include "../../RTPS/Messages/Message.h"
#include "../../RTPS/Messages/Submessage.h"
#include "../../RTPS/Messages/Submessages/submessages.h"
#include "../../RTPS/Messages/SubmessageElements/EntityId.h"
#include "../../RTPS/Structure/Type/Locator_t.h"
#include "../../RTPS/Structure/Endpoint.h"
#include "../DoublyLinkedList.h"
#include "SimpleLinkedList.h"
#include "UDPsocket.h"

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
	class CNU_DDS_DLL_API Transceiver
	{
	private:
		typedef struct
		{
			ByteStream*	data;
			Locator_t	dest;
		}SendingInfo;

	public:
		Locator_t				locator;

	private:
		UDPSocket				socket;

		THREAD_TYPE_NATIVE				recv_handle;
		THREAD_TYPE_NATIVE				send_handle;
		THREAD_TYPE_NATIVE				process_handle;
		CONDITION_TYPE_NATIVE			event_handle_for_send;
		CONDITION_TYPE_NATIVE			event_handle_for_process;
		bool							chk_sending;
		bool							chk_processing;
		bool							chk_recv_queue_0;
		SimpleLinkedList*				recv_queue_0;
		SimpleLinkedList*				recv_queue_1;

		DoublyLinkedList<SendingInfo>*	send_queue;
		MUTEX_TYPE_NATIVE				critical_section_for_send_queue;
		MUTEX_TYPE_NATIVE				critical_section_for_process;
		MUTEX_TYPE_NATIVE				critical_section_for_receiving;

		DoublyLinkedList<Endpoint>*		registered_list;
		bool							isRunning;

	private:
		void		init();

		EntityId_t				_chk_recv_id(Submessage* submessage, EntityId* reader_id, EntityId* writer_id);

	public:
		Transceiver(Locator_t a_locator);
		~Transceiver();

		void					regist(Endpoint* a_handle);
		void					remove(Endpoint* a_handle);

		void					send(ByteStream* a_payload, Locator_t a_dest);

		void					start();
		void					stop();


		bool	operator	==	( const Transceiver rvalue );
		bool	operator	!=	( const Transceiver rvalue );

	//private:
		void					send();
		void					receive();
		void					process();

		static void*			sending(void* arg);
		static void*			receiving(void* arg);
		static void*			processing(void* arg);

		DoublyLinkedList<Message>*	message_decomposition(Message* recv_message);
		Message*					create_a_message(Message* recv_message, Submessage* recv_submessage);

	public:
		Transceiver(){}
	};
}

#ifdef __cplusplus
}
#endif
#endif /* INCLUDE_UTILITY_NETWORK_TRANSCEIVER_H_ */
