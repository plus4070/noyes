#include "../../../include/DCPS/BuiltinEntity/BuiltinSubscriptionsWriterListener.h"

namespace CNU_DDS
{
	BuiltinSubscriptionsWriterListener::BuiltinSubscriptionsWriterListener()
	{

	}

	BuiltinSubscriptionsWriterListener::~BuiltinSubscriptionsWriterListener()
	{

	}

	void	BuiltinSubscriptionsWriterListener::on_liveliness_lost(pDataWriter the_wirter, LivelinessLostStatus* status)
	{

	}

	void	BuiltinSubscriptionsWriterListener::on_offered_deadline_missed(pDataWriter the_writer, OfferedDeadlineMissedStatus* status)
	{

	}

	void	BuiltinSubscriptionsWriterListener::on_offered_incompatible_qos(pDataWriter the_writer, OfferedIncompatibleQosStatus* status)
	{

	}

	void	BuiltinSubscriptionsWriterListener::on_publication_matched(pDataWriter the_writer, PublicationMatchedStatus* status)
	{

	}
}
