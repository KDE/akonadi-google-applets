#include <plasma-contacts.h>

#include <KABC/Addressee>

#include <Akonadi/ItemFetchJob>
#include <Akonadi/ItemFetchScope>
#include <Akonadi/Item>

#include <Plasma/Theme>

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
        
    m_layout = new QGraphicsLinearLayout(Qt::Vertical,this);
    contact_list = new ContactsWidget(this);
    m_scroll = new Plasma::ScrollWidget(this);
    
    m_scroll->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    m_scroll->setWidget(contact_list);
    
    Plasma::Label * label = new Plasma::Label(this);
    label->setText("Contacts");
    label->setAlignment(Qt::AlignCenter);

    m_layout->addItem(label);
    m_layout->addItem(m_scroll);
    
    setLayout(m_layout);
    
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
 
#include "plasma-contacts.moc"

 
