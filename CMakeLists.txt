cmake_minimum_required(VERSION 3.7)
project(whatsvm)

set(CMAKE_CXX_STANDARD 11)

INCLUDE_DIRECTORIES(/opt/jdk1.8.0_131/include /opt/jdk1.8.0_131/include/linux)
#INCLUDE_DIRECTORIES(/Library/Java/JavaVirtualMachines/jdk1.8.0_73.jdk/Contents/Home/include/darwin)

set(SOURCE_FILES library.cpp library.h src/Agent.cpp src/MethodTrace.cpp)
add_library(whatsvm SHARED ${SOURCE_FILES})
