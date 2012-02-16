#include "contactswidget.h"

#include <Akonadi/ItemFetchJob>
#include <Akonadi/ItemFetchScope>
#include <Akonadi/Item>

#include <KABC/Addressee>
#include <KABC/SortMode>

ContactsWidget::ContactsWidget(QGraphicsItem* parent): QGraphicsWidget(parent), m_showEmails(true), m_showNumbers(true)
{
    
    m_layout = new QGraphicsLinearLayout(Qt::Vertical,this);
    
    setLayout(m_layout);

}

void ContactsWidget::setOrientation(Qt::Orientation orientation)
{
    
    if (m_layout->orientation() == orientation) {
	
	return;
	
    }
    
    if (m_layout->orientation() == Qt::Vertical) {
	
	m_layout->setOrientation(Qt::Horizontal);
	
	// TODO: resize
	
    } else {
	
	m_layout->setOrientation(Qt::Vertical);
	
	// TODO: resize

    }
    
}

void ContactsWidget::setShowEmails(bool show)
{
    m_showEmails = show;
    
    for (int i = 0; i < m_layout->count(); i++) {
		
	((ContactItem*)m_layout->itemAt(i))->setShowInfo(m_showEmails,m_showNumbers);
	
    }
    
}

void ContactsWidget::setShowNumbers(bool show)
{
    m_showNumbers = show;
    
    for (int i = 0; i < m_layout->count(); i++) {
		
	((ContactItem*)m_layout->itemAt(i))->setShowInfo(m_showEmails,m_showNumbers);
	
    }
    
}

void ContactsWidget::setCollectionId(Akonadi::Entity::Id id)
{

    m_collectionId = id;
    
    fetchCollections();
    
}

void ContactsWidget::fetchCollections()
{

    if (m_collectionId == -1) {
	
	qDebug() << "No collection";
	
    }
    
    Akonadi::CollectionFetchJob * job = new Akonadi::CollectionFetchJob(Akonadi::Collection::root(), Akonadi::CollectionFetchJob::Recursive, this );
    
    job->fetchScope();
    
    connect(job,SIGNAL(result(KJob*)), SLOT(fetchCollectionsFinished(KJob*)));
    
}

void ContactsWidget::fetchCollectionsFinished(KJob* job)
{

    if (job->error()) {
     
	qDebug() << "fetchCollections failed";
	
        return;
    }

   Akonadi::CollectionFetchJob *fetchJob = qobject_cast<Akonadi::CollectionFetchJob*>(job);

   const Akonadi::Collection::List collections = fetchJob->collections();
   
   foreach ( const Akonadi::Collection &collection, collections ) {
       
      if (collection.id() == m_collectionId) {
	  	  
	  fetchItems(collection);
	  
      }
      
   }
   
}



void ContactsWidget::fetchItems(const Akonadi::Collection & collection)
{
    
    Akonadi::ItemFetchJob *job = new Akonadi::ItemFetchJob(collection);
    
    connect(job,SIGNAL(result(KJob*)), SLOT(fetchItemsFinished(KJob*)));

    job->fetchScope().fetchFullPayload(true);


}

void ContactsWidget::fetchItemsFinished(KJob * job)
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

void ContactsWidget::addContact(ContactItem * item)
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

void ContactsWidget::showContactsContainsText(const QString & string)
{
    
    for (int i = 0; i < m_layout->count(); i++) {
	
	((ContactItem*)m_layout->itemAt(i))->showContact();
		
	if (!((ContactItem*)m_layout->itemAt(i))->containsString(string)) {
	    
	    ((ContactItem*)m_layout->itemAt(i))->hideContact();
	}
	
    }
    

}






