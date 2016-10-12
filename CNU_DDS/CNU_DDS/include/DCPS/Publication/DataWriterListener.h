#ifndef DATAWRITERLISTENER_H_
#define DATAWRITERLISTENER_H_

#include "../../Status/Listener.h"
#include "../Infrastructure/Type/BasicDefinitions.h"
#include "../../Status/Status/InclusionsOfStatuses.h"

namespace CNU_DDS
{
	class DataWriterListener : public Listener
	{
	public:
		virtual void	on_liveliness_lost(pDataWriter the_wirter, LivelinessLostStatus* status)					= 0;
		virtual void	on_offered_deadline_missed(pDataWriter the_writer, OfferedDeadlineMissedStatus* status)		= 0;
		virtual void	on_offered_incompatible_qos(pDataWriter the_writer, OfferedIncompatibleQosStatus* status)	= 0;
		virtual void	on_publication_matched(pDataWriter the_writer, PublicationMatchedStatus* status)			= 0;
	};
}

#endif
