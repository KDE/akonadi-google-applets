#ifndef PLASMA_CONTACTS_HEADER
#define PLASMA_CONTACTS_HEADER

#include <KIcon>
#include <KJob>

#include <Plasma/Applet>
#include <Plasma/ScrollWidget>

#include <QGraphicsLinearLayout>

#include "contactitem.h"
#include "contactswidget.h"

#include <Akonadi/CollectionFetchJob>
#include <Akonadi/Collection>
 
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

        void fetchCollections();
        void fetchItems(const Akonadi::Collection & collections);
	
   public slots:
   
       void fetchCollectionsFinished(KJob *job);
       void fetchItemsFinished(KJob * job);
	
	
};
 
K_EXPORT_PLASMA_APPLET(plasma_contacts, PlasmaContacts)


#endif 
