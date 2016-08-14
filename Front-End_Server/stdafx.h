#pragma once

#include <WinSock2.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#define MAX_CHAR		100
#define EDP_DATA_SIZE	10000
#define PDP_DATA_SIZE	10000
#define MAX_DATA_SIZE	10000
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
#define PORT					3000		// 포트번호 할당
#define TNS_PORT				3001
#define TERMINAL_PORT			3002

#define IP						"127.0.0.1"


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

//TopicNameSpaceTable
typedef struct _TNSP_ENTRY {
	int					TN_SPACE_ID;

	char				TN_SPACE_TOPIC[MAX_CHAR];
	char				TN_SPACE_TOKEN[MAX_CHAR];

	int					TN_SPACE_TOTAL_LEVEL;
	int					TN_SPACE_CURRENT_LEVEL;
	
	int					TN_SPACE_STATE;
	
	SOCKET				TN_SPACE_CURRENT_SOCKET;
	char				TN_SPACE_NEXTZONE_ADDR[ADDRESS_SIZE];
	
	char				TN_SPACE_DATA[MAX_DATA_SIZE];
} TNSP_ENTRY, *PTNSP_ENTRY;

typedef struct _RTNSP_NODE {
	TNSP_ENTRY			key;
	struct _RTNSP_NODE	*next;
} TNSP_NODE, *PTNSP_NODE;

// RequestTable
typedef struct _R_ENTRY {
	SOCKET				REQUEST_SOCEKT;
	TNSN_ENTRY			REQUEST_DATA;
} R_ENTRY, *PR_ENTRY;

typedef struct _R_NODE {
	R_ENTRY				key;
	struct _R_NODE		*next;
} R_NODE, *PR_NODE;