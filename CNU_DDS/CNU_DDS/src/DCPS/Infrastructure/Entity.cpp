#include "../../../include/DCPS/Infrastructure/Entity.h"

namespace CNU_DDS
{
	Entity::~Entity(void)
	{

	}

	ReturnCode_t	Entity::set_qos(EntityQos* qos_list)
	{
		ReturnCode_t	result;

		return result;
	}

	ReturnCode_t	Entity::get_qos(EntityQos* qos_list)
	{
		ReturnCode_t	result;

		return result;
	}

	ReturnCode_t	Entity::set_listener(Listener* a_listener, StatusMask mask)
	{
		ReturnCode_t	result;

		return result;
	}

	Listener*		Entity::get_listener(void)
	{
		return	listener;
	}

	ReturnCode_t	Entity::enable(void)
	{
		ReturnCode_t	result;

		return result;
	}

	StatusCondition*		Entity::get_status_condition(void)
	{
		StatusCondition*	result;

		result	= status_condition;

		return result;
	}

	StatusMask				Entity::get_status_changes(void)
	{
		StatusMask	result	= 0x00000000;

		return result;
	}

	InstanceHandle_t		Entity::get_instance_handle(void)
	{
		InstanceHandle_t	result;

		result	= (InstanceHandle_t)this;

		return result;
	}
}
