#include <plasma-contacts.h>
#include <KABC/Addressee>
#include <Plasma/Theme>
#include <QPainter>

PlasmaContacts::PlasmaContacts(QObject *parent, const QVariantList &args): Plasma::Applet(parent, args)
{
    m_icon = KIcon("kde");
  
    setHasConfigurationInterface(false);  
    setAspectRatioMode(Plasma::IgnoreAspectRatio);
    setBackgroundHints(DefaultBackground);

}
 
void PlasmaContacts::init()
{
        
    m_layout = new QGraphicsLinearLayout(Qt::Vertical,this);
    contact_list = new ContactsWidget(this);
    m_scroll = new Plasma::ScrollWidget(this);
    
    m_scroll->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    m_scroll->setWidget(contact_list);
    
    m_layout->addItem(m_scroll);
    
    setLayout(m_layout);

    
    //TESTING
    ContactItem * item;
    KABC::Addressee * addr = new KABC::Addressee();
    
    for (int i = 0; i < 15; i++) {
	
	item = new ContactItem(addr,this);
	
	contact_list->addContact(item);
	
    }


} 
 
#include "plasma-contacts.moc"

 
