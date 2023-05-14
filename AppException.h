//Ilie Dumitru
#ifndef APPEXCEPTION_H
#define APPEXCEPTION_H

#include<stdexcept>
#include<iostream>

class AppException : public std::runtime_error
{
public:
    explicit AppException(const std::string& mesaj);
};

class InvalidQuery : public AppException
{
public:
    explicit InvalidQuery(const std::string& mesaj);
};

class MissingFontException : public AppException
{
public:
    explicit MissingFontException(const std::string& mesaj);
};

#endif//APPEXCEPTION_H