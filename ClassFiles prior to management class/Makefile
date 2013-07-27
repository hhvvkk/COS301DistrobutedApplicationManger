ManagementTest:ManagementTest.o Management.o Master.o Slave.o Machine.o Build.o
	g++ -o ManagementTest ManagementTest.o Management.o Master.o Slave.o Machine.o Build.o

ManagementTest.o : ManagementTest.cpp Management.h Master.h Machine.h Slave.h Build.h
	g++ -c ManagementTest.cpp

Management.o: Management.cpp Management.h Master.h Machine.h Slave.h Build.h
	g++ -c Management.cpp	

Master.o: Master.cpp Master.h Machine.h Slave.h Build.h
	g++ -c Master.cpp
	
Slave.o: Slave.cpp Slave.h Build.h
	g++ -c Slave.cpp
	
Build.o: Build.cpp Build.h
	g++ -c Build.cpp

Machine.o: Machine.cpp Machine.h
	g++ -c Machine.cpp
	
BuildTest: BuildTest.o Build.o Build.h
	g++ -o BuildTest BuildTest.o Build.o 
        
BuildTest.o: BuildTest.cpp Build.h
	g++ -c BuildTest.cpp
	
MasterWithBuildsTest:MasterWithBuildsTest.o Master.o Slave.o Machine.o Build.o
	g++ -o MasterWithBuildsTest MasterWithBuildsTest.o Master.o Slave.o Machine.o Build.o
	
MasterWithBuildsTest.o: MasterWithBuildsTest.cpp Machine.h Master.h Build.h Slave.h
	g++ -c MasterWithBuildsTest.cpp

MachineTest: MachineTest.o Master.o Slave.o Machine.o Build.o
	g++ -o MachineTest MachineTest.o Master.o Slave.o Machine.o Build.o
	
MachineTest.o: MachineTest.cpp Machine.h Master.h Slave.h Build.h
	g++ -c MachineTest.cpp
	
clean:
	rm *.o
	rm *.exe