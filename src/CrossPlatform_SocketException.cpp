#include "CrossPlatform_SocketException.h"

SocketException::SocketException(std::string& ErrorMessage) : std::runtime_error(ErrorMessage) {}

SocketException::SocketException(const char* ErrorMessage) : std::runtime_error(std::string(ErrorMessage)) {}
