project(plasma-contacts)

cmake_minimum_required(VERSION 2.6)

find_package(Qt4 4.7.0 REQUIRED) 
find_package(KDE4 4.7.0 REQUIRED)
find_package(KdepimLibs REQUIRED)
include(KDE4Defaults)
 
add_definitions (${QT_DEFINITIONS} ${KDE4_DEFINITIONS})
include_directories(
   ${CMAKE_SOURCE_DIR}
   ${CMAKE_BINARY_DIR}
   ${KDE4_INCLUDES}
   ${KDEPIMLIBS_INCLUDE_DIRS}
   )

set(contacts_SRCS contactslayout.cpp contactwidgetitem.cpp contactwidget.cpp plasmacontacts.cpp)

kde4_add_ui_files(contacts_SRCS config.ui)
 
kde4_add_plugin(plasma_applet_google_contacts ${contacts_SRCS})
target_link_libraries(plasma_applet_google_contacts
                      ${KDE4_PLASMA_LIBS}
		      ${KDE4_KDEUI_LIBS}
		      ${KDEPIMLIBS_KABC_LIBS}
                      ${KDEPIMLIBS_AKONADI_LIBS}
		      ${KDE4_KDECORE_LIBS}
		      ${KDE4_KIO_LIBS} 
		      ${KDEPIMLIBS_AKONADI_CONTACT_LIBS})
 
install(TARGETS plasma_applet_google_contacts
        DESTINATION ${PLUGIN_INSTALL_DIR})
 
install(FILES plasma-applet-google-contacts.desktop
        DESTINATION ${SERVICES_INSTALL_DIR})
 
kde4_install_icons(${ICON_INSTALL_DIR})