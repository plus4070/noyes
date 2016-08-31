#pragma once
#include <WinSock2.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <list>

#define MAX_CHAR		100
#define MAX_DATA_SIZE	800
#define MAX_PDD_NUMBER	25
#define BUFSIZE			1000
#define NULL			0

#define ADDRESS_SIZE	16

#define STATE_NEW		0
#define STATE_SET		1
#define STATE_RESPONSE	2
#define	STATE_END		3


#define MESSAGE_TYPE_REQUEST	1
#define MESSAGE_TYPE_RESPONSE	2
#define MESSAGE_TYPE_NOTEXIST	3

#define MESSAGE_TYPE_SAVE		10
#define MESSAGE_TYPE_REMOVE		11
#define MESSAGE_TYPE_MODIFY		12

#define MESSAGE_TYPE_SAVEDONE	20
#define MESSAGE_TYPE_REMOVEDONE	21
#define MESSAGE_TYPE_MODIFYDONE	22

#define NODE_TYPE_PUB			100
#define NODE_TYPE_SUB			200

#define	FES_PORT				3000
#define TERMINAL_PORT			3002	// 포트번호 할당
#define DDS_PORT				4000

using namespace std;

// participantDataDistributor
typedef struct _PDD_HEADER {
	int					MESSAGE_TYPE;
	int					NUMBER_OF_PARTICIPANT;
} PDD_HEADER, *PPDD_HEADER;

typedef struct _PDD_DATA {
	int					PARTICIPANT_NODE_TYPE;
	char				PARTICIPANT_TOPIC[MAX_CHAR];
	char				PARTICIPANT_DOMAIN_ID[MAX_CHAR];
	char				PARTICIPANT_IP[ADDRESS_SIZE];
	int					PARTICIPANT_PORT;
	char 				PARTICIPANT_DATA[MAX_DATA_SIZE];
} PDD_DATA, *PPDD_DATA;

typedef struct _PDD_NODE {
	PDD_HEADER			PDD_HEADER;
	PDD_DATA 			PDD_DATA[MAX_PDD_NUMBER];
} PDD_NODE, *PPDD_NODE;


// TopicTable 최상위만 씀
typedef struct _T_ENTRY {
	char			TD_DOMAIN[MAX_CHAR];
	char			TD_TOPIC[MAX_CHAR];
	int				TD_PUBSUBTYPE;
	IN_ADDR			TD_PARTICIPANT_IP;
	int				TD_PARTICIPANT_PORT;
	char			TD_DATA[MAX_DATA_SIZE];
} T_ENTRY, *PT_ENTRY;

typedef struct _T_NODE {
	T_ENTRY			key;
	struct _T_NODE	*next;
} T_NODE, *PT_NODE;

typedef struct _PARTICIPANT_ENTRY {
	IN_ADDR					TD_PARTICIPANT_IP;
	char					TD_DATA[MAX_DATA_SIZE];
	bool					TD_CHANGE_FLAG;
	_PARTICIPANT_ENTRY		*next;
} PARTICIPANT_ENTRY, *PPARTICIPANT_ENTRY;

typedef struct _TOPIC_ENTRY {
	char					TD_TOPIC[MAX_CHAR];
	bool					TD_CHANGE_FLAG_OF_PUB;
	bool					TD_CHANGE_FLAG_OF_SUB;
	PPARTICIPANT_ENTRY		PUB_PARTICIPANT_LIST;
	PPARTICIPANT_ENTRY		SUB_PARTICIPANT_LIST;
	struct _TOPIC_ENTRY		*next;
} TOPIC_ENTRY, *PTOPIC_ENTRY;

typedef struct _DOMAIN_ENTRY {
	char					TD_DOMAIN[MAX_CHAR];
	bool					TD_CHANGE_FLAG;
	PTOPIC_ENTRY			TOPIC;
	struct _DOMAIN_ENTRY	*next;
} DOMAIN_ENTRY, *PDOMAIN_ENTRY;

// RequestTable
typedef struct _R_ENTRY {
	PDD_NODE			REQUEST_DATA;
	IN_ADDR				REQUEST_IP;
} R_ENTRY, *PR_ENTRY;

typedef struct _R_NODE {
	R_ENTRY				key;
	struct _R_NODE		*next;
} R_NODE, *PR_NODE;


