#pragma once
#include <WinSock2.h>

#define MAX_CHAR		100
#define MAX_DATA_SIZE	800
#define MAX_PDD_NUMBER  1
#define NULL			0

#define	ADDRESS_SIZE	16

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
#define BUFSIZE					100		// 받아올 데이터 최대 크기
#define FES_PORT				3000		// 포트번호 할당
#define TNS_PORT				3001
#define TERMINAL_PORT			3002

#define IP						"127.0.0.1"

// TopicNameTable
typedef struct _TN_ENTRY {
	char				TN_TOPIC[MAX_CHAR];
	char				TN_TOKEN[MAX_CHAR];
	int					TN_LEVEL;
	char				TN_NEXTZONE[ADDRESS_SIZE];
} TN_ENTRY, *PTN_ENTRY;

typedef struct _TN_NODE {
	TN_ENTRY			key;
	struct _TN_NODE		*next;
} TN_NODE, *PTN_NODE;


// TCPSocket
typedef struct _PDD_HEADER {
	int					ID;
	int					MESSAGE_TYPE;
} PDD_HEADER, *PPDD_HEADER;

typedef struct _PDD_DATA {
	int					PARTICIPANT_NODE_TYPE;
	char				PARTICIPANT_TOPIC[MAX_CHAR];
	char				PARTICIPANT_TOPICTYPE[MAX_CHAR];
	char				PARTICIPANT_DOMAIN_ID[MAX_CHAR];
	char				PARTICIPANT_IP[ADDRESS_SIZE];
	int					PARTICIPANT_PORT;
	unsigned char		PARTICIPANT_DATA_ORDERING_TYPE;
	unsigned char 		PARTICIPANT_DATA[MAX_DATA_SIZE];
	int					PARTICIPANT_LENGTH_OF_DATA;
} PDD_DATA, *PPDD_DATA;

typedef struct _PDD_NODE {
	PDD_HEADER			PDD_HEADER;
	PDD_DATA 			PDD_DATA;
} PDD_NODE, *PPDD_NODE;


// RequestTable
typedef struct _R_ENTRY {
	PDD_NODE			REQUEST_DATA;
	IN_ADDR				REQUEST_IP;
} R_ENTRY, *PR_ENTRY;

typedef struct _R_NODE {
	R_ENTRY			key;
	struct _R_NODE	*next;
} R_NODE, *PR_NODE;
