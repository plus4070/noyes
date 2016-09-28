#include "DBManager.h"
#include <stdio.h>

DBManager::DBManager()
{
	resetManager();
}


DBManager::~DBManager()
{
	mysql_close(this->connection);
}


void DBManager::resetManager() {
	//initDBInfo();

	if (!DBConnection()) {
		printf("Connection Error!\n");
	}
	else {
		clearTopicTable();
		defineTopic();
	}
}

void DBManager::clearTopicTable() {
	char query[200];

	sprintf(query, "SELECT * FROM %s", this->table);

	state = executeQuery(query);

	if (state != -1) {
		while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
		{
			removeParticipantTable((char *)sql_row[0]);
			sprintf(query, "DELETE FROM %s WHERE topic='%s'", this->table, sql_row[0]);
			mysql_query(this->connection, (const char *)query);
		}
	}
}

void DBManager::InputTopic(char * topic) {
	//printf("INPUT PARTICIPANT TABLE : %s\n", topic);

	char query[200];

	sprintf(query, "INSERT INTO %s VALUES ( '%s' )", this->table, topic);

	state = executeQuery(query);

	if (state != -1) {
		makeParticipantTable(topic);
	}
}

bool DBManager::isTopicExist(char * topic) {
	char query[200];

	sprintf(query, "SELECT * FROM %s", this->table);

	state = executeQuery(query);

	if (state != -1) {
		while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
		{
			if (strcmp(topic, (char *)sql_row[0]) == 0)
				return true;
		}
	}
	return false;
}

void DBManager::makeParticipantTable(char * topic) {
	//printf("MAKE PARTICIPANT TABLE : %s\n", topic);
	char query[800];
	sprintf(query, "CREATE TABLE `participanttable_%s` (`Domain` VARCHAR(100) NOT NULL, `Type` INT NOT NULL, `IP` VARCHAR(16) NOT NULL,`Port` INT NOT NULL, `Data` VARCHAR(800) NOT NULL);", topic);
	//printf(query);
	mysql_query(this->connection, (const char *)query);
}

void DBManager::removeParticipantTable(char * topic) {
	//printf("REMOVE PARTICIPANT TABLE : %s\n", topic);
	char query[400];
	sprintf(query, "DROP TABLE `participanttable_%s`;", topic);
	//printf(query);
	mysql_query(this->connection, (const char *)query);
}


void DBManager::initDBInfo() {
	int	test_type;

	printf("***** Select inital type *****\n");
	printf("[1] Private Database\n");
	printf("[2] Default \n");
	printf("[others] Exit\n");
	printf("******************************\n");

	printf("input>");
	scanf("%d", &test_type);

	if (test_type == 2) {
		this->host = (char*)"127.0.0.1";
		this->user = (char*)"terminal";
		this->pw = (char*)"tns2458";
		this->db = (char*)"ddsparticipanttable";
	}
	else {
		char host[ADDRESS_SIZE];
		char user[MAX_CHAR];
		char pw[MAX_CHAR];
		char db[MAX_CHAR];
		printf("============ Input Database Info ============\n");

		printf("Input Database IP >");
		scanf("%s", host);

		printf("Input User ID >");
		scanf("%s", user);

		printf("Input Password >");
		scanf("%s", pw);

		printf("Input Database Name >");
		scanf("%s", db);

		/* 
		this->host = (char*)host;
		this->user = (char*)user;
		this->pw = (char*)pw;
		this->db = (char*)db;
		*/
		
		//printf("%s\t%s\t%s\t%s\n", host, user, pw, db);

		this->host = host;
		this->user =user;
		this->pw = pw;
		this->db = db;

		//printf("%s\t%s\t%s\t%s\n", this->host, this->user, this->pw, this->db);
	}

	this->table = TopicTable;
	fflush(stdin);

	printf("================= Complete ==================\n");
}


void DBManager::defineTopic() {
	int test_type;
	char topic[MAX_CHAR];
	printf("[ INITIALIZE TOPIC SPACE ]\n\n");

	printf("***** Select inital type *****\n");
	printf("[1] Create Topic\n");
	printf("[others] Exit\n");
	printf("******************************\n");

	printf("input>");
	scanf("%d", &test_type);
	while (test_type == 1) {
		printf("Input Topic >");
		scanf("%s", topic);
		InputTopic(topic);

		fflush(stdin);

		printf("***** Select inital type *****\n");
		printf("[1] Create Topic\n");
		printf("[others] Exit\n");
		printf("******************************\n");

		printf("input>");
		scanf("%d", &test_type);
	}
}


