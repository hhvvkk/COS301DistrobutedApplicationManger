#ifndef BUILD_H
#define BUILD_H

#include <string>

using namespace std;

class Build{
	private:
		int buildID;
		string buildName;
		string buildDescription;
	public:
		Build(int id, string name, string descript);
		~Build();
		Build();
		void setBuild(int id, string name, string descript);
		int getBuildID() {return buildID;};
		string getBuildName() {return buildName;};
		string getBuildDescription() {return buildDescription;};
		void setBuildID(int id);
		void setBuildName(string name);
		void setBuildDescription(string descript);
		void printBuild();
};


#endif // BUILD_H