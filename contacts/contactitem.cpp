#include "contactitem.h"
#include <kicon.h>

ContactItem::ContactItem(KABC::Addressee * addr, QGraphicsWidget* parent): QGraphicsWidget(parent)
{
   
    m_layout = new QGraphicsLinearLayout(Qt::Horizontal,this);
    
    contact_picture = new Plasma::IconWidget(this);
    contact_name = new Plasma::Label(this);
    
    // TESTING
    contact_picture->setIcon(KIcon("KDE"));
    contact_name->setText("Name LastName");
    
    contact_name->setWordWrap(false);
        
    m_layout->addItem(contact_picture);
    m_layout->addItem(contact_name);
    
    setLayout(m_layout);

    addressee = addr;
    
}


