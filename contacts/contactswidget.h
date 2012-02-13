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
    
    void showContactsContainsText(const QString & string);
    // TODO:
    void showContactsContainsPhone(bool show);
    void showContactsContainsMail(bool show);

private:

    QGraphicsLinearLayout *m_layout;     
    
};

#endif // CONTACTSWIDGET_H
