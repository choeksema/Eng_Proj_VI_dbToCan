#ifndef _DB_CLASS_
#define _DB_CLASS_

#include <mysql_connection.h>
#include <cppconn/resultset.h>
#include <string>

using namespace std;

class DB
{
	string query;
	string address;
	string username;
	string password;
	sql::Connection *con;

public:
	DB(string, string, string);

	bool openConnection();
	bool openConnection(string, string, string);
	bool closeConnection();

	bool setSchema(string);
	string getSchema();

	void setQuery(string);
	string getQuery();

	sql::ResultSet* select();
	sql::ResultSet* select(string);
	bool update();
	bool update(string);
};

#endif
