#include <XSYDMultiTask.h>
unsigned int EasyCrossPlatform::Thread::getRecommendedThreadNum() {
	return std::thread::hardware_concurrency();
}

int EasyCrossPlatform::Thread::SingleWork::DoingJob(EasyCrossPlatform::Thread::WorkInfo MyInfo)
{
	MyInfo.MyWork(std::this_thread::get_id(), MyInfo.Parameters, MyInfo.RunningSign, MyInfo.mMutex);
	*(MyInfo.RunningSign) = false;
	return 0;
}

EasyCrossPlatform::Thread::SingleWork::SingleWork(){
	
}

EasyCrossPlatform::Thread::SingleWork::SingleWork(EasyCrossPlatform::Thread::SpecificWorkPtr mWork)
{
	this->MyWork = mWork;
	this->RunningSign = false;
}


EasyCrossPlatform::Thread::SingleWork::SingleWork(const SingleWork & mWork)
{
	this->MyWork = mWork.MyWork;
	this->RunningSign = false;
}

void EasyCrossPlatform::Thread::SingleWork::setWork(EasyCrossPlatform::Thread::SpecificWorkPtr mWork)
{
	this->MyWork = mWork;
}

bool EasyCrossPlatform::Thread::SingleWork::StartJob(std::mutex *MyMutex, void * Parameter)
{
	if (this->RunningSign) { return false; }
	WorkInfo MyInfo;
	MyInfo.mMutex = MyMutex;
	MyInfo.MyWork = this->MyWork;
	MyInfo.Parameters = Parameter;
	MyInfo.RunningSign = &(this->RunningSign);
	this->RunningSign = true;
	this->mThread = std::thread(EasyCrossPlatform::Thread::SingleWork::DoingJob, MyInfo);
	this->mThread.detach();
	return true;
}

void EasyCrossPlatform::Thread::SingleWork::StopJob()
{
	this->RunningSign = false;
	return;
}

bool EasyCrossPlatform::Thread::SingleWork::getRunningStatus()
{
	return this->RunningSign;
}

EasyCrossPlatform::Thread::SingleWork::~SingleWork()
{
	if (this->RunningSign) { this->StopJob(); }
}

void EasyCrossPlatform::Thread::WorkPool::SuperviseThreads(std::thread::id ThreadID, void * Parameters, bool * RunningSign, std::mutex * Mutex)
{
	//信息预处理
	WorksInfo MyInfo = *((WorksInfo *)Parameters);
	std::deque<WorkerInfo> &MyWork = *(MyInfo.CurrentWorksAddr);
	std::deque<WorkerInfo> &PendingWork = *(MyInfo.PendingWorksAddr);
	unsigned int MaxThreadNum = MyInfo.MaxThread;
	std::mutex* LineMutex = MyInfo.LineMutex;
	delete Parameters;
	//信息预处理完毕

	WorkerInfo TmpWorkerInfo;
	while ((*RunningSign)) {
		if (!MyWork.empty()) {
			for (auto i = MyWork.begin(); i != MyWork.end(); i++) {
				//如果运行完毕, 则自动删除, 空出线程池空间
				if ((*i).wInfo != NULL) {
					if ((*i).wInfo->getRunningStatus() == false) {
						delete (*i).wInfo;
						i = MyWork.erase(i);
					}
				}
				else { //WClsInfo
					if ((*i).wClsInfo->getRunningStatus() == false) {
						i = MyWork.erase(i);
					}
				}
				if (i == MyWork.end()) {
					break;
				}
			}
		}
		//如果正在执行的线程数少于规定的线程数
		if (MyWork.size() < MaxThreadNum && PendingWork.size() > 0) {
			LineMutex->lock();
			TmpWorkerInfo = PendingWork.front();
			PendingWork.pop_front();
			LineMutex->unlock();
			if (TmpWorkerInfo.wInfo != NULL) {
				TmpWorkerInfo.wInfo->StartJob(MyInfo.SharedMutex, TmpWorkerInfo.wParameters);
			}
			else {
				TmpWorkerInfo.wClsInfo->StartJob(MyInfo.SharedMutex, TmpWorkerInfo.wParameters);
			}
			MyWork.push_back(TmpWorkerInfo);
		}
	}
	//不Running了,销毁其余
	if (!MyWork.empty()) {
		for (auto i = MyWork.begin(); i != MyWork.end(); i++) {
			if ((*i).wInfo != NULL) {
				(*i).wInfo->StopJob();
				delete (*i).wInfo;
			}
			else {
				(*i).wClsInfo->StopJob();
			}
			i = MyWork.erase(i);
			if (i == MyWork.end()) {
				break;
			}
		}
	}
	if (!PendingWork.empty()) {
		for (auto i = PendingWork.begin(); i != PendingWork.end(); i++) {
			if ((*i).wInfo != NULL) {
				delete (*i).wInfo;
			}
			else {
				//Dont need to clean.
			}
			i = PendingWork.erase(i);
			if (i == PendingWork.end()) {
				break;
			}
		}
	}
	return;
}

