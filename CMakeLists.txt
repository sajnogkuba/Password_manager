cmake_minimum_required(VERSION 3.25)
project(Passwords_Manager_4_0)

set(CMAKE_CXX_STANDARD 20)


include(FetchContent)
FetchContent_Declare(
        fmt
        GIT_REPOSITORY  https://github.com/fmtlib/fmt
        GIT_TAG         a33701196adfad74917046096bf5a2aa0ab0bb50
)
FetchContent_MakeAvailable(fmt)

add_executable(Passwords_Manager_4_0 main.cpp Menu.h Menu.cpp Code.cpp Executor.h Executor.cpp Password.h Password.cpp)
target_link_libraries(Passwords_Manager_4_0 fmt)