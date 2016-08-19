#include "../../../include/DCPS/BuiltinEntity/BuiltinPublicationsWriterListener.h"

namespace CNU_DDS
{
	BuiltinPublicationsWriterListener::BuiltinPublicationsWriterListener()
	{

	}

	BuiltinPublicationsWriterListener::~BuiltinPublicationsWriterListener()
	{

	}

	void	BuiltinPublicationsWriterListener::on_liveliness_lost(pDataWriter the_wirter, LivelinessLostStatus* status)
	{

	}

	void	BuiltinPublicationsWriterListener::on_offered_deadline_missed(pDataWriter the_writer, OfferedDeadlineMissedStatus* status)
	{

	}

	void	BuiltinPublicationsWriterListener::on_offered_incompatible_qos(pDataWriter the_writer, OfferedIncompatibleQosStatus* status)
	{

	}

	void	BuiltinPublicationsWriterListener::on_publication_matched(pDataWriter the_writer, PublicationMatchedStatus* status)
	{

	}
}
