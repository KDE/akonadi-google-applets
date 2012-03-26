project(plasma-tasks)

cmake_minimum_required(VERSION 2.6)

find_package(Qt4 4.8.0 REQUIRED) 
find_package(KDE4 4.6.0 REQUIRED)
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

set(tasks_SRCS taskwidgetiteminfo.cpp taskwidgetitemdate.cpp taskeditor.cpp tasklayout.cpp taskwidget.cpp taskwidgetitem.cpp plasmatasks.cpp)

kde4_add_ui_files(tasks_SRCS config.ui)
kde4_add_ui_files(tasks_SRCS taskedit.ui)
kde4_add_ui_files(tasks_SRCS appearanceconfig.ui)
 
kde4_add_plugin(plasma_applet_google_tasks ${tasks_SRCS})
target_link_libraries(plasma_applet_google_tasks
                      ${KDE4_PLASMA_LIBS} 
		      ${KDE4_KDEUI_LIBS}
                      ${KDEPIMLIBS_AKONADI_LIBS} 
                      ${KDE4_KDECORE_LIBS}
                      ${KDE4_KCALCORE_LIBS})
 
install(TARGETS plasma_applet_google_tasks
        DESTINATION ${PLUGIN_INSTALL_DIR})
 
install(FILES plasma-applet-google-tasks.desktop
        DESTINATION ${SERVICES_INSTALL_DIR})
 
kde4_install_icons(${ICON_INSTALL_DIR})