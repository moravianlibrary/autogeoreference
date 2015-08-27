/* 
 * File:   GeoreferencerException.h
 * Author: dudae
 *
 * Created on Utorok, 2015, augusta 11, 14:24
 */

#ifndef GEOREFERENCEREXCEPTION_H
#define	GEOREFERENCEREXCEPTION_H

#include <exception>

class GeoreferencerException : public std::exception {
public:
    GeoreferencerException(std::string message) : message(message) {}
    
    ~GeoreferencerException() throw() {}
    
    virtual const char* what() const throw() {
        return message.c_str();
    }
    
private:
    std::string message;
};

#endif	/* GEOREFERENCEREXCEPTION_H */

