if(BUILD_TESTING)
    add_executable(castling_rights castling_rights.cpp)
    target_link_libraries(castling_rights gtest_main)

    add_test(NAME castling_rights_test COMMAND castling_rights)
endif()