/*
 * Mainline file for DB-Elevator communications management
 * 
 * Smoothbrains July 2021, Elevator Project
 * 		Caleb Hoeksema, Greg Huras, Andrew Sammut
 */

#include <iostream>
#include <unistd.h>

#include "../include/useDatabase.h"
#include "../include/useCanBus.h"

using namespace std;

int main(int argc, char **argv)
{
	string rst = "reset";	
	if ((2 == argc) && (rst.compare(argv[1]) == 0))
	{
		if (resetElevatorStats())
		{
			cout << "Elevator stats reset" << endl;
		}
	}
	
	cout << "Welcome to the thing is supposed to do magical database"
		 << " and CAN stuff as it listens to the website" << endl;
	cout << "Disclaimer: requesting the elevator to move while it is"
		 << " already moving could lead to problems as the logic is"
		 << " not fully implemented (so monitor with the"
		 << " liveview)" << endl;
	
	
	while(1)
	{
		/*string userIn = "";
		cout << "Continue?";
		cin >> userIn;*/

		int curFloor = 0;
		int reqFloor = 0;

		// Check if the database is up to date
		if(!getFloorValues(&curFloor, &reqFloor))
		{
			cout << "Something went wrong while getting floor numbers." << endl;
			return (1);
		}
		// cout << "Current floor: " << curFloor << endl;
		// cout << "Requested floor: " << reqFloor << endl;

		if(curFloor != reqFloor)
		{
			// Send a PCAN command to move the elevator to the requested floor
			if (!sendToFloor(reqFloor))
			{
				cout << "Something went wrong while moving the elevator" << endl;
				return (2);
			}

			// Update the database on sucessful completion
			// TODO: should update the elevator only once it reaches the new floor
			// TODO: count overall errors in stats DB?

			if(!setReqFloor(reqFloor))
			{
				cout << "Something went wrong while setting floor number." << endl;
				return (3);
			}
		}
	}
	
	cout << "Exiting since something went wrong." << endl;
	
	return(0);
}
