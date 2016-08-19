#ifndef INCLUDE_RTPS_BEHAVIOR_LISTENERPROCESSOR_H_
#define INCLUDE_RTPS_BEHAVIOR_LISTENERPROCESSOR_H_

#include "../../Utility/EventProcessor/EventProcessor.h"
#include "../../Status/Type/StatusKind.h"

namespace CNU_DDS
{
	enum ListenerEvent
	{
		EVENT_INCONSISTENT_TOPIC,
		EVENT_SAMPLE_REJECTED,
		EVENT_LIVELINESS_CHANGED,
		EVENT_REQUESTED_DEADLINE_MISSED,
		EVENT_REQUESTED_INCOMPATIBLE_QOS,
		EVENT_DATA_AVAILABLE,
		EVENT_SAMPLE_LOST,
		EVENT_SUBSCRIPTION_MATCHED,
		EVENT_OFFERED_DEADLINE_MISSED,
		EVENT_OFFERED_INCOMPATIBLE_QOS,
		EVENT_LIVELINESS_LOST,
		EVENT_PUBLICATION_MATCHED
	};

	class ListenerProcessor: public EventProcessor
	{
	private:
		ListenerProcessor();
		~ListenerProcessor();

	public:
		static ListenerProcessor*	getListenerProcessorInstance();

		void	process();

	private:
		bool	_is_enabled(unsigned long arg, StatusKind kind);
		bool	_inconsistent_topic(unsigned long arg);
		bool	_data_on_readers(unsigned long arg);
		bool	_sample_rejected(unsigned long arg);
		bool	_liveliness_changed(unsigned long arg);
		bool	_requested_deadline_missed(unsigned long arg);
		bool	_requested_incompatible_qos(unsigned long arg);
		bool	_data_available(unsigned long arg);
		bool	_sample_lost(unsigned long arg);
		bool	_subscription_matched(unsigned long arg);
		bool	_offered_deadline_missed(unsigned long arg);
		bool	_offered_incompatible_qos(unsigned long arg);
		bool	_liveliness_lost(unsigned long arg);
		bool	_publication_matched(unsigned long arg);
	};
}

#endif /* INCLUDE_RTPS_BEHAVIOR_LISTENERPROCESSOR_H_ */
