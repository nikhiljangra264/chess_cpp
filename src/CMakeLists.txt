cmake_minimum_required(VERSION 3.16)

add_executable(chess_cpp 
    main.cpp
    board.cpp
    evaluate.cpp
    misc.cpp
    search.cpp
    uci.cpp
    tt.cpp
    hashing.cpp)
