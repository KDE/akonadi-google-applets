#include <plasmacontacts.h>

#include <KABC/Addressee>

#include <Akonadi/ItemFetchJob>
#include <Akonadi/ItemFetchScope>
#include <Akonadi/Item>

#include <Plasma/Theme>
#include <Plasma/IconWidget>

PlasmaContacts::PlasmaContacts(QObject *parent, const QVariantList &args): Plasma::Applet(parent, args), m_icon("user-identity")
{  
    setHasConfigurationInterface(false);  
    setAspectRatioMode(Plasma::IgnoreAspectRatio);
    setBackgroundHints(DefaultBackground);
    setMinimumHeight(300);
    setMinimumWidth(300);
  
}
 
void PlasmaContacts::init()
{
    m_show_numbers = new Plasma::IconWidget(this);
    m_show_numbers->setIcon(KIcon("call-start"));
    m_show_numbers->setDrawBackground(true);
    m_show_numbers->setMaximumHeight(30);
    
    m_show_emails = new Plasma::IconWidget(this);
    m_show_emails->setIcon(KIcon("mail-flag"));
    m_show_emails->setDrawBackground(true);
    m_show_emails->setMaximumHeight(30);
    
    m_line = new Plasma::LineEdit(this);
    m_line->setClearButtonShown(true);
    m_line->setText(" Hledat ");
    m_line->setMinimumHeight(35);
    
    m_buttons_layout = new QGraphicsLinearLayout(Qt::Horizontal);
    m_buttons_layout->addItem(m_line);
    m_buttons_layout->addItem(m_show_emails);
    m_buttons_layout->addItem(m_show_numbers);
    
    contact_list = new ContactsWidget(this);
    
    m_scroll = new Plasma::ScrollWidget(this);
    m_scroll->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    m_scroll->setWidget(contact_list);
    
    m_main_layout = new QGraphicsLinearLayout(Qt::Vertical,this);
    m_main_layout->addItem(m_buttons_layout);
    m_main_layout->addItem(m_scroll);

    setLayout(m_main_layout);
    
    connect(m_line,SIGNAL(textChanged(QString)),SLOT(lineChanged(QString)));
    connect(m_line,SIGNAL(focusChanged(bool)),SLOT(lineFocusChanged(bool)));
    connect(m_show_emails,SIGNAL(clicked()),SLOT(showEmails()));
    connect(m_show_numbers,SIGNAL(clicked()),SLOT(showNumbers()));
    
    fetchCollections();
    
} 

void PlasmaContacts::fetchCollections()
{

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
       
      if (collection.resource().contains("akonadi_googlecontacts_resource"))  
	
	  fetchItems(collection);
     
   }
}

void PlasmaContacts::fetchItems(const Akonadi::Collection & collection)
{
    
    Akonadi::ItemFetchJob *job = new Akonadi::ItemFetchJob(collection);
    
    connect(job,SIGNAL(result(KJob*)), SLOT(fetchItemsFinished(KJob*)));

    job->fetchScope().fetchFullPayload(true);


}

void PlasmaContacts::fetchItemsFinished(KJob* job)
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
	
     contact_list->addContact(item); 
     
   }

}

void PlasmaContacts::lineChanged(QString text)
{
	
    contact_list->showContactsContainsText(text);	
       
}

void PlasmaContacts::lineFocusChanged(bool change)
{
 
    if (change && m_line->text().contains(" Hledat ")) {
	
	m_line->setText("");
	
    }
    
}

// TODO
void PlasmaContacts::showEmails()
{

    
}

void PlasmaContacts::showNumbers()
{

}

 
#include "plasmacontacts.moc"

 
