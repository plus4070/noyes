#ifndef INCLUDE_DCPS_BUILTINENTITY_BUILTINPUBLICATIONSWRITERLISTENER_H_
#define INCLUDE_DCPS_BUILTINENTITY_BUILTINPUBLICATIONSWRITERLISTENER_H_

#include "../Publication/DataWriterListener.h"

namespace CNU_DDS
{
	class BuiltinPublicationsWriterListener: public DataWriterListener
	{
	public:
		BuiltinPublicationsWriterListener();
		~BuiltinPublicationsWriterListener();

		void	on_liveliness_lost(pDataWriter the_wirter, LivelinessLostStatus* status);
		void	on_offered_deadline_missed(pDataWriter the_writer, OfferedDeadlineMissedStatus* status);
		void	on_offered_incompatible_qos(pDataWriter the_writer, OfferedIncompatibleQosStatus* status);
		void	on_publication_matched(pDataWriter the_writer, PublicationMatchedStatus* status);
	};
}

#endif /* INCLUDE_DCPS_BUILTINENTITY_BUILTINPUBLICATIONSWRITERLISTENER_H_ */
