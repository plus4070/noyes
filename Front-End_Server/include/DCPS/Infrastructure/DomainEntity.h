#ifndef DOMAINENTITY_H_
#define DOMAINENTITY_H_

#include "Entity.h"

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
	class CNU_DDS_DLL_API DomainEntity : public Entity
	{
	public:
		pDomainParticipant	related_participant;

	public:
		virtual ~DomainEntity();

		virtual ReturnCode_t	set_qos(EntityQos* qos_list);
		virtual	ReturnCode_t	get_qos(EntityQos* qos_list);

		virtual	ReturnCode_t	set_listener(Listener* a_listener, StatusMask mask);
		virtual	Listener*		get_listener(void);

		virtual	ReturnCode_t	enable(void);
	};

	typedef	DoublyLinkedList<class CNU_DDS_DLL_API DomainEntity>	DomainEntityList;
}

#ifdef __cplusplus
}
#endif
#endif
