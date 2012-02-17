#ifndef PLASMA_CONTACTS_HEADER
#define PLASMA_CONTACTS_HEADER

#include <KConfigDialog>
#include <KIcon>
#include <KJob>

#include <Plasma/Applet>
#include <Plasma/ScrollWidget>
#include <Plasma/LineEdit>
#include <Plasma/PushButton>

#include <QGraphicsLinearLayout>
#include <QGraphicsWidget>

#include <Akonadi/CollectionFetchJob>
#include <Akonadi/Collection>
#include <Akonadi/Item>


#include "contactitem.h"
#include "contactswidget.h"
#include "ui_config.h"

class PlasmaContacts : public Plasma::Applet
{
    Q_OBJECT

    public:
	
        PlasmaContacts(QObject *parent, const QVariantList &args);
        ~PlasmaContacts() {};
 
	void createConfigurationInterface(KConfigDialog *parent);
	void init();
	 
    private:
	
	// Functions
	
	void addContact(ContactItem *item);
	void configChanged();
	void fetchItems(const Akonadi::Collection & collections);
	void fetchCollectionsForContacts();
	void changeTooltip();
	void showContactsContainsText(const QString & text);

	// Variables
	
	Plasma::ScrollWidget *m_scroll;
	Plasma::LineEdit *m_find;

	QGraphicsLinearLayout *m_layout;
	QGraphicsLinearLayout *m_mainLayout;
	
	QGraphicsWidget *m_contactList;
	
	Akonadi::Collection::Id m_id;
	
	bool m_showEmails;
	bool m_showNumbers;
	
	Ui::config configDialog;
	
	//ContactsWidget *contact_list;
	
   public slots:
   
       	void configAccepted();
	
	void fetchCollections();
        void fetchCollectionsFinished(KJob *job);
        void fetchCollectionsForContactsFinished(KJob *job);

	void fetchItemsFinished(KJob * job);

	void lineChanged(const QString & text);
        void lineFocusChanged(bool change);
       	
};
 
K_EXPORT_PLASMA_APPLET(plasma_googgle_contacts, PlasmaContacts)


#endif 
