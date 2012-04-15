enable_testing(true)

macro(add_test testname testsrc)
  set(test_${testname}_SRCS ${testsrc})
  set(QT_USE_QTMAIN TRUE)
  set(QT_USE_QTTEST TRUE)
  
  include(${QT_USE_FILE})
  include_directories(./)

  qt4_automoc(${test_${testname}_SRCS})
  add_executable(test_${testname} ${test_${testname}_SRCS} ${test_additional})
  target_link_libraries(test_${testname} ${QT_QTCORE_LIBRARY}
    ${QT_QTTEST_LIBRARY} ${QT_QTGUI_LIBRARY}
    ${GSOAP_LIBRARIES} ${QT_QTLOCATION_LIBRARY})
endmacro(add_test)

set(test_additional)
add_test(initial test_initial.cpp)

