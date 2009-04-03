# - Try to find the libmarblewidget libraries
# Once done this will define
#
# MARBLE_FOUND - system has libmarble
# MARBLE_INCLUDE_DIR - the libmarble include directory
# MARBLE_LIBRARIES - libmarble library
FIND_PATH(MARBLE_INCLUDE_DIR MarbleWidget.h PATHS /usr/include/marble /usr/local/include/marble ${CMAKE_PREFIX_PATH}/include/marble )
FIND_LIBRARY(MARBLE_LIBRARIES NAMES marblewidget PATHS  ${CMAKE_PREFIX_PATH}/lib )

IF(MARBLE_INCLUDE_DIR AND MARBLE_LIBRARIES)
  SET(MARBLE_FOUND 1)
  #remove last /marble string
  STRING(REGEX REPLACE "/marble" "" MARBLE_INCLUDE_DIR_SUP_LEVEL ${MARBLE_INCLUDE_DIR})
  SET (MARBLE_INCLUDE_DIR ${MARBLE_INCLUDE_DIR_SUP_LEVEL} ${MARBLE_INCLUDE_DIR} )
  if(NOT Marble_FIND_QUIETLY)
   message(STATUS "Found marble: ${MARBLE_LIBRARIES}")
  endif(NOT Marble_FIND_QUIETLY)
ELSE(MARBLE_INCLUDE_DIR AND MARBLE_LIBRARIES)
  SET(MARBLE_FOUND 0 CACHE BOOL "Not found marble library")
  message(STATUS "NOT Found marble, disabling it")
	IF (Marble_FIND_REQUIRED)
      MESSAGE(FATAL_ERROR "Could not find Marble")
  ENDIF (Marble_FIND_REQUIRED)
ENDIF(MARBLE_INCLUDE_DIR AND MARBLE_LIBRARIES)

MARK_AS_ADVANCED(MARBLE_INCLUDE_DIR MARBLE_LIBRARIES)
