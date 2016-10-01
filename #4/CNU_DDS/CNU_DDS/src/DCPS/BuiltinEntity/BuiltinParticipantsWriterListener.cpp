#include "../../../include/DCPS/BuiltinEntity/BuiltinParticipantsWriterListener.h"

namespace CNU_DDS
{
	BuiltinParticipantsWriterListener::BuiltinParticipantsWriterListener()
	{

	}

	BuiltinParticipantsWriterListener::~BuiltinParticipantsWriterListener()
	{

	}

	void	BuiltinParticipantsWriterListener::on_liveliness_lost(pDataWriter the_wirter, LivelinessLostStatus* status)
	{

	}

	void	BuiltinParticipantsWriterListener::on_offered_deadline_missed(pDataWriter the_writer, OfferedDeadlineMissedStatus* status)
	{

	}

	void	BuiltinParticipantsWriterListener::on_offered_incompatible_qos(pDataWriter the_writer, OfferedIncompatibleQosStatus* status)
	{

	}

	void	BuiltinParticipantsWriterListener::on_publication_matched(pDataWriter the_writer, PublicationMatchedStatus* status)
	{

	}
}
