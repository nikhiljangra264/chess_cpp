if(BUILD_TESTING)
    add_executable(castling_rights castling_rights.cpp "../src/lib/board.cpp"  "../src/lib/misc.cpp" "../src/lib/hashing.cpp")
    target_link_libraries(castling_rights gtest_main)
    
    add_executable(board_test board_test.cpp "../src/lib/board.cpp"  "../src/lib/misc.cpp" "../src/lib/hashing.cpp")
    target_link_libraries(board_test gtest_main)

    add_executable(moves_generator moves_generator.cpp "../src/lib/board.cpp"  "../src/lib/misc.cpp" "../src/lib/hashing.cpp")
    target_link_libraries(moves_generator gtest_main)

    add_test(NAME castling_rights_test COMMAND castling_rights)

    add_test(NAME moves_generator_test COMMAND moves_generator)

    add_test(NAME chess_board_test COMMAND board_test)
endif()