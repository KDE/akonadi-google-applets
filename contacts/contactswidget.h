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
    
    void setShowEmails(bool show = 1);
    void setShowNumbers(bool show = 1);

private:

    QGraphicsLinearLayout *m_layout;     
    
    bool m_showEmails;
    bool m_showNumbers;
    
};

#endif // CONTACTSWIDGET_H
