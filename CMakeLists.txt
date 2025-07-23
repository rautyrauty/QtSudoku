cmake_minimum_required(VERSION 3.16)
project(SudokuProject LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Quick)

file(GLOB_RECURSE CPP_SOURCES CONFIGURE_DEPENDS src/*.cpp src/*.h)

set(QRC_FILE src/qml.qrc)

add_executable(SudokuApp ${CPP_SOURCES} ${QRC_FILE})

target_compile_definitions(SudokuApp PRIVATE QT_QML_DEBUG)
target_link_libraries(SudokuApp PRIVATE Qt6::Quick)

set_target_properties(SudokuApp PROPERTIES
   AUTOMOC ON
   AUTORCC ON
   AUTOUIC ON
)

target_compile_definitions(SudokuApp PRIVATE QT_QUICK_LIB)
