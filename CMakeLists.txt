cmake_minimum_required(VERSION 3.16)
project(SudokuProject VERSION 0.0.1 LANGUAGES CXX)

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
