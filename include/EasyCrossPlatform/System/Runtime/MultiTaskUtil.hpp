#ifndef __EASYCROSSPLATFORM_FILE_MULTITASKVARIABLEH_INCLUDED__
    #define __EASYCROSSPLATFORM_FILE_MULTITASKVARIABLEH_INCLUDED__
    #include <EasyCrossPlatform/Internal/EasyCP_Common.h>
    #include <mutex>
    #include <condition_variable>
    #include <shared_mutex>
    #include <atomic>

    namespace EasyCrossPlatform{
        namespace System{
            namespace Runtime{
                namespace Thread{
                    typedef std::shared_mutex mutexType;
                    typedef std::shared_lock<mutexType> readLock;
                    typedef std::unique_lock<mutexType> writeLock;
                    template <typename T> 
                    using atomicVariable = std::atomic<T>;
                }
            }
        }
    }
#endif