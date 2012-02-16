#include <plasmacontacts.h>

#include <KABC/Addressee>
#include <Akonadi/Entity>

#include <Plasma/Theme>
#include <Plasma/IconWidget>

PlasmaContacts::PlasmaContacts(QObject *parent, const QVariantList &args): Plasma::Applet(parent, args), m_icon("user-identity")
{  
    setAspectRatioMode(Plasma::IgnoreAspectRatio);
    setBackgroundHints(DefaultBackground);
    setMinimumHeight(300);
    setMinimumWidth(300);
  
}
 
void PlasmaContacts::init()
{       
    contact_list = new ContactsWidget(this);
    
    m_find = new Plasma::LineEdit(this);
    m_find->setClearButtonShown(true);
    m_find->setText(i18n(" Find "));
    
    m_scroll = new Plasma::ScrollWidget(this);
    m_scroll->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    m_scroll->setWidget(contact_list);
    
    m_layout = new QGraphicsLinearLayout(Qt::Vertical,this);
    m_layout->addItem(m_find);
    m_layout->addItem(m_scroll);

    setLayout(m_layout);
    
    connect(m_find,SIGNAL(textChanged(QString)),SLOT(lineChanged(QString)));
    connect(m_find,SIGNAL(focusChanged(bool)),SLOT(lineFocusChanged(bool)));
                  
} 

void PlasmaContacts::configChanged()
{

    KConfigGroup conf = config();

    contact_list->setOrientation((Qt::Orientation)(conf.readEntry("orientation",1) + 1));
    contact_list->setShowEmails(conf.readEntry("showEmails",true));
    contact_list->setShowNumbers(conf.readEntry("showNumbers",true));
    contact_list->setCollectionId(conf.readEntry("collection",-1));
    
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
	
    contact_list->showContactsContainsText(text);	
       
}

void PlasmaContacts::lineFocusChanged(bool change)
{
 
    if (change && m_find->text().contains(i18n(" Find "))) {
	
	m_find->setText("");
	
    }
    
}
 
#include "plasmacontacts.moc"

 
