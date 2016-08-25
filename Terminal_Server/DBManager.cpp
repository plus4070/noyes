#include "DBManager.h"
#include <stdio.h>

DBManager::DBManager()
{
	resetManager();
	
	if (!DBConnection()) {
		printf("Connection Error!\n");
	}
	//outputTest();
	//modifyTest();
}


DBManager::~DBManager()
{
	mysql_close(this->connection);
}


void DBManager::resetManager() {

}

void DBManager::testDB() {
	puts(mysql_get_client_info());
}

bool DBManager::checkTableExist() {
	return false;
}

bool DBManager::DBConnection() {
	this->connection = mysql_init(NULL);

	if (mysql_real_connect(connection, host, user, pw, db, 3306, NULL, 0) == NULL)
	{
		printf("에러 : %d, %s\n", mysql_errno(connection), mysql_error(connection));
		return false;
	}
	return true;
}

bool DBManager::isEntryExist(T_ENTRY entry) {
	char    query[200];
	sprintf(query, "SELECT * FROM %s WHERE Topic='%s' AND Domain = '%s' AND Type = %d AND IP='%s' AND Port=%d;", this->table, entry.TD_TOPIC, entry.TD_DOMAIN, entry.TD_PUBSUBTYPE, inet_ntoa(entry.TD_PARTICIPANT_IP), entry.TD_PARTICIPANT_PORT);

	printf("IS ENTRY EXIST :: %s\n", query);
	state = mysql_query(this->connection, query);

	if (state != 0)
	{
		printf("ERROR NUMBER\t= %u\n", mysql_errno(this->connection)); // mysql errno 출력
		printf("ERROR MESSAGE\t= %s\n", mysql_error(this->connection)); //mysql error 메세지 출력
	}
	else {
		sql_result = mysql_store_result(this->connection);
		if ((sql_row = mysql_fetch_row(sql_result)) != NULL)
		{
			return true;
		}
	}
	return false;
}

list<IN_ADDR> DBManager::InsertEntry(T_ENTRY entry) {
	int		state;
	char    query[200];
	sprintf(query, "INSERT INTO %s VALUES ('%s', '%s', %d, '%s', %d, '%s');", this->table, entry.TD_TOPIC, entry.TD_DOMAIN, entry.TD_PUBSUBTYPE, inet_ntoa(entry.TD_PARTICIPANT_IP), entry.TD_PARTICIPANT_PORT, entry.TD_DATA);
	printf("INSERT ENTRY :: %s\n", query);

	state = executeQuery(query);
	
	return selectOppositeEntry(entry, state);
}

list<IN_ADDR> DBManager::deleteEntry(T_ENTRY entry) {
	char    query[200];
	sprintf(query, "DELETE FROM %s WHERE Topic='%s' AND Domain = '%s' AND Type = %d AND IP='%s' AND Port=%d;", this->table, entry.TD_TOPIC, entry.TD_DOMAIN, entry.TD_PUBSUBTYPE, inet_ntoa(entry.TD_PARTICIPANT_IP), entry.TD_PARTICIPANT_PORT);

	printf("DELETE ENTRY :: %s\n", query);

	state = executeQuery(query);

	return selectOppositeEntry(entry, state);
}

list<IN_ADDR> DBManager::updateEntry(T_ENTRY entry) {
	char    query[200];
	sprintf(query, "UPDATE %s set ParticipantData = '%s' WHERE Topic='%s' AND Domain = '%s' AND Type = %d AND IP='%s' AND Port=%d;", this->table, entry.TD_DATA, entry.TD_TOPIC, entry.TD_DOMAIN, entry.TD_PUBSUBTYPE, inet_ntoa(entry.TD_PARTICIPANT_IP), entry.TD_PARTICIPANT_PORT);

	printf("UPDATE ENTRY :: %s\n", query);

	state = executeQuery(query);

	return selectOppositeEntry(entry, state);
}

void DBManager::showAllEntry() {
	char    query[200];
	sprintf(query, "SELECT * FROM %s;", this->table);
	printf("SHOW ALL ENTRY :: %s\n", query);
	state = mysql_query(this->connection, query);

	if (state != 0)
	{
		printf("ERROR NUMBER\t= %u\n", mysql_errno(this->connection)); // mysql errno 출력
		printf("ERROR MESSAGE\t= %s\n", mysql_error(this->connection)); //mysql error 메세지 출력
	}
	else {
		sql_result = mysql_store_result(this->connection);
		while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
		{
			printf("%s\t%s\t%d\t%s\t%d\t%s\n", sql_row[0], sql_row[1], atoi(sql_row[2]), sql_row[3], atoi(sql_row[4]), sql_row[5]);
		}
	}
}

 bool Compare(const IN_ADDR &firstElem, const IN_ADDR &secondElem) {
	return firstElem.S_un.S_addr < secondElem.S_un.S_addr;
}


list<IN_ADDR> DBManager::selectOppositeEntry(T_ENTRY entry, int state) {
	char    query[200];
	list<IN_ADDR> sendList;
	IN_ADDR addr;

	if (state != -1) {
		sprintf(query, "SELECT IP FROM %s WHERE Topic='%s' AND Domain = '%s' AND Type = %d;", this->table, entry.TD_TOPIC, entry.TD_DOMAIN, entry.TD_PUBSUBTYPE == NODE_TYPE_PUB ? NODE_TYPE_SUB : NODE_TYPE_PUB);

		printf("SELECT OPPOSITE ENTRY :: %s\n", query);

		state = executeQuery(query);

		if (state != -1) {
			while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
			{
				addr.S_un.S_addr = inet_addr(sql_row[0]);
				sendList.push_back(addr);
			}
		}
	}

	sendList.sort(Compare);

	return sendList;
}

int	DBManager::executeQuery(char * query) {
	int state = mysql_query(this->connection, (const char *)query);

	if (state != 0)
	{
		printf("ERROR NUMBER\t= %u\n", mysql_errno(this->connection)); // mysql errno 출력
		printf("ERROR MESSAGE\t= %s\n", mysql_error(this->connection)); //mysql error 메세지 출력
		state = -1;
	}
	else {
		this->sql_result =  mysql_store_result(this->connection);
	}

	return state;
}