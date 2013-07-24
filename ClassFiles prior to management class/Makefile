MasterWithBuildsTest:MasterWithBuildsTest.o Machine.o Build.o Slave.o Master.o Management.o
	g++ -o MasterWithBuildsTest MasterWithBuildsTest.o Machine.o Master.o Build.o Slave.o
        
MasterTest.o: MasterWithBuildsTest.cpp Machine.h Master.h Build.h Slave.h
	g++ -c MasterWithBuildsTest.cpp

Master.o: Master.cpp Master.h Machine.h Slave.h
	g++ -c Master.cpp

Machine.o: Machine.cpp Machine.h
	g++ -c Machine.cpp
	
Build.o: Build.cpp Build.h
	g++ -c Build.cpp
	
Slave.o: Slave.cpp Slave.h
	g++ -c Slave.cpp
	
Management.o: Management.cpp Management.h
	g++ -c Management.cpp

clean:
	rm *.o