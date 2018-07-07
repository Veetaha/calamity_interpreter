#include "message_exception.h"
#include <string>
#include "error.h"


const char *MessageException::what() const noexcept{
    return m_error.c_str();
}
