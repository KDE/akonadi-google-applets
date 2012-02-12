#include "contactswidget.h"

#include <KABC/Addressee>
#include <KABC/SortMode>

ContactsWidget::ContactsWidget(QGraphicsWidget *parent) : QGraphicsWidget(parent)
{
    m_layout = new QGraphicsLinearLayout(Qt::Vertical,this);
    
    setLayout(m_layout);

}

void ContactsWidget::addContact(ContactItem* item)
{
    
    for (int i = 0; i < m_layout->count(); i++) {
	
	if ((item->getAddressee()->name().toLower()) < (((ContactItem*)m_layout->itemAt(i))->getAddressee()->name().toLower())) {
	 
	   m_layout->insertItem(i,item);  
	   
	   return;
	   	    
	}
	   
    }
     
    m_layout->addItem(item); 
}


