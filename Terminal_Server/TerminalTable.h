#include "stdafx.h"
#include <vector>
#include <list>
#pragma once

#define MAX_CHAR 100
#define EDP_DATA_SIZE 100
#define PDP_DATA_SIZE 100
#define NULL 0

using namespace std;


class TerminalTable {

public:

	TerminalTable();

	void				resetTable();
	bool				isEntryExist(T_ENTRY entry);
	bool				addTopic2(T_ENTRY entry);

	bool				addEntry(T_ENTRY entry);
	bool				addEntryToDomain(T_ENTRY entry);		//<도메인을 엔트리에 등록
	bool				addEntryToTopic(T_ENTRY entry, DOMAIN_ENTRY * domainNode);		//<토픽을 엔트리에 등록
	bool				addEntryToParticipant(T_ENTRY entry, TOPIC_ENTRY * topicNode);		//<참가자를 엔트리에 등록

	bool				deleteTopic(T_ENTRY entry);
	
	bool				isDomainExist(char * domain);
	bool				isTopicExist(char * topic, TOPIC_ENTRY * topicNode);
	int				isParticipantExist(PARTICIPANT_ENTRY participant, list<PARTICIPANT_ENTRY> list);

	void				resetModifyFlag();
	bool				isTableModified();

	int				getAllModifiedData(PDD_DATA dataArray[MAX_PDD_NUMBER]);
	void				test_showAllEntry();

	vector<IN_ADDR>		getAllAddressList();


	PDOMAIN_ENTRY d_head;
	bool	isModifyEntryExist;

private:
	PT_NODE t_head;
};
