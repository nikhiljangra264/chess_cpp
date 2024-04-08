# CMakeList.txt : CMake project for chess_cpp, include source and define
# project specific logic here.
#

# Add source to this project's executable.
add_executable (chess_cpp "chess_cpp.cpp" "chess_cpp.h" "src/board.cpp" "src/board.h" "src/engine.cpp" "src/engine.h" "src/evaluate.cpp" "src/evaluate.h" "src/header.h" "src/limit.h" "src/psqt.h" "src/uci.cpp" "src/uci.h")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET chess_cpp PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
