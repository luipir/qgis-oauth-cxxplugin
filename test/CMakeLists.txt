# Standard includes and utils to compile into all tests.
SET (util_SRCS testutils.h qgstest.h)

#####################################################
# Don't forget to include output directory, otherwise
# the UI file won't be wrapped!
include_directories(SYSTEM
  ${Qt5Sql_INCLUDE_DIRS}
  ${QT_INCLUDE_DIR}
  ${QGIS_INCLUDE_DIR}
  ${QCA_INCLUDE_DIR}
  ${O2_INCLUDE_DIR}
  ${QJSON_INCLUDE_DIR}
  ../oauth2
)
include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}
)

#############################################################
# Compiler defines

# This define is used for tests that need to locate the test
# data under tests/testdata in the qgis source tree.
# the TEST_DATA_DIR variable is set in the top level CMakeLists.txt
ADD_DEFINITIONS(-DOAUTH2_TEST_DATA="\\"${CMAKE_CURRENT_SOURCE_DIR}/testdata\\"")

#ADD_DEFINITIONS(-DINSTALL_PREFIX="\\"${CMAKE_INSTALL_PREFIX}\\"")

#############################################################
# libraries

# because of htonl
IF (WIN32)
  SET(PLATFORM_LIBRARIES wsock32)
ENDIF (WIN32)

if(APPLE)
  add_definitions(-DQT_NO_CAST_TO_ASCII)
endif(APPLE)

add_definitions(-DCMAKE_BUILD_TYPE=RelWithDebInfo)
add_definitions(-UQT_NO_DEBUG_STREAM)
add_definitions(-UQT_NO_WARNING_OUTPUT)
add_definitions(-UQT_NO_DEBUG_OUTPUT)

macro (ADD_QGIS_TEST testname testsrc)

  add_executable(qgis_${testname} ${testsrc} ${util_SRCS})
  add_dependencies(qgis_${testname} oauth2authmethod_static)
  set_target_properties(qgis_${testname} PROPERTIES AUTOMOC TRUE)
  if(APPLE)
    target_link_libraries(qgis_${testname} ${APP_SERVICES_LIBRARY})
  endif(APPLE)
  if(EXISTS ${O2_LIBRARY_STATIC})
    # prefer static linking, so newer o2 lib does not need separately packaged
    # already linked in oauth2authmethod_static
    set(_o2_test_lib)
  else()
    set(_o2_test_lib ${O2_LIBRARY})
  endif()
  target_link_libraries(qgis_${testname}    
    ${QGIS_CORE_LIBRARY}
    ${QGIS_GUI_LIBRARY}
    ${QT_QTCORE_LIBRARY}
    ${QT_QTGUI_LIBRARY}
    ${QT_QTNETWORK_LIBRARY}
    ${QT_QTXML_LIBRARY}
    ${QT_QTWEBKIT_LIBRARY}
    ${QT_QTMAIN_LIBRARY}
    ${QT_QTTEST_LIBRARY}
    ${_o2_test_lib}
    ${QJSON_LIBRARIES}
    oauth2authmethod_static
    )
  add_test(qgis_${testname} qgis_${testname} -maxwarnings 10000)
endmacro (ADD_QGIS_TEST)

#############################################################
# Tests:

ADD_QGIS_TEST(authoauth2configtest testqgsauthoauth2config.cpp)
