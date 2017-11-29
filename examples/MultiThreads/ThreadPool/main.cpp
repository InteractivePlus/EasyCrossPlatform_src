#include "EasyCrossPlatform.h"
#include <iostream>
//Worker Pool Test
//Build under Windows, can be tested on Unix / Linux if you remove the system("pause");

struct TestThreadPara {
	unsigned int WorkID;
};

//Function to be multitasked
void TaskJob_MTask(std::thread::id ThreadID, void * Parameters, bool * RunningSign, std::mutex *Mutex) {
	TestThreadPara &MyPara = *((TestThreadPara*)Parameters);
	Mutex->lock();
	std::cout << "Hi, Here's Subthread" << MyPara.WorkID << std::endl;
	std::cout << "Thread Exited" << std::endl;
	Mutex->unlock();
	return;
}

//Class to be multitasked
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
	EasyCrossPlatform::Thread::SingleWork myOwnWork(TaskJob_MTask);
	EasyCrossPlatform::Thread::WorkPool myWorks(4);
	MyMultiTaskWork MyMultiWork;
	myWorks.Start();
	TestThreadPara MyPara[10];
	for (unsigned int i = 0; i < 10; i++) {
		MyPara[i].WorkID = i;
		myWorks.addWork(myOwnWork, &MyPara[i]);
	}
	for (unsigned int i = 0; i < 10; i++) {
		myWorks.addWork(MyMultiWork, NULL);
	}
	system("pause");
	myWorks.Stop();
	return 0;
}