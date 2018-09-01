#include <EasyCrossPlatform/System/Runtime/MultiTask.h>
#include <thread>
unsigned int EasyCrossPlatform::Runtime::Thread::getRecommendedThreadNum(){
    return std::thread::hardware_concurrency();
}
void EasyCrossPlatform::Runtime::Thread::SingleWorkController::__doJobStatic(SingleWorkController* ClassPtr){
    bool* RunningControlPtr = ClassPtr->m_RunningControl.get();
    bool* RunningIndicatorPtr = ClassPtr->m_RunningIndicator.get();
    ClassPtr->m_WorkDetail(RunningControlPtr,ClassPtr->m_Parameters);
    (*RunningIndicatorPtr) = false;
}

EasyCrossPlatform::Runtime::Thread::SingleWorkController::SingleWorkController(){
    this->m_RunningControl = std::unique_ptr<bool>(new bool);
    this->m_RunningIndicator = std::unique_ptr<bool>(new bool);
    (*this->m_RunningControl) = false;
    (*this->m_RunningIndicator) = false;
}
EasyCrossPlatform::Runtime::Thread::SingleWorkController::SingleWorkController(EasyCrossPlatform::Runtime::Thread::SingleWorkFunctionPtr WorkDetail){
    this->m_WorkDetail = WorkDetail;
    this->m_RunningControl = std::unique_ptr<bool>(new bool);
    this->m_RunningIndicator = std::unique_ptr<bool>(new bool);
    (*this->m_RunningControl) = false;
    (*this->m_RunningIndicator) = false;
}
EasyCrossPlatform::Runtime::Thread::SingleWorkController::SingleWorkController(EasyCrossPlatform::Runtime::Thread::SingleWorkController&& OldWorkController){
    if((*this->m_RunningIndicator)){
        throw std::runtime_error("You cannot change the work when the work inside is still processsing");
    }
    this->m_WorkDetail = OldWorkController.m_WorkDetail;
    this->m_RunningControl = std::move(OldWorkController.m_RunningControl);
    this->m_RunningIndicator = std::move(OldWorkController.m_RunningIndicator);
}
void EasyCrossPlatform::Runtime::Thread::SingleWorkController::StartWork(void* Params){
    if((*this->m_RunningIndicator)){
        throw std::runtime_error("You cannot start another process while the work inside is still processing");
    }

    this->m_Parameters = Params;
    (*this->m_RunningControl) = true;
    (*this->m_RunningIndicator) = true;
    std::thread myThread = std::thread(SingleWorkController::__doJobStatic,this);
    myThread.detach();
}

void EasyCrossPlatform::Runtime::Thread::SingleWorkController::StopWork(){
    (*this->m_RunningControl) = false;
}

void EasyCrossPlatform::Runtime::Thread::SingleWorkController::WaitUntilWorkFinished(){
    while((*this->m_RunningIndicator)){
        std::this_thread::sleep_for(std::chrono::duration<unsigned int,std::milli>(200));
    }
}

bool EasyCrossPlatform::Runtime::Thread::SingleWorkController::isRunning(){
    return (*this->m_RunningIndicator);
}
bool EasyCrossPlatform::Runtime::Thread::SingleWorkController::isStopping(){
    return !(*this->m_RunningControl);
}
void EasyCrossPlatform::Runtime::Thread::SingleWorkController::setWork(SingleWorkFunctionPtr WorkDetail){
    if((*this->m_RunningIndicator)){
        throw std::runtime_error("You cannot change the work when the work inside is still processsing");
    }
    this->m_WorkDetail = WorkDetail;
}
EasyCrossPlatform::Runtime::Thread::SingleWorkController::~SingleWorkController(){
    if((*this->m_RunningIndicator)){
        throw std::runtime_error("You should not destruct this class when the work had not finished processing");
    }
}