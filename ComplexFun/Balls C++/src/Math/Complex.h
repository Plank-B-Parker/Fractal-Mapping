#pragma once
#include <cmath>

struct complex{
    float a = 0, b = 0;

    complex(float a, float b): a(a), b(b){}
    complex(float a): a(a){}
    complex(){}
    float length(){
        return std::sqrt(a*a + b*b);
    }
    
    void operator +=(complex x){
        a += x.a;
        b += x.b;
    }

};

inline complex operator +(complex x, complex y){
    return complex(x.a + y.a, x.b + y.b);
}
inline complex operator -(complex x, complex y){
    return complex(x.a - y.a, x.b - y.b);
}
inline complex operator *(complex x, complex y){
    return complex(x.a * y.a - x.b * y.b, x.a * y.b + x.b * y.a);
}
inline complex conj(complex x){
    return complex(x.a, -x.b);
}
inline complex operator /(complex x, float y){
    return complex(x.a/y, x.b/y);
}
inline complex operator /(complex x, complex y){
    return x * conj(y) / y.length();
}
