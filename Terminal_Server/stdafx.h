#pragma once
#include <WinSock2.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <list>

#define MAX_CHAR		100
#define MAX_DATA_SIZE	800
#define MAX_PDD_NUMBER	1
#define BUFSIZE			1000
#define NULL			0

#define ADDRESS_SIZE	16

#define STATE_NEW		0
#define STATE_SET		1
#define STATE_RESPONSE	2
#define	STATE_END		3


#define MESSAGE_TYPE_REQUEST		1
#define MESSAGE_TYPE_RESPONSE		2
#define MESSAGE_TYPE_NOTEXIST		3

#define MESSAGE_TYPE_SAVE			10
#define MESSAGE_TYPE_REMOVE			11
#define MESSAGE_TYPE_MODIFY			12

#define MESSAGE_OPTION_PLUS_DONE	10

#define MESSAGE_TYPE_SAVEDONE		20
#define MESSAGE_TYPE_REMOVEDONE		21
#define MESSAGE_TYPE_MODIFYDONE		22


#define NODE_TYPE_PUB				100 //Writer
#define NODE_TYPE_SUB				200 //Reader

#define	FES_PORT					3000
#define TERMINAL_PORT				3002	// 포트번호 할당
#define DDS_PORT					3000

#define QUERY_TYPE_INSERT			1
#define QUERY_TYPE_DELETE			2
#define QUERY_TYPE_UPDATE			3
#define QUERY_TYPE_SELECT			4


using namespace std;

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


