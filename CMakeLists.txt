cmake_minimum_required(VERSION 3.16)
project(SudokuProject VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(BUILD_TESTING "Build the unit tests" OFF)
set(SUDOKU_SANITIZERS "" CACHE STRING
   "Semicolon-separated list of sanitizers to build with, e.g. address;undefined")

if(SUDOKU_SANITIZERS)
   list(JOIN SUDOKU_SANITIZERS "," _sanitizers)
   add_compile_options(-fsanitize=${_sanitizers} -fno-sanitize-recover=all
      -fno-omit-frame-pointer -g)
   add_link_options(-fsanitize=${_sanitizers})
   add_compile_definitions(_GLIBCXX_ASSERTIONS)
endif()

find_package(Qt6 REQUIRED COMPONENTS Quick)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

file(GLOB_RECURSE CORE_SOURCES CONFIGURE_DEPENDS src/*.cpp src/*.h)
list(REMOVE_ITEM CORE_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp)

add_library(sudoku_core STATIC ${CORE_SOURCES})
target_include_directories(sudoku_core PUBLIC src)
target_link_libraries(sudoku_core PUBLIC Qt6::Quick)

set(QRC_FILE src/qml.qrc)

add_executable(SudokuApp src/main.cpp ${QRC_FILE})

target_compile_definitions(SudokuApp PRIVATE QT_QML_DEBUG)
target_link_libraries(SudokuApp PRIVATE sudoku_core)

target_compile_definitions(SudokuApp PRIVATE QT_QUICK_LIB)

if(BUILD_TESTING)
   enable_testing()
   add_subdirectory(tests)
endif()

include(GNUInstallDirs)

install(PROGRAMS $<TARGET_FILE:SudokuApp>
   DESTINATION ${CMAKE_INSTALL_BINDIR}
   RENAME qtsudoku
)

install(FILES qtsudoku.desktop
   DESTINATION ${CMAKE_INSTALL_DATADIR}/applications
)

foreach(size 16 32 48 64 128 256 512)
   install(FILES icons/${size}-apps-qtsudoku.png
      DESTINATION ${CMAKE_INSTALL_DATADIR}/icons/hicolor/${size}x${size}/apps
      RENAME qtsudoku.png
   )
endforeach()
