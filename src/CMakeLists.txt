cmake_minimum_required(VERSION 3.16)

# Add source to this project's executable.
add_executable(chess_cpp 
    chess_cpp.cpp
    lib/board.cpp
    lib/evaluate.cpp
    lib/misc.cpp
    
    lib/search.cpp
    lib/uci.cpp
 "lib/tt.cpp" "lib/hashing.cpp")

# Optionally, you can include header files via target_include_directories()
# target_include_directories(chess_cpp PRIVATE src)

# TODO: Add tests and install targets if needed.
