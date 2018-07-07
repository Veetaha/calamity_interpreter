#ifndef MESSAGEEXCEPTION_H
#define MESSAGEEXCEPTION_H

#include <stdexcept>
#include <string>
#include "error.h"
#include <boost/lexical_cast.hpp>

class  MessageException : public std::exception {
    std::string m_error;
public:

    template <typename TString>
    explicit MessageException(const TString & errorString)
        : m_error(boost::lexical_cast<std::string>(errorString)) {
        Debug_log("MessageException instance was thrown -> " << m_error.c_str());
    }

    template <typename TString>
    explicit MessageException(TString && errorString)
        : m_error(boost::lexical_cast<std::string>(std::move(errorString))) {
        Debug_log("MessageException instance was thrown -> " << m_error.c_str());
    }



    virtual const char * what() const noexcept final override;
};

#endif // MESSAGEEXCEPTION_H