EasyCrossPlatform::Thread::WorkPool::WorkPool(const unsigned int ThreadNum)
{
	this->MaxThread = ThreadNum;
	this->SupervisingThread = new EasyCrossPlatform::Thread::SingleWork(this->SuperviseThreads);
}


EasyCrossPlatform::Thread::WorkPool::~WorkPool()
{
	this->SupervisingThread->StopJob();
	this->Started = false;
	delete this->SupervisingThread;
	this->SupervisingThread = NULL;
}

void EasyCrossPlatform::Thread::WorkPool::Start()
{
	if (!this->Started) {
		WorksInfo* MyInfo = new WorksInfo;
		MyInfo->CurrentWorksAddr = &(this->CurrentWorks);
		MyInfo->PendingWorksAddr = &(this->PendingWorks);
		MyInfo->MaxThread = this->MaxThread;
		MyInfo->SharedMutex = &this->MyMutex;
		MyInfo->LineMutex = &this->LineMutex;
		this->SupervisingThread->StartJob(NULL, (void*)MyInfo);
	}
}

void EasyCrossPlatform::Thread::WorkPool::Stop()
{
	this->Started = false;
	this->SupervisingThread->StopJob();
}

void EasyCrossPlatform::Thread::WorkPool::addWork(SingleWork &MyWork, void* Parameter)
{
	SingleWork* TmpWork = new SingleWork(MyWork);
	WorkerInfo MInfo;
	MInfo.wInfo = TmpWork;
	MInfo.wParameters = Parameter;
	MInfo.wClsInfo = NULL;
	this->LineMutex.lock();
	this->PendingWorks.push_back(MInfo);
	this->LineMutex.unlock();
}

void EasyCrossPlatform::Thread::WorkPool::addWork(SingleWorkCls & MyWork, void * Parameter)
{
	SingleWorkCls* TmpWork = &MyWork;
	WorkerInfo MInfo;
	MInfo.wClsInfo = TmpWork;
	MInfo.wParameters = Parameter;
	MInfo.wInfo = NULL;
	this->LineMutex.lock();
	this->PendingWorks.push_back(MInfo);
	this->LineMutex.unlock();
}

void EasyCrossPlatform::Thread::WorkPool::addWork_AtFront(SingleWork &MyWork, void* Parameter)
{
	SingleWork* TmpWork = new SingleWork(MyWork);
	WorkerInfo MInfo;
	MInfo.wInfo = TmpWork;
	MInfo.wParameters = Parameter;
	MInfo.wClsInfo = NULL;
	this->LineMutex.lock();
	this->PendingWorks.push_front(MInfo);
	this->LineMutex.unlock();
}

void EasyCrossPlatform::Thread::WorkPool::addWork_AtFront(SingleWorkCls &MyWork, void * Parameter)
{
	SingleWorkCls* TmpWork = &MyWork;
	WorkerInfo MInfo;
	MInfo.wClsInfo = TmpWork;
	MInfo.wParameters = Parameter;
	MInfo.wInfo = NULL;
	this->LineMutex.lock();
	this->PendingWorks.push_front(MInfo);
	this->LineMutex.unlock();
}

int EasyCrossPlatform::Thread::SingleWorkCls::DoingJob(SingleWorkCls * ClassPtr)
{
	ClassPtr->ThreadJob(std::this_thread::get_id(),ClassPtr->TempMutex, ClassPtr->TempParameter);
	ClassPtr->RunningSign = false;
	return 0;
}

EasyCrossPlatform::Thread::SingleWorkCls::SingleWorkCls()
{
	this->RunningSign = false;
}

EasyCrossPlatform::Thread::SingleWorkCls::SingleWorkCls(SingleWorkCls & CopyWorkCls)
{
	this->RunningSign = false;
}

bool EasyCrossPlatform::Thread::SingleWorkCls::StartJob(std::mutex* MyMutex, void* Parameters)
{
	if (this->RunningSign) {
		return false;
	}
	this->TempParameter = Parameters;
	this->TempMutex = MyMutex;
	this->RunningSign = true;
	this->mThread = std::thread(EasyCrossPlatform::Thread::SingleWorkCls::DoingJob, this);
	this->mThread.detach();
	return true;
}

void EasyCrossPlatform::Thread::SingleWorkCls::StopJob()
{

	this->RunningSign = false;
	return;
}
bool EasyCrossPlatform::Thread::SingleWorkCls::getRunningStatus()
{
	return this->RunningSign;
}

void EasyCrossPlatform::Thread::SingleWorkCls::ThreadJob(std::thread::id ThreadID, std::mutex* MyMutex, void* Parameters)
{
	return;
}

EasyCrossPlatform::Thread::SingleWorkCls::~SingleWorkCls()
{
	if (this->RunningSign) { this->StopJob(); }
}
