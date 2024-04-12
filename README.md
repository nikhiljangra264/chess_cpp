# Chess Engine in C++

chess_cpp is a free and simple UCI chess engine.

chess_cpp does not include a graphical user interface (GUI) that is required to display a chessboard and to make it easy to input moves. 
Read the documentation for your GUI of choice for information about how to use UCI Chess Engine with it.

For example Lucas Chess GUI: https://github.com/lukasmonk/lucaschessR

### Prerequisites

- CMake (version 3.16 or higher)
- C++ compiler (supporting C++11 or higher)

### Building

1. Clone this repository to your local machine: `git clone https://github.com/nikhiljangra264/chess_cpp`
1. Navigate to the root directory of the project: `cd chess_cpp`
1. Create a build directory: `mkdir build && cd build`
1. Generate build files: `cmake ..`
1. Build the project: `cmake --build .`
1. You can find executable in `src` directory.

### Testing

To run tests after building the project, you can use the following commands:

- Run all tests: `ctest`
- Run specific tests: `ctest -R <regex_pattern>`
- Run tests with output to console: `ctest --output-on-failure`

## Directory Structure

- `src/`: Contains the source code for the chess engine.
- `tests/`: Contains unit tests for the engine.

## Contributing

Contributions are welcome! If you'd like to contribute to this project, feel free to submit a pull request.

## License

This project is licensed under the GNU GENERAL PUBLIC LICENSE 3 - see the [LICENSE](LICENSE) file for details.

