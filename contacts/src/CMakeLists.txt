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

set(contacts_SRCS contactwidgetiteminfolabel.cpp contactwidgetiteminfo.cpp contactslayout.cpp contactwidgetitem.cpp contactwidget.cpp plasmacontacts.cpp)

kde4_add_ui_files(contacts_SRCS ui/config.ui)

kde4_add_plugin(plasma_applet_contacts ${contacts_SRCS})
target_link_libraries(plasma_applet_contacts
                      ${KDE4_PLASMA_LIBS}
		      ${KDE4_KDEUI_LIBS}
		      ${KDEPIMLIBS_KABC_LIBS}
                      ${KDEPIMLIBS_AKONADI_LIBS}
		      ${KDE4_KDECORE_LIBS}
		      ${KDE4_KIO_LIBS} 
		      ${KDEPIMLIBS_AKONADI_CONTACT_LIBS})

install(TARGETS plasma_applet_contacts
        DESTINATION ${PLUGIN_INSTALL_DIR})
 
install(FILES plasma-applet-contacts.desktop
        DESTINATION ${SERVICES_INSTALL_DIR})

install(FILES
	im-icons/aim.png
	im-icons/cell-phone.png
	im-icons/email.png
	im-icons/gadu.png
	im-icons/home-phone.png
	im-icons/homepage.png
	im-icons/icq.png
	im-icons/irc.png
	im-icons/jabber.png
	im-icons/msn.png
	im-icons/skype.png
	im-icons/work-phone.png
	DESTINATION ${SHARE_INSTALL_PREFIX}/icons/hicolor/32x32/apps/
	)
	
kde4_install_icons(${ICON_INSTALL_DIR})