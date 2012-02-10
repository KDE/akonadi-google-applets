#include "contactitem.h"
#include <kicon.h>

ContactItem::ContactItem(QGraphicsWidget* parent): QGraphicsWidget(parent)
{
   
    m_layout = new QGraphicsLinearLayout(Qt::Horizontal,this);
    
    contact_picture = new Plasma::IconWidget(this);
    contact_name = new Plasma::Label(this);
    
    contact_picture->setIcon(KIcon("KDE"));
    contact_name->setText("Honza Grulich");
    
    m_layout->addItem(contact_picture);
    m_layout->addItem(contact_name);
    
    this->setLayout(m_layout);
    //this->setMinimumSize(200, 40);

}


