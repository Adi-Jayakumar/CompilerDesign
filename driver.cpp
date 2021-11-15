#include <cstdio>
#include <iostream>

#ifdef _WIN32
#define DLLEXPORT __declspec(dllexport)
#else
#define DLLEXPORT
#endif

extern "C" DLLEXPORT int print_int(int X)
{
    fprintf(stderr, "%d\n", X);
    return 0;
}

extern "C" DLLEXPORT float print_float(float X)
{
    fprintf(stderr, "%f\n", X);
    return 0;
}

extern "C"
{
    void nothing();
}

int main()
{
    nothing();
    std::cout << "answer = " << std::endl;
}