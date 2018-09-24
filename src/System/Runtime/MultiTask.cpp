#include <EasyCrossPlatform/System/Runtime/MultiTask.h>
#include <thread>
unsigned int EasyCrossPlatform::System::Runtime::Thread::getRecommendedThreadNum(){
    return std::thread::hardware_concurrency();
}
void EasyCrossPlatform::System::Runtime::Thread::SingleWorkController::__doJobStatic(SingleWorkController* ClassPtr){
    atomicVariable<bool>* RunningControlPtr = ClassPtr->m_RunningControl.get();
    atomicVariable<bool>* RunningIndicatorPtr = ClassPtr->m_RunningIndicator.get();
    std::condition_variable* ConditionalVariable = ClassPtr->m_Notify.get();
    
    ClassPtr->m_WorkDetail(RunningControlPtr,ClassPtr->m_Parameters);
    
    (*RunningIndicatorPtr) = false;
    ConditionalVariable->notify_all();
}

EasyCrossPlatform::System::Runtime::Thread::SingleWorkController::SingleWorkController(){
    this->m_RunningControl = std::unique_ptr<atomicVariable<bool>>(new atomicVariable<bool>(false));
    this->m_RunningIndicator = std::unique_ptr<atomicVariable<bool>>(new atomicVariable<bool>(false));
    this->m_Notify = std::unique_ptr<std::condition_variable>(new std::condition_variable());
    this->m_NotifyMutex = std::unique_ptr<std::mutex>(new std::mutex());
}
EasyCrossPlatform::System::Runtime::Thread::SingleWorkController::SingleWorkController(EasyCrossPlatform::System::Runtime::Thread::SingleWorkFunctionPtr WorkDetail){
    this->m_WorkDetail = WorkDetail;
    this->m_RunningControl = std::unique_ptr<atomicVariable<bool>>(new atomicVariable<bool>(false));
    this->m_RunningIndicator = std::unique_ptr<atomicVariable<bool>>(new atomicVariable<bool>(false));
    this->m_Notify = std::unique_ptr<std::condition_variable>(new std::condition_variable());
    this->m_NotifyMutex = std::unique_ptr<std::mutex>(new std::mutex());
}
EasyCrossPlatform::System::Runtime::Thread::SingleWorkController::SingleWorkController(EasyCrossPlatform::System::Runtime::Thread::SingleWorkController&& OldWorkController){
    if((*this->m_RunningIndicator)){
        throw std::runtime_error("You cannot change the work when the work inside is still processsing");
    }
    this->m_WorkDetail = OldWorkController.m_WorkDetail;
    this->m_RunningControl = std::move(OldWorkController.m_RunningControl);
    this->m_RunningIndicator = std::move(OldWorkController.m_RunningIndicator);
    this->m_Notify = std::move(OldWorkController.m_Notify);
    this->m_NotifyMutex = std::move(OldWorkController.m_NotifyMutex);
}
EasyCrossPlatform::System::Runtime::Thread::SingleWorkController& EasyCrossPlatform::System::Runtime::Thread::SingleWorkController::operator=(EasyCrossPlatform::System::Runtime::Thread::SingleWorkController&& OldWorkController){
    if((*this->m_RunningIndicator)){
        throw std::runtime_error("You cannot change the work when the work inside is still processsing");
    }
    this->m_WorkDetail = OldWorkController.m_WorkDetail;
    this->m_RunningControl = std::move(OldWorkController.m_RunningControl);
    this->m_RunningIndicator = std::move(OldWorkController.m_RunningIndicator);
    this->m_Notify = std::move(OldWorkController.m_Notify);
    this->m_NotifyMutex = std::move(OldWorkController.m_NotifyMutex);
    return (*this);
}
void EasyCrossPlatform::System::Runtime::Thread::SingleWorkController::StartWork(void* Params){
    if((*this->m_RunningIndicator)){
        throw std::runtime_error("You cannot start another process while the work inside is still processing");
    }

    this->m_Parameters = Params;
    (*this->m_RunningControl) = true;
    (*this->m_RunningIndicator) = true;
    std::thread myThread = std::thread(SingleWorkController::__doJobStatic,this);
    myThread.detach();
}

void EasyCrossPlatform::System::Runtime::Thread::SingleWorkController::StopWork(){
    (*this->m_RunningControl) = false;
}

void EasyCrossPlatform::System::Runtime::Thread::SingleWorkController::WaitUntilWorkFinished(){
    if((*this->m_RunningIndicator)){
        std::unique_lock<std::mutex> mLock{(*this->m_NotifyMutex)};
        this->m_Notify->wait(
            mLock,
            [this]() -> bool{
                return (!this->isRunning());
            }
        );
    }
}

bool EasyCrossPlatform::System::Runtime::Thread::SingleWorkController::isRunning(){
    return (*this->m_RunningIndicator);
}
bool EasyCrossPlatform::System::Runtime::Thread::SingleWorkController::isStopping(){
    return !(*this->m_RunningControl);
}
void EasyCrossPlatform::System::Runtime::Thread::SingleWorkController::setWork(SingleWorkFunctionPtr WorkDetail){
    if((*this->m_RunningIndicator)){
        throw std::runtime_error("You cannot change the work when the work inside is still processsing");
    }
    this->m_WorkDetail = WorkDetail;
}
EasyCrossPlatform::System::Runtime::Thread::SingleWorkController::~SingleWorkController(){
    if((*this->m_RunningIndicator)){
        throw std::runtime_error("You should not destruct this class when the work had not finished processing");
    }
}

void EasyCrossPlatform::System::Runtime::Thread::SingleWorkCls::__staticWorkPtr(const EasyCrossPlatform::System::Runtime::Thread::atomicVariable<bool>& RunningController, void* myClassVoidPtr){
    SingleWorkCls* myClass = (SingleWorkCls*) myClassVoidPtr;
    myClass->DoJob(RunningController,myClass->m_Parameters);
}

void EasyCrossPlatform::System::Runtime::Thread::SingleWorkCls::StartWork(void* Params){
    this->m_Parameters = Params;
    this->m_WorkController.StartWork(this);
}
void EasyCrossPlatform::System::Runtime::Thread::SingleWorkCls::StopWork(){
    this->m_WorkController.StopWork();
}
void EasyCrossPlatform::System::Runtime::Thread::SingleWorkCls::WaitUntilWorkFinished(){
    this->m_WorkController.WaitUntilWorkFinished();
}
bool EasyCrossPlatform::System::Runtime::Thread::SingleWorkCls::isRunning(){
    return this->m_WorkController.isRunning();
}
bool EasyCrossPlatform::System::Runtime::Thread::SingleWorkCls::isStopping(){
    return this->m_WorkController.isStopping();
}