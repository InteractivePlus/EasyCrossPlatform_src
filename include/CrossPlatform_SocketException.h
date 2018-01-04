#ifndef __EasyCrossPlatform_CrossPlatform_SocketExceptionFile__
#define __EasyCrossPlatform_CrossPlatform_SocketExceptionFile__

#include <stdexcept>
#include <string>
#include <EasyCP_Common.h>
class SocketException : public std::runtime_error
{
public:
	explicit SocketException(std::string& ErrorMessage);
	explicit SocketException(const char* ErrorMessage);
};
#endif//__EasyCrossPlatform_CrossPlatform_SocketExceptionFile__