cmake_minimum_required(VERSION 3.16)

project(chess_cpp
        VERSION 0.0.1
        DESCRIPTION "A Tiny UCI Chess Engine"
        LANGUAGES CXX)

include(FetchContent)
FetchContent_Declare(
    googletest
    # Specify the commit you depend on and update it regularly.
    URL https://github.com/google/googletest/archive/5376968f6948923e2411081fd9372e71a59d8e77.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

include(CTest)
# add_subdirectory(external)
add_subdirectory(src)
add_subdirectory(tests)