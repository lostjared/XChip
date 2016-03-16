#ifndef LOG_H
#define LOG_H

#ifdef _WIN32
#define CLS() std::system("cls")
#elif __linux__
#define CLS() std::system("clear")
#endif




#include <iostream>

#define LOG(x) std::cout << x << std::endl
#define LOGerr(x) std::cerr << x << std::endl



#endif // LOG_H