void DBManager::testDB() {
	puts(mysql_get_client_info());
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

bool DBManager::isEntryExist(PDD_DATA entry) {
	char    query[200];
	sprintf(query, "SELECT * FROM %s WHERE Topic='%s' AND Domain = '%s' AND Type = %d AND IP='%s' AND Port=%d;", this->table, entry.PARTICIPANT_TOPIC, entry.PARTICIPANT_DOMAIN_ID, entry.PARTICIPANT_NODE_TYPE, entry.PARTICIPANT_IP, entry.PARTICIPANT_PORT);

	//printf("IS ENTRY EXIST :: %s\n", query);
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

list<PDD_DATA> DBManager::InsertEntry(PDD_DATA entry) {
	int		state;
	char    query[200];

	sprintf(query, "INSERT INTO	`participanttable_%s` VALUES ('%s', %d, '%s', %d, '%s');", entry.PARTICIPANT_TOPIC, entry.PARTICIPANT_DOMAIN_ID, entry.PARTICIPANT_NODE_TYPE, entry.PARTICIPANT_IP, entry.PARTICIPANT_PORT, entry.PARTICIPANT_DATA);
	//printf("INSERT ENTRY :: %s\n", query);

	state = executeQuery(query);
	
	return selectRelationEntry(entry, state);
}

list<PDD_DATA> DBManager::deleteEntry(PDD_DATA entry) {
	char    query[200];
	sprintf(query, "DELETE FROM participanttable_%s WHERE Domain = '%s' AND Type = %d AND IP='%s' AND Port=%d;", entry.PARTICIPANT_TOPIC, entry.PARTICIPANT_DOMAIN_ID, entry.PARTICIPANT_NODE_TYPE, entry.PARTICIPANT_IP, entry.PARTICIPANT_PORT);

	//printf("DELETE ENTRY :: %s\n", query);

	state = executeQuery(query);

	return selectRelationEntry(entry, state);
}

list<PDD_DATA> DBManager::updateEntry(PDD_DATA entry) {
	char    query[200];
	sprintf(query, "UPDATE participanttable_%s set Data = '%s' WHERE Domain = '%s' AND Type = %d AND IP='%s' AND Port=%d;", entry.PARTICIPANT_TOPIC, entry.PARTICIPANT_DATA,  entry.PARTICIPANT_DOMAIN_ID, entry.PARTICIPANT_NODE_TYPE, entry.PARTICIPANT_IP, entry.PARTICIPANT_PORT);

	//printf("UPDATE ENTRY :: %s\n", query);

	state = executeQuery(query);

	return selectRelationEntry(entry, state);
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

 bool Compare(const PDD_DATA &firstElem, const PDD_DATA &secondElem) {
	return firstElem.PARTICIPANT_IP < secondElem.PARTICIPANT_IP;
}


list<PDD_DATA> DBManager::selectRelationEntry(PDD_DATA entry, int state) {
	char    query[200];
	list<PDD_DATA> sendList;
	IN_ADDR addr;
	PDD_DATA data;

	if (state != -1) {
		sprintf(query, "SELECT * FROM `participanttable_%s` WHERE Domain = '%s' AND Type = %d;",  entry.PARTICIPANT_TOPIC, entry.PARTICIPANT_DOMAIN_ID, entry.PARTICIPANT_NODE_TYPE == NODE_TYPE_PUB ? NODE_TYPE_SUB : NODE_TYPE_PUB);

		//printf("SELECT OPPOSITE ENTRY :: %s\n", query);

		state = executeQuery(query);

		if (state != -1) {
			printf("GET DB DATA \n");
			while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
			{
				printf("%s, %d, %s, %d, %s\n", sql_row[0], atoi(sql_row[1]), sql_row[2], atoi(sql_row[3]), sql_row[4]);
				memset(&data, 0, sizeof(PDD_DATA));
				strcpy(data.PARTICIPANT_TOPIC, entry.PARTICIPANT_TOPIC);
				strcpy(data.PARTICIPANT_DOMAIN_ID, sql_row[0]);
				data.PARTICIPANT_NODE_TYPE = atoi(sql_row[1]);
				strcpy(data.PARTICIPANT_IP, sql_row[2]);
				data.PARTICIPANT_PORT = atoi(sql_row[3]);
				memcpy(data.PARTICIPANT_DATA, sql_row[4], sizeof(sql_row[4]));
				sendList.push_back(data);
			}
			printf("GET DB DATA ------------------------------ \n");
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

void DBManager::makeQuery(PDD_DATA entry, char * query, int queryType) {
	switch (queryType)
	{
	case QUERY_TYPE_INSERT:
		sprintf(query, "INSERT INTO	`participanttable_%s` VALUES ('%s', %d, '%s', %d, '%s');", entry.PARTICIPANT_TOPIC, entry.PARTICIPANT_DOMAIN_ID, entry.PARTICIPANT_NODE_TYPE, entry.PARTICIPANT_IP, entry.PARTICIPANT_PORT, entry.PARTICIPANT_DATA);
		break;
	case QUERY_TYPE_DELETE:
		sprintf(query, "DELETE FROM participanttable_%s WHERE Domain = '%s' AND Type = %d AND IP='%s' AND Port=%d;", entry.PARTICIPANT_TOPIC, entry.PARTICIPANT_DOMAIN_ID, entry.PARTICIPANT_NODE_TYPE, entry.PARTICIPANT_IP, entry.PARTICIPANT_PORT);
		break;
	case QUERY_TYPE_UPDATE:
		sprintf(query, "UPDATE participanttable_%s set Data = '%s' WHERE Domain = '%s' AND Type = %d AND IP='%s' AND Port=%d;", entry.PARTICIPANT_TOPIC, entry.PARTICIPANT_DATA, entry.PARTICIPANT_DOMAIN_ID, entry.PARTICIPANT_NODE_TYPE, entry.PARTICIPANT_IP, entry.PARTICIPANT_PORT);
		break;
	case QUERY_TYPE_SELECT:
		sprintf(query, "SELECT * FROM `participanttable_%s` WHERE Domain = '%s' AND Type = %d;", entry.PARTICIPANT_TOPIC, entry.PARTICIPANT_DOMAIN_ID, entry.PARTICIPANT_NODE_TYPE == NODE_TYPE_PUB ? NODE_TYPE_SUB : NODE_TYPE_PUB);
		break;
	default:
		break;
	}
}