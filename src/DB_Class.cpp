#include "../include/DB_Class.h"
#include <cppconn/driver.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>
#include <iostream>
// cppconn exception in cppconn/exception.h?

using namespace std;


// DB constructor method
DB::DB(string addr, string user, string pswd)
{
	address = addr;
	username = user;
	password = pswd;
}

// ADD LOGGING TO THIS FUNCTION
bool DB::openConnection()
{
	if (address.empty() || username.empty() || password.empty())
		return false;

	try
	{
		sql::Driver* driver = get_driver_instance();
		con = driver->connect(address, username, password);
		return true;
	}
	catch(const std::exception& ex)
	{
		// Add logging
		cout << ex.what() << endl;
		return false;
	}
}

bool DB::openConnection(string addr, string user, string pswd)
{
	address = addr;
	username = user;
	password = pswd;
	return openConnection();
}

bool DB::closeConnection()
{
	try
	{
		delete con;    // Not sure here..
	}
	catch(const exception& ex)
	{
		cout << ex.what() << endl;
		return false;
	}
	return true;
}


bool DB::setSchema(string reqSchema)
{
	try
	{
		con->setSchema(reqSchema);
	}
	catch (const exception& ex)
	{
		cout << ex.what() << endl;
		return false;
	}
	return true;
}

string DB::getSchema()
{
	string schema;
	try
	{
		schema = (string)con->getSchema();
	}
	catch (const exception& ex)
	{
		cout << "getSchema failed: " << ex.what() << endl;
		return "";
	}

	return schema;
}


void DB::setQuery(string reqQuery)
{
	query = reqQuery;
}


string DB::getQuery()
{
	return query;
}


// ADD LOGGING
sql::ResultSet* DB::select()
{
	if (query.empty())
		return nullptr;

	sql::ResultSet* result = nullptr;

	try
	{
		sql::Statement* stmt = con->createStatement();
		result = stmt->executeQuery(query);

		delete stmt;
	}
	catch(const exception& ex)
	{
		cout << ex.what() << endl;
		return nullptr;
	}

	return result; // Delete this pointer?
}

sql::ResultSet* DB::select(string reqQuery)
{
	setQuery(reqQuery);
	return select();
}

bool DB::update()
{
	if (query.empty())
		return false;

	sql::PreparedStatement* pstmt = nullptr;
	bool success = false;

	try
	{
		pstmt = con->prepareStatement(query);
		success = pstmt->executeUpdate();

		delete pstmt;
	}
	catch(const exception& ex)
	{
		cout << ex.what() << endl;
		return false;
	}
	return success;
}

bool DB::update(string reqQuery)
{
	query = reqQuery;
	return update();
}