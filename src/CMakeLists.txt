include_directories( .  ${QJSON_INCLUDE_DIR} ${MARBLE_INCLUDE_DIR} )
include(${QT_USE_FILE})

set(rockmarble_SRCS
  main.cpp
  mainwindow.cpp
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
qt4_add_resources(rockmarble_RCC_SRCS ../resources.qrc )


add_executable(rockmarble
   ${rockmarble_SRCS}
   ${rockmarble_UIS_H}
   ${rockmarble_MOC_SRCS}
   ${rockmarble_RCC_SRCS}
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
      
      set(ICON_INSTALL_DIR ${CMAKE_INSTALL_PREFIX}/share/icons)
      install (FILES ../icons/rockmarble_128x128.png DESTINATION ${ICON_INSTALL_DIR}/hicolor/128x128/apps/ RENAME rockmarble.png)
      install (FILES ../icons/rockmarble_64x64.png DESTINATION ${ICON_INSTALL_DIR}/hicolor/64x64/apps/ RENAME rockmarble.png)
      install (FILES ../icons/rockmarble_48x48.png DESTINATION ${ICON_INSTALL_DIR}/hicolor/48x48/apps/ RENAME rockmarble.png)
      install (FILES ../icons/rockmarble_32x32.png DESTINATION ${ICON_INSTALL_DIR}/hicolor/32x32/apps/ RENAME rockmarble.png)
      install (FILES ../icons/rockmarble_22x22.png DESTINATION ${ICON_INSTALL_DIR}/hicolor/22x22/apps/ RENAME rockmarble.png)
      install (FILES ../icons/rockmarble_16x16.png DESTINATION ${ICON_INSTALL_DIR}/hicolor/16x16/apps/ RENAME rockmarble.png)
    endif (APPLE)
endif(WIN32)
