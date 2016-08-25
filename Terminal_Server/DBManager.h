#include "stdafx.h"
#include <mysql.h>
#include <list>
#include <WinSock2.h>
#include <algorithm>

#pragma comment(lib, "libmySQL.lib")

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
	const char	*table	= (char*)"topicnamespace";
	
	char    *query;
	int     state;

public:
	DBManager();
	~DBManager();
	void resetManager();
	bool checkTableExist();
	bool DBConnection();

	bool isEntryExist(T_ENTRY entry);
	list<IN_ADDR> InsertEntry(T_ENTRY entry);
	list<IN_ADDR> deleteEntry(T_ENTRY entry);
	list<IN_ADDR> updateEntry(T_ENTRY entry);
	list<IN_ADDR> selectOppositeEntry(T_ENTRY entry, int state);

	void showAllEntry();
	void testDB();

	int executeQuery(char * query);
};

