#ifndef PLASMA_CONTACTS_HEADER
#define PLASMA_CONTACTS_HEADER

#include <KIcon>
#include <KJob>

#include <Plasma/Applet>
#include <Plasma/ScrollWidget>
#include <Plasma/LineEdit>
#include <Plasma/IconWidget>

#include <QGraphicsLinearLayout>

#include <Akonadi/CollectionFetchJob>
#include <Akonadi/Collection>

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
	Plasma::LineEdit *m_line;
	Plasma::IconWidget *m_show_emails;
	Plasma::IconWidget *m_show_numbers;
	
	QGraphicsLinearLayout *m_main_layout;
	QGraphicsLinearLayout *m_buttons_layout;
	
	ContactsWidget *contact_list;

        void fetchCollections();
        void fetchItems(const Akonadi::Collection & collections);
	
   public slots:
   
       void fetchCollectionsFinished(KJob *job);
       void fetchItemsFinished(KJob * job);
       
       void lineChanged(const QString & text);
       void lineFocusChanged(bool change);
       
       // TODO
       void showEmails();
       void showNumbers();	
	
};
 
K_EXPORT_PLASMA_APPLET(plasma_googgle_contacts, PlasmaContacts)


#endif 
