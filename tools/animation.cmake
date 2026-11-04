if(NOT PYTHON_EXECUTABLE)
   unset(PYTHON_EXECUTABLE)
   find_program(PYTHON_EXECUTABLE NAMES python3)
endif()
if(NOT PYTHON_EXECUTABLE)
   message(FATAL_ERROR "Animation generation requires Python 3 with Pillow")
endif()
execute_process(
   COMMAND "${CMAKE_COMMAND}" -E env
      QT_QPA_PLATFORM=offscreen QT_QUICK_BACKEND=software
      QT_QUICK_CONTROLS_STYLE=Basic QT_SCALE_FACTOR=1 QT_FONT_DPI=96
      "${PYTHON_EXECUTABLE}" "${SOURCE_DIR}/tools/animation.py"
      "${CAPTURE_TOOL}" "${SOURCE_DIR}/screenshots/demo.gif"
   RESULT_VARIABLE result
)
if(NOT result EQUAL 0)
   message(FATAL_ERROR "Animation generation failed: ${result}")
endif()
