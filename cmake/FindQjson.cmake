# - Try to find the libqjson libraries
# Once done this will define
#
# QJSON_FOUND - system has libqjson
# QJSON_INCLUDE_DIR - the libqjson include directory
# QJSON_LIBRARIES - libqjson library
FIND_PATH(QJSON_INCLUDE_DIR json_driver.hh PATHS /usr/include/qjson /usr/local/include/qjson ${CMAKE_PREFIX_PATH}/include/qjson )
FIND_LIBRARY(QJSON_LIBRARIES NAMES qjson PATHS  ${CMAKE_PREFIX_PATH}/qjson )

IF(QJSON_INCLUDE_DIR AND QJSON_LIBRARIES)
  SET(QJSON_FOUND 1)
  #remove last /qjson string
  STRING(REGEX REPLACE "/qjson" "" QJSON_INCLUDE_DIR_SUP_LEVEL ${QJSON_INCLUDE_DIR})
  SET (QJSON_INCLUDE_DIR ${QJSON_INCLUDE_DIR_SUP_LEVEL} ${QJSON_INCLUDE_DIR} )
  if(NOT Qjson_FIND_QUIETLY)
   message(STATUS "Found qjson: ${QJSON_LIBRARIES}")
  endif(NOT Qjson_FIND_QUIETLY)
ELSE(QJSON_INCLUDE_DIR AND QJSON_LIBRARIES)
  SET(QJSON_FOUND 0 CACHE BOOL "Not found qjson library")
  message(STATUS "NOT Found qjson, disabling it")
	IF (Qjson_FIND_REQUIRED)
      MESSAGE(FATAL_ERROR "Could not find Qjson")
  ENDIF (Qjson_FIND_REQUIRED)
ENDIF(QJSON_INCLUDE_DIR AND QJSON_LIBRARIES)

MARK_AS_ADVANCED(QJSON_INCLUDE_DIR QJSON_LIBRARIES)
