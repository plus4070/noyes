#include "../../../include/DCPS/BuiltinEntity/BuiltinPublicationsWriter.h"
#include "../../../include/RTPS/Discovery/SEDPdiscoveredPublicationData.h"
#include "../../../include/RTPS/Behavior/StatefulWriter.h"

namespace CNU_DDS
{
	ReturnCode_t		BuiltinPublicationsWriter::write(DDS_Data* data, InstanceHandle_t handle)
	{
		SEDPdiscoveredPublicationData*	sending_data	= (SEDPdiscoveredPublicationData*)data;

		if(this->related_rtps_writer)
		{
			((StatefulWriter*)this->related_rtps_writer)->new_change(CHANGE_KIND_ALIVE, sending_data->Serialize() ,handle);
		}

		return RETCODE_OK;
	}
}
