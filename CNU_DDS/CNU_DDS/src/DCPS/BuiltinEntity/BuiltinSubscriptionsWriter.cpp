#include "../../../include/DCPS/BuiltinEntity/BuiltinSubscriptionsWriter.h"

#include "../../../include/RTPS/Discovery/SEDPdiscoveredSubscriptionData.h"
#include "../../../include/RTPS/Behavior/StatefulWriter.h"

namespace CNU_DDS
{
	ReturnCode_t		BuiltinSubscriptionsWriter::write(DDS_Data* data, InstanceHandle_t handle)
	{
		SEDPdiscoveredSubscriptionData*	sending_data	= (SEDPdiscoveredSubscriptionData*)data;

		if(this->related_rtps_writer)
		{
			((StatefulWriter*)this->related_rtps_writer)->new_change(CHANGE_KIND_ALIVE, sending_data->Serialize() ,handle);
		}

		return RETCODE_OK;
	}
}
