#ifndef __EASYCROSSPLATFORM_FILE_WHEREAMIH_INCLUDED__
    #define __EASYCROSSPLATFORM_FILE_WHEREAMIH_INCLUDED__
    #include <EasyCrossPlatform/Internal/EasyCP_Common.h>
    namespace EasyCrossPlatform{
        namespace System{
            namespace Runtime{
                    const constexpr unsigned int PathMaxSize = 1024U;
                    std::string APPPath(); //Returns the directory that the program is in, ending with "\\" or "/"
                    std::string APPExeName();
                    std::string CWDPath();
            }
        }
    }
#endif