project(plasma-calendar)

cmake_minimum_required(VERSION 2.6)

find_package(Qt4 REQUIRED) 
find_package(KDE4 REQUIRED)
find_package(KdepimLibs REQUIRED)
include(KDE4Defaults)
 
add_definitions (${QT_DEFINITIONS} ${KDE4_DEFINITIONS})
include_directories(
   ${CMAKE_SOURCE_DIR}
   ${CMAKE_BINARY_DIR}
   ${KDE4_INCLUDES}
   ${KDEPIMLIBS_INCLUDE_DIRS}
   )

set(calendar_SRCS agendawidget.cpp plasmacalendar.cpp)

#kde4_add_ui_files(calendar_SRCS config.ui)
 
kde4_add_plugin(plasma_applet_google_calendar ${calendar_SRCS})
target_link_libraries(plasma_applet_google_calendar
                      ${KDE4_PLASMA_LIBS}
		      ${KDE4_KDEUI_LIBS}
		      ${KDEPIMLIBS_KABC_LIBS}
                      ${KDEPIMLIBS_AKONADI_LIBS}
		      ${KDE4_KDECORE_LIBS}
		      ${KDE4_KIO_LIBS} 
		      ${KDEPIMLIBS_AKONADI_CONTACT_LIBS})
 
install(TARGETS plasma_applet_google_calendar
        DESTINATION ${PLUGIN_INSTALL_DIR})
 
install(FILES plasma-applet-google-calendar.desktop
        DESTINATION ${SERVICES_INSTALL_DIR})
 
kde4_install_icons(${ICON_INSTALL_DIR})