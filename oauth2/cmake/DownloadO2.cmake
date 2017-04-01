# Download Module for o2 Library Source
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# Copyright (c) March 2017, Boundless Spatial
# Author: Larry Shaffer <lshaffer (at) boundlessgeo (dot) com>
#
# Official o2 project source code repository: https://github.com/pipacs/o2
#
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.

# NOTE: These are temporary source code provenance endpoints, until necessary
#       commits are done upstream to support the OAuth2 plugin
set(_o2_commit b84a433947a9e423365a807f2438d7d179674860)
set(_o2_url "https://github.com/MonsantoCo/o2/archive/${_o2_commit}.tar.gz")
set(_o2_dl_file "${CMAKE_BINARY_DIR}/o2-${_o2_commit}.tar.gz")
set(_o2_dl_sha1 1abe4866ecd10ef6ca2c0e6e6c6f8eafab0d2cbd)
set(_o2_dl_timeout 45)
set(_o2_dl_inactive_timeout 30)

set(_o2_prefix "${CMAKE_BINARY_DIR}/o2-${_o2_commit}")

message(STATUS "Downloading or verifying o2 library source archive...")

file(DOWNLOAD ${_o2_url} ${_o2_dl_file}
   INACTIVITY_TIMEOUT ${_o2_dl_inactive_timeout}
   TIMEOUT ${_o2_dl_timeout}
   STATUS _o2_dl_status
   LOG _o2_dl_log
   #SHOW_PROGRESS
   EXPECTED_HASH SHA1=${_o2_dl_sha1}
   TLS_VERIFY on
   #TLS_CAINFO file
)

list(GET _o2_dl_status 0 _o2_dl_status_code)
list(GET _o2_dl_status 1 _o2_dl_status_string)

if(NOT "${_o2_dl_status_code}" STREQUAL "0")
  set(_o2_dl_log_file ${CMAKE_BINARY_DIR}/o2-download.log)
  set(_o2_dl_log_title "Error downloading or verifying o2 library source archive")
  set(_o2_dl_log_msg "${_o2_dl_log_title}
    from url=${_o2_url}
    to file =${_o2_dl_file}
    timeout =${_o2_dl_timeout} seconds
    status code: ${_o2_dl_status_code}
    status string: ${_o2_dl_status_string}
    connection log:
    ${_o2_dl_log}
  ")
  file(WRITE ${_o2_dl_file} "${_o2_dl_log_msg}")
  message(FATAL_ERROR "${_o2_dl_log_title}
      See log: ${_o2_dl_log_file}
  ")
endif()

if(NOT EXISTS ${_o2_dl_file})
  message(FATAL_ERROR "Download file does not exist")
endif()

execute_process(COMMAND ${CMAKE_COMMAND} -E tar xfz ${_o2_dl_file})

# These match variables set by FindO2.cmake
set(O2_INCLUDE_DIR "${_o2_prefix}/src"  CACHE INTERNAL "Path to o2 library headers" FORCE)
set(O2_LIBRARY "" CACHE INTERNAL "Path to o2 built shared library" FORCE)
set(O2_LIBRARY_STATIC "" CACHE INTERNAL "Path to o2 built static library" FORCE)
set(O2_FOUND TRUE CACHE INTERNAL "Whether O2 has been found" FORCE)