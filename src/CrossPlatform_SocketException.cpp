#include "CrossPlatform_SocketException.h"
using namespace std;
SocketException::SocketException(std::string& ErrorMessage) : std::runtime_error(ErrorMessage) {}

SocketException::SocketException(const char* ErrorMessage) : std::runtime_error(std::string(ErrorMessage)) {}
