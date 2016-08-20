#pragma once
#include <WinSock2.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <list>

#define MAX_CHAR		100
#define EDP_DATA_SIZE	1000
#define PDP_DATA_SIZE	1000
#define MAX_DATA_SIZE	1000
#define MAX_PDD_NUMBER	25
#define NULL			0

#define ADDRESS_SIZE	16

#define STATE_NEW		0
#define STATE_SET		1
#define STATE_RESPONSE	2
#define	STATE_END		3


#define MESSAGE_TYPE_SAVE		1
#define MESSAGE_TYPE_REQUEST	2
#define MESSAGE_TYPE_RESPONSE	3
#define MESSAGE_TYPE_NOTEXIST	4
#define MESSAGE_TYPE_SAVEDONE	5
#define NODE_TYPE_PUB			100
#define NODE_TYPE_SUB			200
#define BUFSIZE					100		// 받아올 데이터 최대 크기
#define	FES_PORT				3000
#define TERMINAL_PORT			3002	// 포트번호 할당
#define DDS_PORT				4000

// TCPSocket
typedef struct _TNSN_ENTRY {
	int				TNSN_ID;
	char			TNSN_TOPIC[MAX_CHAR];
	char			TNSN_TOKEN[MAX_CHAR];
	int				TNSN_TOKENLEVEL;
	int				TNSN_NODETYPE;
	int				TNSN_DATATYPE;
	int				TNSN_DATASIZE;
	char			TNSN_DATA[MAX_CHAR];
} TNSN_ENTRY, *PTNSN_ENTRY;

// TopicTable
typedef struct _T_ENTRY {
	char			TD_DOMAIN[MAX_CHAR];
	char			TD_TOPIC[MAX_CHAR];
	char			TD_TOKEN[MAX_CHAR];
	int				TD_LEVEL;
	char			TD_DATA[PDP_DATA_SIZE];
	IN_ADDR			TD_PARTICIPANT_IP;
	bool			TD_CHANGE_FLAG;
} T_ENTRY, *PT_ENTRY;

typedef struct _T_NODE {
	T_ENTRY			key;
	struct _T_NODE	*next;
} T_NODE, *PT_NODE;

typedef struct _PARTICIPANT_ENTRY {
	char					TD_DATA[PDP_DATA_SIZE];
	IN_ADDR					TD_PARTICIPANT_IP;
	bool					TD_CHANGE_FLAG;
} PARTICIPANT_ENTRY, *PPARTICIPANT_ENTRY;

typedef struct _TOPIC_ENTRY {
	char					TD_TOPIC[MAX_CHAR];
	int						TD_LEVEL;
	bool					TD_CHANGE_FLAG;
	int						NumOfUnderEntry;
	list<PARTICIPANT_ENTRY> PARTICIPANT_LIST;
	struct _TOPIC_ENTRY		*next;
} TOPIC_ENTRY, *PTOPIC_ENTRY;

typedef struct _DOMAIN_ENTRY {
	char					TD_DOMAIN[MAX_CHAR];
	bool					TD_CHANGE_FLAG;
	int						NumOfUnderEntry;
	struct _DOMAIN_ENTRY	*next;
} DOMAIN_ENTRY, *PDOMAIN_ENTRY;


// RequestTable
typedef struct _R_ENTRY {
	TNSN_ENTRY			REQUEST_DATA;
	IN_ADDR				REQUEST_IP;
} R_ENTRY, *PR_ENTRY;

typedef struct _R_NODE {
	R_ENTRY				key;
	struct _R_NODE		*next;
} R_NODE, *PR_NODE;


// participantDataDistributor
typedef struct _PDD_HEADER {
	char				PARTICIPANT_DOMAIN_ID[MAX_CHAR];
	int					PARTICIPANT_DOMAIN_SIZE;
	int					PARTICIPANT_NODE_TYPE;
	int					PARTICIPANT_NUMBER_OF_DATA;
} PDD_HEADER, *PPDD_HEADER;

typedef struct _PDD_DATA {
	char				PARTICIPANT_TOPIC[MAX_CHAR];
	char 				PARTICIPANT_DATA[MAX_DATA_SIZE];
} PDD_DATA, *PPDD_DATA;

typedef struct _PDD_NODE {
	PDD_HEADER			PDD_HEADER;
	PDD_DATA 			PDD_DATA[MAX_PDD_NUMBER];
} PDD_NODE, *PPDD_NODE;
