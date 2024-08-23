cmake_minimum_required(VERSION 3.16)

set(CMAKE_CXX_STANDARD 17)

project(chess_cpp
        VERSION 3.0.0
        DESCRIPTION "A UCI Chess Engine"
        LANGUAGES CXX)

include(FetchContent)
set(CMAKE_POLICY_DEFAULT_CMP0135 NEW)
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