#include "library.h"

#include <iostream>
#include <jvmti.h>

void hello() {
    std::cout << "Hello, World!" << std::endl;
}