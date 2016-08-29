#include "stdafx.h"
#include <mysql.h>
#include <list>
#include <WinSock2.h>
#include <algorithm>

#pragma comment(lib, "libmySQL.lib")

#define TopicTable "TopicTable"

class DBManager
{
private:
	MYSQL		*connection;      // the connection
	MYSQL_RES	*sql_result;  // the results
	MYSQL_ROW	sql_row;      // the results row (line by line)

	const char	*host	= (char*)"127.0.0.1";
	const char	*user	= (char*)"terminal";
	const char	*pw		= (char*)"tns2458";
	const char	*db		= (char*)"ddsparticipanttable";
	const char	*table	= (char*)"topictable";

	char    *query;
	int     state;

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

	bool isEntryExist(T_ENTRY entry);
	list<PDD_DATA> InsertEntry(T_ENTRY entry);
	list<PDD_DATA> deleteEntry(T_ENTRY entry);
	list<PDD_DATA> updateEntry(T_ENTRY entry);
	list<PDD_DATA> selectRelationEntry(T_ENTRY entry, int state);

	void showAllEntry();
	void testDB();

	int executeQuery(char * query);
};

