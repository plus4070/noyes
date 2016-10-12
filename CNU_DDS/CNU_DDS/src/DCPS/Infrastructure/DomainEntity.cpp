#include "../../../include/DCPS/Infrastructure/DomainEntity.h"

namespace CNU_DDS
{
	DomainEntity::~DomainEntity(void)
	{

	}

	ReturnCode_t	DomainEntity::set_qos(EntityQos* qos_list)
	{
		ReturnCode_t	result;

		return result;
	}

	ReturnCode_t	DomainEntity::get_qos(EntityQos* qos_list)
	{
		ReturnCode_t	result;

		return result;
	}

	ReturnCode_t	DomainEntity::set_listener(Listener* a_listener, StatusMask mask)
	{
		ReturnCode_t	result;

		return result;
	}

	Listener*		DomainEntity::get_listener(void)
	{
		return	listener;
	}

	ReturnCode_t	DomainEntity::enable(void)
	{
		ReturnCode_t	result;

		return result;
	}
}
