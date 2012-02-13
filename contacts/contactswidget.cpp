#include "contactswidget.h"

#include <KABC/Addressee>
#include <KABC/SortMode>

ContactsWidget::ContactsWidget(QGraphicsWidget *parent) : QGraphicsWidget(parent)
{
    m_layout = new QGraphicsLinearLayout(Qt::Vertical,this);
       
    setLayout(m_layout);

}

void ContactsWidget::addContact(ContactItem * item)
{
    
    for (int i = 0; i < m_layout->count(); i++) {
	
	if ((item->addressee()->name().toLower()) < (((ContactItem*)m_layout->itemAt(i))->addressee()->name().toLower())) {
	 
	   m_layout->insertItem(i,item);  
	   
	   return;
	   	    
	}
	   
    }
     
    m_layout->addItem(item); 
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

// TODO
void ContactsWidget::showContactsContainsPhone(bool show)
{

    Q_UNUSED(show);
    
}

void ContactsWidget::showContactsContainsMail(bool show)
{

    Q_UNUSED(show);
    
}



