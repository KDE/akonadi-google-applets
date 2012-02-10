#ifndef CONTACTSWIDGET_H
#define CONTACTSWIDGET_H

#include <Plasma/ScrollWidget>
#include <QGraphicsWidget>
#include <QGraphicsLinearLayout>

#include "contactitem.h"

class ContactsWidget : public Plasma::ScrollWidget
{
    Q_OBJECT

public:
    ContactsWidget(QGraphicsWidget *parent = 0);
    
    ~ContactsWidget() { };
    
    void addContact(ContactItem *item);

private:

    QGraphicsLinearLayout *m_layout;       


};

#endif // CONTACTSWIDGET_H
