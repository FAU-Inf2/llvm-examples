cmake_minimum_required(VERSION 3.6)
project(strength-reduction-pass)
# we need LLVM_HOME in order to automatically set LLVM_DIR
if(DEFINED ENV{LLVM_HOME})
    set(ENV{LLVM_DIR} $ENV{LLVM_HOME}/lib/cmake/llvm)
endif()

find_package(LLVM REQUIRED CONFIG)
add_definitions(${LLVM_DEFINITIONS})
include_directories(${LLVM_INCLUDE_DIRS})
link_directories(${LLVM_LIBRARY_DIRS})
if (${LLVM_VERSION_MAJOR} VERSION_GREATER_EQUAL 10)
    set(CMAKE_CXX_STANDARD 17)
endif ()
add_library(StrengthReduction MODULE
   # List your source files here.
   StrengthReduction.cpp
)
# Use C++11 to compile your pass
target_compile_features(StrengthReduction PRIVATE cxx_range_for cxx_auto_type)
# LLVM is (typically) built with no C++ RTTI. We need to match that;
# otherwise, we'll get linker errors about missing RTTI data.
set_target_properties(StrengthReduction PROPERTIES
    COMPILE_FLAGS "-fno-rtti"
)
# Build Standalone executable
add_executable(Standalone Standalone.cpp)
target_link_libraries(Standalone LLVM)
