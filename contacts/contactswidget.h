#ifndef CONTACTSWIDGET_H
#define CONTACTSWIDGET_H

#include <QGraphicsWidget>
#include <QGraphicsLinearLayout>
#include <QGraphicsLayoutItem>

#include "contactitem.h"

class ContactsWidget : public QGraphicsWidget
{
    Q_OBJECT

public:
    ContactsWidget(QGraphicsWidget *parent = 0);
    
    ~ContactsWidget() { };
    
    void addContact(ContactItem *item);
    void showContactsContains(QString string);

private:

    QGraphicsLinearLayout *m_layout;       

};

#endif // CONTACTSWIDGET_H
