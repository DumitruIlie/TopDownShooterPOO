//Ilie Dumitru
#include"AppException.h"

AppException::AppException(const std::string& mesaj) : std::runtime_error(mesaj) {if(!mesaj.empty()) std::cerr<<mesaj;}

InvalidQuery::InvalidQuery(const std::string& mesaj) : AppException(mesaj) {}