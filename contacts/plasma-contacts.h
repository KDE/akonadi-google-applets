#ifndef PLASMA_CONTACTS_HEADER
#define PLASMA_CONTACTS_HEADER

#include <KIcon>

#include <Plasma/Applet>
#include <Plasma/ScrollWidget>

#include <QGraphicsLinearLayout>

#include "contactitem.h"
#include "contactswidget.h"
 
class PlasmaContacts : public Plasma::Applet
{
    Q_OBJECT
    public:
	
        PlasmaContacts(QObject *parent, const QVariantList &args);
        ~PlasmaContacts() {};
 
	void init();
 
    private:
        KIcon m_icon;
	
	Plasma::ScrollWidget *m_scroll;
	
	QGraphicsLinearLayout *m_layout;
	ContactsWidget *contact_list;
	
	
};
 
K_EXPORT_PLASMA_APPLET(plasma_contacts, PlasmaContacts)


#endif 
