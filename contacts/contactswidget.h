#ifndef CONTACTSWIDGET_H
#define CONTACTSWIDGET_H

#include <QGraphicsWidget>
#include <QGraphicsLinearLayout>
#include <QGraphicsLayoutItem>

#include <Akonadi/Collection>
#include <Akonadi/CollectionFetchJob>

#include <KJob>

#include "contactitem.h"

class ContactsWidget : public QGraphicsWidget
{
    Q_OBJECT

public:
    ContactsWidget(QGraphicsItem* parent = 0);
    
    ~ContactsWidget() { };
    
    void setOrientation(Qt::Orientation orientation = Qt::Vertical);
    void setShowEmails(bool show = 1);
    void setShowNumbers(bool show = 1);
    void setCollectionId(Akonadi::Collection::Id id);
    
    void showContactsContainsText(const QString & string);
    
    void fetchItems(const Akonadi::Collection & collections);
    
public slots:
    
    void fetchItemsFinished(KJob * job);
    void fetchCollectionsFinished(KJob *job);

private:

    QGraphicsLinearLayout *m_layout;     
    
    Akonadi::Collection::Id m_collectionId;
    bool m_showEmails;
    bool m_showNumbers;
    
    void fetchCollections();
    
    void addContact(ContactItem *item);

    
};

#endif // CONTACTSWIDGET_H
