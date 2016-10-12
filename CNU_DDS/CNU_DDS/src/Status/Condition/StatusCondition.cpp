#include "../../../include/Status/Condition/StatusCondition.h"
#include "../../../include/DCPS/Infrastructure/Entity.h"

namespace CNU_DDS
{
	ReturnCode_t	StatusCondition::set_enabled_statuses(StatusMask mask)
	{
		ReturnCode_t	result	= RETCODE_OK;

		enabled_statuses	= mask;

		return result;
	}

	StatusMask		StatusCondition::get_enabled_statuses(void)
	{
		return enabled_statuses;
	}

	Entity*			StatusCondition::get_entity(void)
	{
		return related_entity;
	}
}
