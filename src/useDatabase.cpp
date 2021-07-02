/*
 * File managing database interactions on the server
 * 
 * Smoothbrains July 2021, Elevator Project
 * 		Caleb Hoeksema, Greg Huras, Andrew Sammut
 */

#include <iostream>
#include <sstream>
#include "../include/useDatabase.h"
// #include <cppconn/resultset.h>

using namespace std;

bool getFloorValues(int* curFloor, int* reqFloor)
{
	// Set to default output value
	*curFloor = 0;
	*reqFloor = 0;

	// Initialize the database
	DB database = DB(ADDRESS, USERNAME, PASSWORD);

	if (!database.openConnection())
	{
		cout << "Failed opening database. Please check credentials." << endl;
		return false;
	}

	if (!database.setSchema(SCHEMA))
	{
		cout << "Failed selecting schema. Please check if table exists." << endl;
		return false;
	}

	// Generate a query
	stringstream ss;
	ss << "SELECT " << COL_CUR_FLOOR << "," << COL_REQ_FLOOR << " FROM " << ELEVATOR_TABLE << " WHERE " << COL_NODE_ID << "=" << ID_CAR_NODE;
	database.setQuery(ss.str());

	sql::ResultSet* results = database.select();

	if (nullptr == results)
	{
		cout << "Current floor not found." << endl;
		return false;
	}

	while(results->next())
	{
		try
		{
			*curFloor = results->getInt(COL_CUR_FLOOR);
			*reqFloor = results->getInt(COL_REQ_FLOOR);
		}
		catch(const exception& ex)
		{
			cout << "Failed interpreting query results: " << ex.what() << endl;
			return false;
		}
	}

	if(!database.closeConnection())
	{
		cout << "Failed closing database." << endl;
		return false;
	}

	return true;
}


bool setReqFloor(int curFloor)
{
	// Initialize the database
	DB database = DB(ADDRESS, USERNAME, PASSWORD);

	if (!database.openConnection())
	{
		cout << "Failed opening database. Please check credentials." << endl;
		return false;
	}

	if (!database.setSchema(SCHEMA))
	{
		cout << "Failed selecting schema. Please check if database exists." << endl;
		return false;
	}

	// Generate a query
	stringstream ss;
	ss << "UPDATE " << ELEVATOR_TABLE << " SET " << COL_CUR_FLOOR << "=" << curFloor << " WHERE " << COL_NODE_ID << "=" << ID_CAR_NODE;
	database.setQuery(ss.str());

	if(!database.update())
	{
		cout << "Failed updating the database" << endl;
		return false;
	}

	if(!database.closeConnection())
	{
		cout << "Failed closing the database" << endl;
		return false;
	}
	
	if(!updateElevatorStats(curFloor))
	{
		cout << "Failed updating stats database" << endl;
	}

	return true;
}


bool resetElevatorStats()
{
	DB db = DB(ADDRESS, USERNAME, PASSWORD);
	
	if (!db.openConnection())
	{
		cout << "Failed opening database. Please check credentials." << endl;
		return false;
	}
	
	if (!db.setSchema(SCHEMA))
	{
		cout << "Failed selecting schema. Please check if database exists." << endl;
		return false;
	}
	
	stringstream ss;
	ss << "UPDATE " << STATS_TABLE << " SET " 
	   << COL_STAT_FLOOR_1 << "=DEFAULT," 
	   << COL_STAT_FLOOR_2 << "=DEFAULT,"
	   << COL_STAT_FLOOR_3 << "=DEFAULT,"
	   << COL_STAT_DATETIME << "=NOW()";
	db.setQuery(ss.str());
	
	if(!db.update())
	{
		cout << "Failed updating the database" << endl;
		return false;
	}

	if(!db.closeConnection())
	{
		cout << "Failed closing the database" << endl;
		return false;
	}
	
	return true;
}


bool updateElevatorStats(int floor)
{
	string colName = "";
	int currentFloorCount = 0;
	switch (floor)
	{
		case 1:
			colName = COL_STAT_FLOOR_1;
			break;
		case 2:
			colName = COL_STAT_FLOOR_2;
			break;
		case 3:
			colName = COL_STAT_FLOOR_3;
			break;
		default:
			cout << "The requested floor doesn't exist" << endl;
			return false;
	}
	
	DB db = DB(ADDRESS, USERNAME, PASSWORD);
	
	if (!db.openConnection())
	{
		cout << "Failed opening database. Please check credentials." << endl;
		return false;
	}
	
	if (!db.setSchema(SCHEMA))
	{
		cout << "Failed selecting schema. Please check if database exists." << endl;
		return false;
	}
	
	// get current value
	stringstream ss;
	ss << "SELECT " << colName << " FROM " << STATS_TABLE;
	db.setQuery(ss.str());

	sql::ResultSet* results = db.select();

	if (nullptr == results)
	{
		cout << "Current floor not found." << endl;
		return false;
	}
	/*else if(results->getRow() != 1)
	{
		cout << "Unexpected number of records (" << results->getRow() << ")." << endl;
		cout << colName << endl;
		return false;
	}*/

	while(results->next())
	{
		try
		{
			currentFloorCount = results->getInt(colName);
		}
		catch(const exception& ex)
		{
			cout << "Failed interpreting query results: " << ex.what() << endl;
			return false;
		}
	}
	
	// update value
	ss.str("");
	ss.clear();
	ss << "UPDATE " << STATS_TABLE << " SET " << colName << "=" << (currentFloorCount + 1);
	db.setQuery(ss.str());
	
	if(!db.update())
	{
		cout << "Failed updating the database" << endl;
		return false;
	}

	if(!db.closeConnection())
	{
		cout << "Failed closing the database" << endl;
		return false;
	}
	
	return true;
}
