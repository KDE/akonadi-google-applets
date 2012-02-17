#include <plasmacontacts.h>

#include <KABC/Addressee>

#include <Akonadi/Entity>
#include <Akonadi/ItemFetchScope>
#include <Akonadi/ItemFetchJob>


#include <Plasma/Theme>
#include <Plasma/IconWidget>

PlasmaContacts::PlasmaContacts(QObject *parent, const QVariantList &args):
        Plasma::Applet(parent, args)

{
    m_showEmails = true;
    m_showNumbers = true;
    m_id = -1;

    setAspectRatioMode(Plasma::IgnoreAspectRatio);
    setBackgroundHints(DefaultBackground);
    setMinimumHeight(300);
    setMinimumWidth(300);

}

void PlasmaContacts::init()
{
    //contact_list = new ContactsWidget(this);

    m_find = new Plasma::LineEdit(this);
    m_find->setClearButtonShown(true);
    m_find->setText(i18n(" Find "));

    m_layout = new QGraphicsLinearLayout();

    m_contactList = new QGraphicsWidget(this);
    m_contactList->setLayout(m_layout);

    m_scroll = new Plasma::ScrollWidget(this);
    m_scroll->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    m_scroll->setWidget(m_contactList);

    m_mainLayout = new QGraphicsLinearLayout(Qt::Vertical,this);
    m_mainLayout->addItem(m_find);
    m_mainLayout->addItem(m_scroll);

    setLayout(m_mainLayout);

    connect(m_find,SIGNAL(textChanged(QString)),SLOT(lineChanged(QString)));
    connect(m_find,SIGNAL(focusChanged(bool)),SLOT(lineFocusChanged(bool)));

    configChanged();

}

void PlasmaContacts::configChanged()
{

    KConfigGroup conf = config();

    if (conf.readEntry("showEmails",true) != m_showEmails) {
	
       m_showEmails = conf.readEntry("showEmails",true);
	
       changeTooltip();
       
    }
    
    if (conf.readEntry("showNumbers",true) != m_showNumbers) {
	
       m_showNumbers = conf.readEntry("showNumbers",true);
       
       changeTooltip();
	
    }
    
    if (conf.readEntry("collection",-1) != m_id) {
	
	m_id = conf.readEntry("collection",-1);

        fetchCollectionsForContacts();
	
    }
    
    if ((conf.readEntry("orientation",1)+1) != m_layout->orientation()) {
	
	m_layout->setOrientation((Qt::Orientation)(conf.readEntry("orientation",1)+1));
	
    }
}

void PlasmaContacts::createConfigurationInterface(KConfigDialog* parent)
{
    QWidget *widget = new QWidget(0);

    configDialog.setupUi(widget);

    KConfigGroup conf = config();

    configDialog.loadCollections->setIcon(KIcon("view-refresh"));
    configDialog.orientationBox->setCurrentIndex(conf.readEntry("orientation",0));
    configDialog.showEmails->setChecked(conf.readEntry("showEmails",true));
    configDialog.showNumbers->setChecked(conf.readEntry("showNumbers",true));

    connect(parent, SIGNAL(accepted()), this, SLOT(configAccepted()));
    connect(configDialog.loadCollections,SIGNAL(clicked(bool)),SLOT(fetchCollections()));

    parent->addPage(widget,"General",icon());
}

void PlasmaContacts::configAccepted()
{
    KConfigGroup conf = config();

    conf.writeEntry("collection",configDialog.collectionBox->currentText());
    conf.writeEntry("orientation",configDialog.orientationBox->currentIndex());
    conf.writeEntry("showEmails",configDialog.showEmails->isChecked());
    conf.writeEntry("showNumbers",configDialog.showNumbers->isChecked());

    emit configNeedsSaving();

}


void PlasmaContacts::fetchCollections()
{

    configDialog.collectionBox->clear();

    Akonadi::CollectionFetchJob * job = new Akonadi::CollectionFetchJob(Akonadi::Collection::root(), Akonadi::CollectionFetchJob::Recursive, this );

    job->fetchScope();

    connect(job,SIGNAL(result(KJob*)), SLOT(fetchCollectionsFinished(KJob*)));

}

