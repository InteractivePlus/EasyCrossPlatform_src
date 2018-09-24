#ifndef __EASYCROSSPLATFORM_FILE_MULTITASKH_INCLUDED__
    #define __EASYCROSSPLATFORM_FILE_MULTITASKH_INCLUDED__
    #include <EasyCrossPlatform/Internal/EasyCP_Common.h>
    #include <EasyCrossPlatform/Templates/ThreadManager.hpp>
    #include <EasyCrossPlatform/System/Runtime/MultiTaskUtil.hpp>
    #include <memory>
    #include <atomic>
    #include <condition_variable>

    namespace EasyCrossPlatform{
        namespace System{
            namespace Runtime{
                namespace Thread{
                    unsigned int getRecommendedThreadNum();
                    
                    typedef void(*SingleWorkFunctionPtr)(const atomicVariable<bool>&,void*);
                    class SingleWorkController : public EasyCrossPlatform::Templates::Thread::RunnableTaskController{
                        private:
                            static void __doJobStatic(SingleWorkController* ClassPtr);
                        protected:
                            std::unique_ptr<std::condition_variable> m_Notify;
                            std::unique_ptr<std::mutex> m_NotifyMutex;
                            SingleWorkFunctionPtr m_WorkDetail = NULL;
                            std::unique_ptr<atomicVariable<bool>> m_RunningControl;
                            std::unique_ptr<atomicVariable<bool>> m_RunningIndicator;
                            void* m_Parameters = NULL;
                        public:
                            SingleWorkController();
                            SingleWorkController(SingleWorkFunctionPtr WorkDetail);
                            SingleWorkController(SingleWorkController&& OldWorkController);
                            SingleWorkController(SingleWorkController& OldWorkController) = delete;
                            SingleWorkController& operator=(SingleWorkController&& OldWorkController);
                            SingleWorkController& operator=(SingleWorkController& OldWorkController) = delete;
                            void StartWork(void* Params);
                            void StopWork();
                            void WaitUntilWorkFinished();
                            bool isRunning();
                            bool isStopping();
                            void setWork(SingleWorkFunctionPtr WorkDetail);
                            ~SingleWorkController();
                    };
                    class SingleWorkCls : public EasyCrossPlatform::Templates::Thread::RunnableTaskController{
                        private:
                            static void __staticWorkPtr(const atomicVariable<bool>& RunningController, void* myClassVoidPtr);
                        protected:
                            SingleWorkController m_WorkController{SingleWorkCls::__staticWorkPtr};
                            void* m_Parameters = NULL;
                        public:
                            SingleWorkCls();
                            void StartWork(void* Params);
                            void StopWork();
                            void WaitUntilWorkFinished();
                            bool isRunning();
                            bool isStopping();
                            virtual void DoJob(const atomicVariable<bool>& RunningController, void* Params) = 0;
                    };
                    struct SingleWorkerInfo{
                        EasyCrossPlatform::Templates::Thread::RunnableTaskController* TaskControllerPtr;
                        void* RunningParameter;
                    };
                    class WorkerPool{
                        private:
                            
                        protected:
                            std::unique_ptr<mutexType> m_PendingWorkerQueueMutex;
                            std::unique_ptr<mutexType> m_CurrentWorkerQueueMutex;
                            std::deque<EasyCrossPlatform::Templates::Thread::RunnableTaskController> m_PendingWorkerQueue;
                            std::deque<EasyCrossPlatform::Templates::Thread::RunnableTaskController> m_CurrentWorkers;
                        public:

                    };
                }
            }
        }
    }
#endif