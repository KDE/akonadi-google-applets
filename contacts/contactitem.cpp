#include "contactitem.h"

#include <KABC/Picture>

#include <kicon.h>

ContactItem::ContactItem(KABC::Addressee * addr, QGraphicsWidget* parent): QGraphicsWidget(parent)
{
   
    m_layout = new QGraphicsLinearLayout(Qt::Horizontal,this);
    
    contact_picture = new Plasma::IconWidget(this);
    contact_name = new Plasma::Label(this);
    
    addressee = addr;
    
    qDebug() << addr->hasCategory("photo");
    
    if (addr->photo().isEmpty()) {
		
	// TODO: kde icon just for now
	contact_picture->setIcon(KIcon("kde"));
	
    } else {

	QPixmap pixmap;
    
        pixmap.convertFromImage(addr->photo().data());
    
        contact_picture->setIcon(KIcon(QIcon(pixmap)));
	
    }
    
    if (addr->name().isEmpty()) {
    
        // TODO: set "Without name" just for now
	contact_name->setText("Without name");
	
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
    
    setLayout(m_layout);
    
    setMinimumSize(250,50);
}


