#include "contactswidget.h"

#include <KABC/Addressee>
#include <KABC/SortMode>

ContactsWidget::ContactsWidget(QGraphicsWidget *parent) : QGraphicsWidget(parent), m_showEmails(1), m_showNumbers(1)
{
    m_layout = new QGraphicsLinearLayout(Qt::Vertical,this);
       
    setLayout(m_layout);

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





