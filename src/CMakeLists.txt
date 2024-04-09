cmake_minimum_required(VERSION 3.16)

# Add source to this project's executable.
add_executable(chess_cpp 
    chess_cpp.cpp
    lib/board.cpp
    lib/engine.cpp
    lib/evaluate.cpp
    lib/uci.cpp
)

if(BUILD_TESTING)
    add_executable(board_test board_test.cpp)
    target_link_libraries(board_test gtest_main)

    add_test(NAME board_test_ COMMAND board_test)
endif()

# Optionally, you can include header files via target_include_directories()
# target_include_directories(chess_cpp PRIVATE src)

# TODO: Add tests and install targets if needed.
