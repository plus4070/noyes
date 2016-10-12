#include "stdafx.h"
#include <mysql.h>
#include <list>
#include <WinSock2.h>
#include <algorithm>

#include <deque>

#pragma comment(lib, "libmySQL.lib")
#pragma once
#define TopicTable "TopicTable"

class DBManager
{
public:
	DBManager();
	~DBManager();
	void resetManager();
	bool DBConnection();

	void clearTopicTable();
	void initDBInfo();
	void defineTopic();

	void InputTopic(char * topic);
	void makeParticipantTable(char  * topic);
	bool isTopicExist(char  * topic);
	void removeParticipantTable(char  * topic);

	bool isEntryExist(PDD_DATA entry);
	list<PDD_DATA> InsertEntry(PDD_DATA entry);
	list<PDD_DATA> deleteEntry(PDD_DATA entry);
	list<PDD_DATA> updateEntry(PDD_DATA entry);
	list<PDD_DATA> selectRelationEntry(PDD_DATA entry, int state);

	void showAllEntry();
	void testDB();

	int  executeQuery(char * query);
	void makeQuery(PDD_DATA entry, char * query, int queryType);



	list<PDD_DATA>	inputQ(PDD_DATA entry);
	bool			isDefinedTopic(PDD_DATA entry);

private:
	MYSQL		*connection;      // the connection
	MYSQL_RES	*sql_result;  // the results
	MYSQL_ROW	sql_row;      // the results row (line by line)

							  // DB Connect Infomation
	const char	*host = (char*)"127.0.0.1";
	const char	*user = (char*)"terminal";
	const char	*pw = (char*)"tns2458";
	const char	*db = (char*)"ddsparticipanttable";
	const char	*table = (char*)"topictable";

	char    *query;
	int     state;


	deque<PDD_DATA> tempQ;
};

