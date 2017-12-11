#include "EasyCrossPlatform.h"
#include <iostream>

//Extended Class MultiThread Test.
//Build under Windows, can be tested on Unix / Linux if you remove the system("pause");

class MyMultiTaskWork : public EasyCrossPlatform::Thread::SingleWorkCls {
public:
	void ThreadJob(std::thread::id & ThreadID, std::mutex* MyMutex = NULL, void* Parameters = NULL){
		while (this->getRunningStatus()) {
			std::cout << "Hi! Here is Cls for SingleWorkCls" << std::endl;
		}
		return;
	}
};

int main(int argc, char** args) {
	MyMultiTaskWork MT;
	std::cout << "MultiTask Starts" << std::endl;
	MT.StartJob();
	system("pause");
	MT.StopJob();
	system("pause");
	return 0;
}