#include "EasyCrossPlatform.h"
#include <iostream>
//Sigle Thread Test
//Can Be Built Under Windows, if you remove the system(), then it works on unix / linux
struct TestThreadPara {
	unsigned int WorkID;
};

//Function to do multithread
void TaskJob(std::thread::id ThreadID, void * Parameters, bool * RunningSign, std::mutex *Mutex) {
	TestThreadPara &MyPara = *((TestThreadPara*)Parameters);
	while (*RunningSign) {
		std::cout << "Hi, Here's Subthread" << MyPara.WorkID << std::endl;
	}
	std::cout << "Thread Exited" << std::endl;
	return;
}

int main(int argc, char** args) {
	EasyCrossPlatform::Thread::SingleWork myOwnWork(TaskJob);
	TestThreadPara MyPara;
	MyPara.WorkID = 0;
	myOwnWork.StartJob(NULL, &MyPara);
	std::cout << "Main Thread Here" << std::endl;
	system("pause");
	myOwnWork.StopJob();
	std::cout << "Thread Had been stopped" << std::endl;
	system("pause");
	return 0;
}