#include <iostream>
#include <dlfcn.h>
//Выделение указателей под функции из дин. библиотек
bool (*ServerUp)(int) = nullptr;
std::string(*ReceiveRequest)() = nullptr;

void *libHandle = dlopen("libserver.so", RTLD_LAZY);
int main() {
    std::cout << "Hello, World!" << std::endl;
    return 0;
}
