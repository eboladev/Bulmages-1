enable_testing(true)

macro(add_test testname testsrc)
  set(test_${testname}_SRCS ${testsrc})
  set(QT_USE_QTMAIN TRUE)
  set(QT_USE_QTTEST TRUE)
  
  include(${QT_USE_FILE})
  include_directories(
	./
	${CMAKE_BINARY_DIR}/bulmalib/src/
	${CMAKE_SOURCE_DIR}/bulmalib/src/
	${CMAKE_BINARY_DIR}/bulmacont/src/
	${CMAKE_SOURCE_DIR}/bulmacont/src/
	${CMAKE_BINARY_DIR}/bulmafact/src/
	${CMAKE_SOURCE_DIR}/bulmafact/src/
	${CMAKE_BINARY_DIR}/bulmatpv/src/
	${CMAKE_SOURCE_DIR}/bulmatpv/src/
	)

  qt4_automoc(${test_${testname}_SRCS})
  add_executable(test_${testname} ${test_${testname}_SRCS})
  target_link_libraries(test_${testname} bulmalib libbulmacont libbulmafact
	libbulmatpv ${QT_QTCORE_LIBRARY} ${QT_LIBRARIES} ${PGSQL_LIBRARIES}
    ${QT_QTTEST_LIBRARY} ${QT_QTGUI_LIBRARY}
    ${GSOAP_LIBRARIES} ${QT_QTLOCATION_LIBRARY})
endmacro(add_test)

set(initial_test
  main.cpp
  test_initial.cpp)
add_test(initial "${initial_test}")
