#include "contactswidget.h"

ContactsWidget::ContactsWidget(QGraphicsWidget *parent)
{

    m_layout = new QGraphicsLinearLayout(Qt::Vertical,this); 

}

void ContactsWidget::addContact(ContactItem* item)
{

    m_layout->addItem(item);
 
}



