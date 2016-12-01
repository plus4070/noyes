#ifndef DATAWRITERLISTENER_H_
#define DATAWRITERLISTENER_H_

#include "../../Status/Listener.h"
#include "../Infrastructure/Type/BasicDefinitions.h"
#include "../../Status/Status/InclusionsOfStatuses.h"

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
	class CNU_DDS_DLL_API DataWriterListener : public Listener
	{
	public:
		virtual void	on_liveliness_lost(pDataWriter the_wirter, LivelinessLostStatus* status)					= 0;
		virtual void	on_offered_deadline_missed(pDataWriter the_writer, OfferedDeadlineMissedStatus* status)		= 0;
		virtual void	on_offered_incompatible_qos(pDataWriter the_writer, OfferedIncompatibleQosStatus* status)	= 0;
		virtual void	on_publication_matched(pDataWriter the_writer, PublicationMatchedStatus* status)			= 0;
	};
}

#ifdef __cplusplus
}
#endif
#endif
