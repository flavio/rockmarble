PROJECT(rockmarble)
# Stop cmake 2.6 from whining
CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
IF(COMMAND cmake_policy)
  CMAKE_POLICY(SET CMP0003 NEW)
ENDIF(COMMAND cmake_policy)

#SET(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR})

set (CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# compile in debug mode
IF(NOT CMAKE_BUILD_TYPE)
  SET(CMAKE_BUILD_TYPE RelWithDebInfo CACHE STRING
      "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel."
      FORCE)
ENDIF(NOT CMAKE_BUILD_TYPE)

IF("${CMAKE_BUILD_TYPE}" MATCHES "^Rel.*")
  ADD_DEFINITIONS("-DQT_NO_DEBUG_OUTPUT")
ENDIF("${CMAKE_BUILD_TYPE}" MATCHES "^Rel.*")

# Find Qt4
FIND_PACKAGE( Qt4 REQUIRED )

# Find QJson
FIND_PACKAGE( Qjson REQUIRED )

# Find Marble
FIND_PACKAGE( Marble REQUIRED )


#add extra search paths for libraries and includes
SET (LIB_SUFFIX "" CACHE STRING "Define suffix of directory name (32/64)" )
SET (LIB_DESTINATION "${CMAKE_INSTALL_PREFIX}/lib${LIB_SUFFIX}" CACHE STRING "Library directory name" FORCE)


set (ROCKMARBLE_DATA_PATH ${data_dir})

# Always include srcdir and builddir in include path
# This saves typing ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY} in about every subdir
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# put the include dirs which are in the source or build tree
# before all other include dirs, so the headers in the sources
# are prefered over the already installed ones
set(CMAKE_INCLUDE_DIRECTORIES_PROJECT_BEFORE ON)

# stupid qtcreator... I have to keep everything under the same folder!

include_directories( .  ${QJSON_INCLUDE_DIR} ${MARBLE_INCLUDE_DIR} )
include(${QT_USE_FILE})

set(rockmarble_SRCS
  main.cpp
  mainwindow.cpp
  artist.cpp
  datafetcher.cpp
  event.cpp
  location.cpp
  eventmodel.cpp
  eventsortfilterproxymodel.cpp
  lineedit.cpp
)

set(rockmarble_MOC_HDRS
  mainwindow.h
  datafetcher.h
  lineedit.h
  eventsortfilterproxymodel.h
)
set(rockmarble_UIS
  mainwindow.ui
)

qt4_wrap_ui(rockmarble_UIS_H ${rockmarble_UIS})
qt4_wrap_cpp(rockmarble_MOC_SRCS ${rockmarble_MOC_HDRS})


add_executable(rockmarble
   ${rockmarble_SRCS}
   ${rockmarble_UIS_H}
   ${rockmarble_MOC_SRCS}
)

target_link_libraries(rockmarble ${QJSON_LIBRARIES} ${MARBLE_LIBRARIES} ${QT_LIBRARIES})

install(TARGETS rockmarble RUNTIME DESTINATION bin)

if(WIN32)
    install (TARGETS rockmarble RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX})
else(WIN32)
    if (APPLE)
      # No need for this when installing to a bundle
    else (APPLE)
      install (TARGETS rockmarble RUNTIME DESTINATION bin )
      install( FILES rockmarble.desktop  DESTINATION share/applications)
    endif (APPLE)
endif(WIN32)
