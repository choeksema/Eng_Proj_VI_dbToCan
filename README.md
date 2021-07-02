# Eng_Proj_VI_dbToCan

Implements the database to CAN bus communication for the elevator project (ESE Engineering Project VI Elevator). 
Currently the repository contains preliminary code. For example, the floor is not checked before updating the database with the new floor value. However, if you start the process on the server, the elevator can be controlled using the website.

The code also will update the elevatorStats table in the database. It adds 1 to the floor value every time an elevator is sucessfully sent to a new elevator floor (it does it after updating the current position in the database).![image](https://user-images.githubusercontent.com/56086870/124214760-bf1f2f00-dac0-11eb-9b0d-f7c29e14f60b.png)


To run, navigate the directory (~/Desktop/DB-CAN/ on the server). Run "make". Run "./dbTesting". In order to reset the statistics database, run "./dbTesting reset". Then you can use the "inside elevevator" floor send buttons. The "outside elevator" call buttons don't work yet. The FSM logic is not fully implemented yet. (If running locally, you may need to add an "obj" folder in the top level directory of the local repos as the makefile probably won't make one.)

Eventually, it would be nice to add a logging feature to save all database queries, writes, errors, warnings, or anything that could be useful to a file.

This repository is the property of the SmoothBrains.
