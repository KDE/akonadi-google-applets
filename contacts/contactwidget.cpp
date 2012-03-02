/*
    Akonadi google contact plasmoid - contactwidget.cpp
    Copyright (C) 2012  Jan Grulich <grulja@gmail.com>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#include "contactwidget.h"

#include <Akonadi/Entity>
#include <Akonadi/ItemFetchScope>
#include <Akonadi/ItemFetchJob>
#include <QGraphicsOpacityEffect>

ContactWidget::ContactWidget(QGraphicsWidget* parent)
    : QGraphicsWidget(parent),
      m_findData(true),
      m_showEmptyContacts(true)

{
    m_layout = new ContactsLayout(Qt::Vertical, this);
    m_layout->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
    setLayout(m_layout);

    m_monitor = new Akonadi::Monitor();
    m_monitor->itemFetchScope().fetchFullPayload(true);

    connect(m_monitor, SIGNAL(itemAdded(Akonadi::Item,Akonadi::Collection)),
            SLOT(itemAdded(Akonadi::Item,Akonadi::Collection)));
    connect(m_monitor, SIGNAL(itemChanged(Akonadi::Item,QSet<QByteArray>)),
            SLOT(itemChanged(Akonadi::Item,QSet<QByteArray>)));
    connect(m_monitor, SIGNAL(itemRemoved(Akonadi::Item)),
            SLOT(itemRemoved(Akonadi::Item)));

}

void ContactWidget::setCollection(Akonadi::Collection::Id id)
{

    clear();

    m_id = id;

    if (m_id != -1)

        fetchCollections();

}

void ContactWidget::setFilterData(bool filter)
{

    m_findData = filter;

}

void ContactWidget::setShowEmptyContacts(bool show)
{

    if (m_showEmptyContacts != show) {

        m_showEmptyContacts = show;

        updateContacts();

    }

}

void ContactWidget::showContactsContains(const QString& text)
{
    while (!m_listFilterText.isEmpty()) {

        addItem(static_cast<ContactWidgetItem*>(m_listFilterText.first()));

        static_cast<ContactWidgetItem*>(m_listFilterText.first())->show();

        m_listFilterText.pop_front();

    }

    ContactWidgetItem * item;

    for (int i = 0; i < m_layout->count(); i++) {

        item = static_cast<ContactWidgetItem*>(m_layout->itemAt(i));

        if ( ((!item->hasStringInName(text)) && m_findData && (!item->hasStringInData(text))) ||
                ((!item->hasStringInName(text)) && !m_findData)) {

            item->hide();

            m_listFilterText.push_back(item);

            m_layout->removeItem(item);;

            i--;

        } else {
	    
            item->show();
	    
        }
    }


}

void ContactWidget::fetchCollections()
{

    Akonadi::CollectionFetchJob * job = new Akonadi::CollectionFetchJob(Akonadi::Collection::root(), Akonadi::CollectionFetchJob::Recursive, this );

    job->fetchScope();

    connect(job,SIGNAL(result(KJob*)), SLOT(fetchCollectionsFinished(KJob*)));

}

void ContactWidget::fetchCollectionsFinished(KJob* job)
{
    if (job->error()) {

        qDebug() << "fetchCollections failed";

        return;
    }

    Akonadi::CollectionFetchJob *fetchJob = qobject_cast<Akonadi::CollectionFetchJob*>(job);

    const Akonadi::Collection::List collections = fetchJob->collections();

    foreach ( const Akonadi::Collection &collection, collections ) {

        if (collection.id() == m_id) {

            m_monitor->setCollectionMonitored(collection,true);

            fetchItems(collection);

        }

    }

}

void ContactWidget::fetchItems(const Akonadi::Collection & collection)
{

    Akonadi::ItemFetchJob *job = new Akonadi::ItemFetchJob(collection);

    connect(job,SIGNAL(result(KJob*)), SLOT(fetchItemsFinished(KJob*)));

    job->fetchScope().fetchFullPayload(true);


}

void ContactWidget::fetchItemsFinished(KJob * job)
{
    if (job->error()) {

        qDebug() << "fetchItems failed";

        return;
    }

    Akonadi::ItemFetchJob *fetchJob = qobject_cast<Akonadi::ItemFetchJob*>(job);

    const Akonadi::Item::List items = fetchJob->items();

    foreach ( const Akonadi::Item &item, items ) {

        ContactWidgetItem * contact;

        contact = new ContactWidgetItem(item,this);

        addItem(contact);

    }

}

void ContactWidget::addItem(ContactWidgetItem* item)
{

    if (!m_showEmptyContacts && item->isEmpty()) {

        item->setVisible(false);
        m_listFilterEmpty.push_back(item);

        return;
    }

    ContactWidgetItem * tmpItem;

    for (int i = 0; i < m_layout->count(); i++) {

        tmpItem = static_cast<ContactWidgetItem*>(m_layout->itemAt(i));
	
        if (item->operator<(tmpItem)) {

            m_layout->insertItem(i,item);

            return;

        }

    }

    m_layout->addItem(item);
    item->show();

}

void ContactWidget::clear()
{

    ContactWidgetItem * item;

    while (m_layout->count() > 0) {

        item = static_cast<ContactWidgetItem*>(m_layout->itemAt(0));

        m_layout->removeItem(item);

        item->deleteLater();

    }

}

void ContactWidget::updateContacts()
{

    while (!m_listFilterEmpty.isEmpty()) {

        addItem(static_cast<ContactWidgetItem*>(m_listFilterEmpty.first()));

        static_cast<ContactWidgetItem*>(m_listFilterEmpty.first())->show();

        m_listFilterEmpty.pop_front();

    }

    ContactWidgetItem * item;

    if (!m_showEmptyContacts) {

        for (int i = 0; i < m_layout->count(); i++) {

            item = static_cast<ContactWidgetItem*>(m_layout->itemAt(i));

            if (item->isEmpty()) {

                item->hide();

                m_listFilterEmpty.push_back(item);

                m_layout->removeItem(item);;

                i--;
            }

        }

    }

}

void ContactWidget::itemAdded(const Akonadi::Item & item, const Akonadi::Collection  & collection)
{

    if (collection.id() == m_id) {

        ContactWidgetItem * contact;

        contact = new ContactWidgetItem(item,this);

        addItem(contact);

    }

}

void ContactWidget::itemChanged(const Akonadi::Item & item, QSet< QByteArray > array )
{

    Q_UNUSED(array);

    ContactWidgetItem * tmpItem;

    for (int i = 0; i < m_layout->count(); i++) {

        tmpItem = static_cast<ContactWidgetItem*>(m_layout->itemAt(i));

        if (tmpItem->operator=(item)) {

            tmpItem->updateContact(item);

            return;
        }

    }

}

void ContactWidget::itemRemoved(const Akonadi::Item & item)
{

    ContactWidgetItem * tmpItem;

    for (int i = 0; i < m_layout->count(); i++) {

        tmpItem = static_cast<ContactWidgetItem*>(m_layout->itemAt(i));

        if (tmpItem->operator=(item)) {

            tmpItem->hide();

            m_layout->removeItem(tmpItem);;

            tmpItem->deleteLater();

            return;
        }

    }

}


ContactWidget::~ContactWidget()
{

    delete m_monitor;

}
