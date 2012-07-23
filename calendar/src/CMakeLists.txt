project(plasma-akonadi-calendars)

cmake_minimum_required(VERSION 2.6)

find_package(Qt4 REQUIRED) 
find_package(KDE4 REQUIRED)
find_package(KdepimLibs REQUIRED)
include(KDE4Defaults)
 
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${KDE4_ENABLE_EXCEPTIONS}") 
 
add_definitions (${QT_DEFINITIONS} ${KDE4_DEFINITIONS})
include_directories(
   ${CMAKE_SOURCE_DIR}
   ${CMAKE_BINARY_DIR}
   ${KDE4_INCLUDES}
   ${KDEPIMLIBS_INCLUDE_DIRS}
   )

set(calendar_SRCS calendarconfig.cpp calendarwidgetdayitem.cpp calendarwidget.cpp agendaconfig.cpp agendawidgetdatelabel.cpp agendawidgetlayout.cpp agendawidget.cpp agendawidgeteventitem.cpp agendawidgetdateitem.cpp plasmacalendar.cpp)

kde4_add_ui_files(calendar_SRCS ui/config.ui)
kde4_add_ui_files(calendar_SRCS ui/agendaconfig.ui)
kde4_add_ui_files(calendar_SRCS ui/calendarconfig.ui)

kde4_add_plugin(plasma_applet_akonadi_calendars ${calendar_SRCS})
target_link_libraries(plasma_applet_akonadi_calendars
                      ${KDE4_PLASMA_LIBS}
		      ${KDE4_KDEUI_LIBS}
		      ${KDEPIMLIBS_KABC_LIBS}
                      ${KDEPIMLIBS_AKONADI_LIBS}
		      ${KDE4_KDECORE_LIBS}
		      ${KDE4_KIO_LIBS} 
		      ${KDEPIMLIBS_AKONADI_CONTACT_LIBS})
 
install(TARGETS plasma_applet_akonadi_calendars
        DESTINATION ${PLUGIN_INSTALL_DIR})
 
install(FILES plasma-applet-akonadi-calendars.desktop
        DESTINATION ${SERVICES_INSTALL_DIR})
 
kde4_install_icons(${ICON_INSTALL_DIR})