void PlasmaContacts::fetchCollectionsFinished(KJob* job)
{

    if (job->error()) {

        qDebug() << "fetchCollections failed";

        return;
    }

    Akonadi::CollectionFetchJob *fetchJob = qobject_cast<Akonadi::CollectionFetchJob*>(job);

    const Akonadi::Collection::List collections = fetchJob->collections();

    foreach ( const Akonadi::Collection &collection, collections ) {

        if (collection.resource().contains("akonadi_googlecontacts_resource")) {

            configDialog.collectionBox->addItem(QString::number(collection.id()));

        }

    }

}


void PlasmaContacts::lineChanged(const QString & text)
{

    showContactsContainsText(text);

}

void PlasmaContacts::lineFocusChanged(bool change)
{

    if (change && m_find->text().contains(i18n(" Find "))) {

        m_find->setText("");

    }

}

void PlasmaContacts::fetchCollectionsForContacts()
{

    if (m_id == -1) {

        qDebug() << "No collection";

    }

    Akonadi::CollectionFetchJob * job = new Akonadi::CollectionFetchJob(Akonadi::Collection::root(), Akonadi::CollectionFetchJob::Recursive, this );

    job->fetchScope();

    connect(job,SIGNAL(result(KJob*)), SLOT(fetchCollectionsForContactsFinished(KJob*)));

}

void PlasmaContacts::fetchCollectionsForContactsFinished(KJob* job)
{
    if (job->error()) {

        qDebug() << "fetchCollections failed";

        return;
    }

    Akonadi::CollectionFetchJob *fetchJob = qobject_cast<Akonadi::CollectionFetchJob*>(job);

    const Akonadi::Collection::List collections = fetchJob->collections();

    foreach ( const Akonadi::Collection &collection, collections ) {

        if (collection.id() == m_id) {

            fetchItems(collection);

        }

    }

}

void PlasmaContacts::fetchItems(const Akonadi::Collection & collection)
{

    Akonadi::ItemFetchJob *job = new Akonadi::ItemFetchJob(collection);

    connect(job,SIGNAL(result(KJob*)), SLOT(fetchItemsFinished(KJob*)));

    job->fetchScope().fetchFullPayload(true);


}

void PlasmaContacts::fetchItemsFinished(KJob * job)
{
    if (job->error()) {

        qDebug() << "fetchItems failed";

        return;
    }

    Akonadi::ItemFetchJob *fetchJob = qobject_cast<Akonadi::ItemFetchJob*>(job);

    const Akonadi::Item::List items = fetchJob->items();

    foreach ( const Akonadi::Item &item, items ) {

        KABC::Addressee tmp = item.payload<KABC::Addressee>();

        KABC::Addressee * addr = new KABC::Addressee(tmp);

        ContactItem * item;

        item = new ContactItem(addr,this);

        addContact(item);

    }

}

void PlasmaContacts::addContact(ContactItem * item)
{
    
    for (int i = 0; i < m_layout->count(); i++) {
	
	if ((item->addressee()->name().toLower()) < (((ContactItem*)m_layout->itemAt(i))->addressee()->name().toLower())) {
	 
	   m_layout->insertItem(i,item);  
	   
	   item->setShowInfo(m_showEmails, m_showNumbers);
	   
	   return;
	   	    
	}
	   
    }
     
    m_layout->addItem(item); 
    
    item->setShowInfo(m_showEmails, m_showNumbers);
    
}

void PlasmaContacts::showContactsContainsText(const QString & string)
{
    
    for (int i = 0; i < m_layout->count(); i++) {
	
	((ContactItem*)m_layout->itemAt(i))->showContact();
		
	if (!((ContactItem*)m_layout->itemAt(i))->containsString(string)) {
	    
	    ((ContactItem*)m_layout->itemAt(i))->hideContact();
	}
	
    }
    
}

void PlasmaContacts::changeTooltip()
{
        
     for (int i = 0; i < m_layout->count(); i++) {
		
	 ((ContactItem*)m_layout->itemAt(i))->setShowInfo(m_showEmails,m_showNumbers);
	
     }
    
}


#include "plasmacontacts.moc"


