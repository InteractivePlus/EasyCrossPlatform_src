#ifndef __EASYCROSSPLATFORM_FILE_THREADMANAGERHPP_INCLUDED__
    #define __EASYCROSSPLATFORM_FILE_THREADMANAGERHPP_INCLUDED__
    #include <EasyCrossPlatform/Internal/EasyCP_Common.h>
    namespace EasyCrossPlatform{
        namespace Templates{
            namespace Thread{
                class RunnableTaskController{
                    private:

                    protected:

                    public:
                        virtual void StartWork(void* Params) = 0;
                        virtual void StopWork() = 0;
                        virtual void WaitUntilWorkFinished() = 0;
                        virtual bool isRunning() = 0;
                        virtual bool isStopping() = 0;
                };
            }
        }
    }
#endif