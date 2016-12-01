#ifndef ENTITYID_T_H_
#define ENTITYID_T_H_

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
	class CNU_DDS_DLL_API EntityId_t
	{
	public:
		unsigned char	entity_key[3];
		unsigned char	entity_kind;

	public:
		void	operator =  (const EntityId_t& arg);
		bool	operator == (const EntityId_t& arg);
	};

	const unsigned char	ENTITY_KIND_USER_DEFINED_UNKNOWN			= 0x00;
	const unsigned char	ENTITY_KIND_USER_DEFINED_WRITER_WITH_KEY	= 0x02;
	const unsigned char	ENTITY_KIND_USER_DEFINED_WRITER_NO_KEY		= 0x03;
	const unsigned char	ENTITY_KIND_USER_DEFINED_READER_NO_KEY		= 0x04;
	const unsigned char	ENTITY_KIND_USER_DEFINED_READER_WITH_KEY	= 0x07;

	const unsigned char	ENTITY_KIND_BUILTIN_UNKNOWN					= 0xc0;
	const unsigned char	ENTITY_KIND_BUILTIN_PARTICIPANT				= 0xc1;
	const unsigned char	ENTITY_KIND_BUILTIN_WRITER_WITH_KEY			= 0xc2;
	const unsigned char	ENTITY_KIND_BUILTIN_WRITER_NO_KEY			= 0xc3;
	const unsigned char	ENTITY_KIND_BUILTIN_READER_NO_KEY			= 0xc4;
	const unsigned char	ENTITY_KIND_BUILTIN_READER_WITH_KEY			= 0xc7;

	const EntityId_t	ENTITYID_UNKNOWN								= {{0x00, 0x00, 0x00}, 0x00};
	const EntityId_t	ENTITYID_PARTICIPANT							= {{0x00, 0x00, 0x01}, 0xc1};
	const EntityId_t	ENTITYID_SEDP_BUILTIN_TOPIC_WRITER				= {{0x00, 0x00, 0x02}, 0xc2};
	const EntityId_t	ENTITYID_SEDP_BUILTIN_TOPIC_READER				= {{0x00, 0x00, 0x02}, 0xc7};
	const EntityId_t	ENTITYID_SEDP_BUILTIN_PUBLICATIONS_WRITER		= {{0x00, 0x00, 0x03}, 0xc2};
	const EntityId_t	ENTITYID_SEDP_BUILTIN_PUBLICATIONS_READER		= {{0x00, 0x00, 0x03}, 0xc7};
	const EntityId_t	ENTITYID_SEDP_BUILTIN_SUBSCRIPTIONS_WRITER		= {{0x00, 0x00, 0x04}, 0xc2};
	const EntityId_t	ENTITYID_SEDP_BUILTIN_SUBSCRIPTIONS_READER		= {{0x00, 0x00, 0x04}, 0xc7};
	const EntityId_t	ENTITYID_SPDP_BUILTIN_PARTICIPANT_WRITER		= {{0x00, 0x01, 0x00}, 0xc2};
	const EntityId_t	ENTITYID_SPDP_BUILTIN_PARTICIPANT_READER		= {{0x00, 0x01, 0x00}, 0xc7};
	const EntityId_t	ENTITYID_P2P_BUILTIN_PARTICIPANT_MESSAGE_WRITER	= {{0x00, 0x02, 0x00}, 0xc2};
	const EntityId_t	ENTITYID_P2P_BUILTIN_PARTICIPANT_MESSAGE_READER	= {{0x00, 0x02, 0x00}, 0xc7};

	const EntityId_t	ENTITYID_WRITER_APPLICATIONS					= {{0x00, 0x00, 0x01}, 0xC2};
	const EntityId_t	ENTITYID_READER_APPLICATIONS					= {{0x00, 0x00, 0x01}, 0xC7};
	const EntityId_t	ENTITYID_WRITER_CLIENTS							= {{0x00, 0x00, 0x05}, 0xC2};
	const EntityId_t	ENTITYID_READER_CLIENTS							= {{0x00, 0x00, 0x05}, 0xC7};
	const EntityId_t	ENTITYID_WRITER_SERVICES						= {{0x00, 0x00, 0x06}, 0xC2};
	const EntityId_t	ENTITYID_READER_SERVICES						= {{0x00, 0x00, 0x06}, 0xC7};
	const EntityId_t	ENTITYID_WRITER_MANAGERS						= {{0x00, 0x00, 0x07}, 0xC2};
	const EntityId_t	ENTITYID_READER_MANAGERS						= {{0x00, 0x00, 0x07}, 0xC7};
	const EntityId_t	ENTITYID_WRITER_APPLICATIONS_SELF				= {{0x00, 0x00, 0x08}, 0xC2};
}

#ifdef __cplusplus
}
#endif
#endif
