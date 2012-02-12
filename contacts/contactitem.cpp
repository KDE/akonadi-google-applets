#include "contactitem.h"

#include <kicon.h>

#include <KABC/Picture>

ContactItem::ContactItem(KABC::Addressee * addr, QGraphicsWidget* parent): QGraphicsWidget(parent)
{    
    m_layout = new QGraphicsLinearLayout(Qt::Horizontal,this);
    
    contact_picture = new Plasma::IconWidget(this);
    contact_name = new Plasma::Label(this);
    
    addressee = addr;
        
    if (addr->photo().isEmpty()) {
		
	// TODO: 
	contact_picture->setIcon(KIcon("user-identity"));
	
    } else {

	QPixmap pixmap;
    
        pixmap.convertFromImage(addr->photo().data());
    
        contact_picture->setIcon(KIcon(QIcon(pixmap)));
	
    }
    
    if (addr->name().isEmpty()) {
    
        // TODO: 
	contact_name->setText("no name");
	
    } else {
	
	contact_name->setText(addr->name());
	
    }
    
    contact_name->setWordWrap(false);
    contact_name->setAlignment(Qt::AlignLeft);
    contact_name->setAlignment(Qt::AlignVCenter);
    contact_picture->setMinimumSize(50,50);
    contact_picture->setMaximumSize(50,50);
    contact_name->setMinimumSize(200,50);
    contact_name->setMaximumSize(2000,50);
    
    m_layout->addItem(contact_picture);
    m_layout->addItem(contact_name);
    
    m_layout->setSpacing(10);
    
    setLayout(m_layout);
    
    setMinimumSize(250,50);
}


