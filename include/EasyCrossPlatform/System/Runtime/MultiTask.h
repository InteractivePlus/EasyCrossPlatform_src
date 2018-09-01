#ifndef __EASYCROSSPLATFORM_FILE_MULTITASKH_INCLUDED__
    #define __EASYCROSSPLATFORM_FILE_MULTITASKH_INCLUDED__
    #include <EasyCrossPlatform/Internal/EasyCP_Common.h>
    #include <memory>

    namespace EasyCrossPlatform{
        namespace Runtime{
            namespace Thread{
                unsigned int getRecommendedThreadNum();
                
                class SingleWorkController;
                typedef void(*SingleWorkFunctionPtr)(const bool*,void*);
                class SingleWorkController{
                    private:
                        static void __doJobStatic(SingleWorkController* ClassPtr);
                    protected:
                        SingleWorkFunctionPtr m_WorkDetail = NULL;
                        std::unique_ptr<bool> m_RunningControl;
                        std::unique_ptr<bool> m_RunningIndicator;
                        void* m_Parameters;
                    public:
                        SingleWorkController();
                        SingleWorkController(SingleWorkFunctionPtr WorkDetail);
                        SingleWorkController(SingleWorkController&& OldWorkController);
                        void StartWork(void* Params);
                        void StopWork();
                        void WaitUntilWorkFinished();
                        bool isRunning();
                        bool isStopping();
                        void setWork(SingleWorkFunctionPtr WorkDetail);
                        ~SingleWorkController();
                };
            }
        }
    }
#endif