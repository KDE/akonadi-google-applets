project(plasma-contacts)

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

set(contacts_SRCS contactswidget.cpp contactitem.cpp plasma-contacts.cpp)
 
kde4_add_plugin(plasma_applet_contacts ${contacts_SRCS})
target_link_libraries(plasma_applet_contacts
                      ${KDE4_PLASMA_LIBS} ${KDE4_KDEUI_LIBS} ${KDEPIMLIBS_KABC_LIBS})
 
install(TARGETS plasma_applet_contacts
        DESTINATION ${PLUGIN_INSTALL_DIR})
 
install(FILES plasma-applet-contacts.desktop
        DESTINATION ${SERVICES_INSTALL_DIR})
 
