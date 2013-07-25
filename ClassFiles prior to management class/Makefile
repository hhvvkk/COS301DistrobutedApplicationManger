ManagementTest:ManagementTest.o Machine.o Build.o Slave.o Master.o Management.o
	g++ -o ManagementTest ManagementTest.o Machine.o Master.o Build.o Slave.o Management.o

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
	
clean:
	rm *.o