#ifndef __XSYDMultiTaskFile__
	#define __XSYDMultiTaskFile__
	#include <EasyCP_Common.h>
	#include <thread>
	#include <mutex>
	namespace EasyCrossPlatform {
		namespace Thread {

			typedef void (*SpecificWorkPtr)(std::thread::id ThreadID, void* Parameters, bool * RunningSign, std::mutex *Mutex);
			struct WorkInfo {
				SpecificWorkPtr MyWork;
				bool * RunningSign;
				std::mutex *mMutex;
				void * Parameters;
			};
			class SingleWork {
				private:
					std::thread *mThread;
					bool RunningSign;
					SpecificWorkPtr MyWork;
					static int DoingJob(WorkInfo MyInfo);
				protected:

				public:
					SingleWork();
					SingleWork(SpecificWorkPtr mWork);
					SingleWork(SingleWork &mWork);
					void setWork(SpecificWorkPtr mWork);
					bool StartJob(std::mutex *MyMutex = NULL, void* Parameter = NULL);
					void StopJob();
					bool getRunningStatus();
					~SingleWork();
			};
			class SingleWorkCls {
				private:
					std::thread *mThread;
					std::mutex* TempMutex;
					void* TempParameter;
					bool RunningSign;
					static int DoingJob(SingleWorkCls* ClassPtr);
				protected:

				public:
					SingleWorkCls();
					SingleWorkCls(SingleWorkCls &CopyWorkCls);
					bool StartJob(std::mutex* MyMutex = NULL, void* Parameters = NULL);
					void StopJob();
					bool getRunningStatus();
					virtual void ThreadJob(std::thread::id ThreadID, std::mutex* MyMutex = NULL, void* Parameters = NULL);
					~SingleWorkCls();
			};
			struct WorkerInfo {
				SingleWork* wInfo;
				SingleWorkCls *wClsInfo;
				void* wParameters;
			};
			struct WorksInfo {
				std::deque<WorkerInfo>* CurrentWorksAddr;
				std::deque<WorkerInfo>* PendingWorksAddr;
				unsigned int MaxThread;
				std::mutex *SharedMutex;
				std::mutex *LineMutex;
			};
			class WorkPool {
				private:
					static void SuperviseThreads(std::thread::id ThreadID, void* Parameters, bool * RunningSign, std::mutex *Mutex);
					unsigned int MaxThread = 4;
					std::deque<WorkerInfo> CurrentWorks;
					std::deque <WorkerInfo> PendingWorks;
					std::mutex MyMutex;
					std::mutex LineMutex;
					SingleWork* SupervisingThread;
					bool Started;
				public:
					WorkPool(const unsigned int ThreadNum = 4);
					~WorkPool();
					void Start();
					void Stop();
					void addWork(SingleWork &MyWork, void* Parameter = NULL);
					void addWork(SingleWorkCls &MyWork, void* Parameter = NULL);
					void addWork_AtFront(SingleWork &MyWork, void* Parameter = NULL);
					void addWork_AtFront(SingleWorkCls &MyWork, void* Parameter = NULL);
			};
		}
	}
#endif
