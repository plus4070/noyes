#ifndef ENTITY_H_
#define ENTITY_H_

#include "Type/BasicDefinitions.h"
#include "Type/ReturnCode_t.h"

#include "../../QoS/QosPolicy/QosPolicy.h"
#include "../../QoS/EntityQos/EntityQos.h"

#include "../../Status/Status/InclusionsOfStatuses.h"
#include "../../Status/Condition/InclusionsOfConditions.h"
#include "../../Status/Condition/StatusCondition.h"
#include "../../Status/Listener.h"
#include "../../Status/EntityStatus/EntityStatus.h"

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
	class CNU_DDS_DLL_API Entity
	{
	public:
		EntityQos*			qos;
		EntityStatus*		status;
		Listener*			listener;
		StatusCondition*	status_condition;
		bool				is_enabled;

	public:
		virtual ~Entity();

		virtual ReturnCode_t	set_qos(EntityQos* qos_list);
		virtual	ReturnCode_t	get_qos(EntityQos* qos_list);

		virtual	ReturnCode_t	set_listener(Listener* a_listener, StatusMask mask);
		virtual	Listener*		get_listener(void);

		virtual	ReturnCode_t	enable(void);

		StatusCondition*		get_status_condition(void);
		StatusMask				get_status_changes(void);

		InstanceHandle_t		get_instance_handle(void);
	};

	typedef	DoublyLinkedList<class CNU_DDS_DLL_API Entity>	EntityList;
}

#ifdef __cplusplus
}
#endif
#endif
