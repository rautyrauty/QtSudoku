find_package(Qt6 REQUIRED COMPONENTS Test)

option(SUDOKU_VALGRIND "Run the tests under valgrind memcheck" OFF)
set(SUDOKU_TEST_TIMEOUT 300)
if(SUDOKU_VALGRIND)
   set(SUDOKU_TEST_TIMEOUT 1800)
   find_program(VALGRIND_EXECUTABLE valgrind REQUIRED)
   set(SUDOKU_TEST_LAUNCHER ${VALGRIND_EXECUTABLE} -q --error-exitcode=99
      --leak-check=full --errors-for-leak-kinds=definite --track-origins=yes)
endif()

set(SUDOKU_TEST_ENV
   "QT_QPA_PLATFORM=offscreen"
   "ASAN_OPTIONS=detect_leaks=1:detect_stack_use_after_return=1:check_initialization_order=1:strict_init_order=1:strict_string_checks=1:abort_on_error=1"
   "UBSAN_OPTIONS=print_stacktrace=1:halt_on_error=1"
   "TSAN_OPTIONS=halt_on_error=1:second_deadlock_stack=1:report_thread_leaks=0"
)

foreach(name tst_cell tst_sudoku tst_blockmodel tst_threads)
   add_executable(${name} ${name}.cpp testutils.h)
   target_link_libraries(${name} PRIVATE sudoku_core Qt6::Test)
   add_test(NAME ${name} COMMAND ${SUDOKU_TEST_LAUNCHER} $<TARGET_FILE:${name}>)
   set_tests_properties(${name} PROPERTIES
      ENVIRONMENT "${SUDOKU_TEST_ENV}"
      WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
      TIMEOUT ${SUDOKU_TEST_TIMEOUT}
   )
endforeach()
