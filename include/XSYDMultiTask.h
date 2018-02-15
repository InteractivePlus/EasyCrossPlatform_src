#ifndef __XSYDMultiTaskFile__
	#define __XSYDMultiTaskFile__
	#include <EasyCP_Common.h>
	#include <thread>
	#include <mutex>
	namespace EasyCrossPlatform {
		namespace Thread {
			unsigned int getRecommendedThreadNum();
			typedef void (*SpecificWorkPtr)(std::thread::id ThreadID, void* Parameters, bool * RunningSign, std::mutex *Mutex);
			struct WorkInfo {
				SpecificWorkPtr MyWork = NULL;
				bool * RunningSign = NULL;
				std::mutex *mMutex = NULL;
				void * Parameters = NULL;
			};
			class SingleWork {
				private:
					std::thread mThread;
					SpecificWorkPtr MyWork;
					static int DoingJob(WorkInfo MyInfo);
				protected:
					bool RunningSign = false;
				public:
					
					SingleWork();
					SingleWork(SpecificWorkPtr mWork);
					SingleWork(const SingleWork &mWork);
					void setWork(SpecificWorkPtr mWork);
					bool StartJob(std::mutex *MyMutex = NULL, void* Parameter = NULL);
					void StopJob();
					bool getRunningStatus();
					~SingleWork();
			};
			class SingleWorkCls {
				private:
					std::thread mThread;
					std::mutex* TempMutex = NULL;
					void* TempParameter = NULL;
					bool RunningSign = false;
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
				SingleWork* wInfo = NULL;
				SingleWorkCls *wClsInfo = NULL;
				void* wParameters = NULL;
			};
			struct WorksInfo {
				std::deque<WorkerInfo>* CurrentWorksAddr = NULL;
				std::deque<WorkerInfo>* PendingWorksAddr = NULL;
				unsigned int MaxThread;
				std::mutex *SharedMutex = NULL;
				std::mutex *LineMutex = NULL;
			};
			class WorkPool {
				private:
					static void SuperviseThreads(std::thread::id ThreadID, void* Parameters, bool * RunningSign, std::mutex *Mutex);
					unsigned int MaxThread = 4U;
					std::deque<WorkerInfo> CurrentWorks;
					std::deque <WorkerInfo> PendingWorks;
					std::mutex MyMutex;
					std::mutex LineMutex;
					SingleWork* SupervisingThread = NULL;
					bool Started = false;
				public:
					WorkPool(const unsigned int ThreadNum = 4U);
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
