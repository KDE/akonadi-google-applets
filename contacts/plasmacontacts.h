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

#include <Akonadi/CollectionFetchJob>
#include <Akonadi/Collection>

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
	
	void configChanged();
	
        KIcon m_icon;
	
	Plasma::ScrollWidget *m_scroll;
	Plasma::LineEdit *m_find;

	QGraphicsLinearLayout *m_layout;
	
	ContactsWidget *contact_list;
	
	Ui::config configDialog;
	
   public slots:
   
       	void configAccepted();
       
	void fetchCollections();
        void fetchCollectionsFinished(KJob *job);
       
        void lineChanged(const QString & text);
        void lineFocusChanged(bool change);
       	
};
 
K_EXPORT_PLASMA_APPLET(plasma_googgle_contacts, PlasmaContacts)


#endif 
