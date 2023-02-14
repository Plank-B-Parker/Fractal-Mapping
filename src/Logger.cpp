#include "Logger.h"
#include "Math/Complex.h"

inline void log(std::string message){
    std::cout << message << std::endl;
}
inline void log(std::string name, complex z){
    std::cout << name << ": (" << z.a << ", " << z.b << ")" << std::endl;
}