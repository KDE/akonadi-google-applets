/*
    Akonadi google tasks plasmoid - taskwidget.cpp
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


#include "taskwidget.h"

#include <Akonadi/Entity>
#include <Akonadi/ItemFetchScope>
#include <Akonadi/ItemFetchJob>

TaskWidget::TaskWidget(QGraphicsWidget* parent)
   : QGraphicsWidget(parent)
{
    
    m_layout = new QGraphicsLinearLayout(Qt::Vertical,this);
    m_layout->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
    
    setLayout(m_layout);

}

void TaskWidget::setCollection(Akonadi::Entity::Id id)
{
    clear();

    m_id = id;

    if (m_id != -1)

        fetchCollections();
    
}

void TaskWidget::fetchCollections()
{

    Akonadi::CollectionFetchJob * job = new Akonadi::CollectionFetchJob(Akonadi::Collection::root(), Akonadi::CollectionFetchJob::Recursive, this );

    job->fetchScope();

    connect(job,SIGNAL(result(KJob*)), SLOT(fetchCollectionsFinished(KJob*)));

}

void TaskWidget::fetchCollectionsFinished(KJob* job)
{
    if (job->error()) {

        qDebug() << "fetchCollections failed";

        return;
    }

    Akonadi::CollectionFetchJob *fetchJob = qobject_cast<Akonadi::CollectionFetchJob*>(job);

    const Akonadi::Collection::List collections = fetchJob->collections();

    foreach ( const Akonadi::Collection &collection, collections ) {

        if (collection.id() == m_id) {

            fetchItems(collection);

        }

    }

}

void TaskWidget::fetchItems(const Akonadi::Collection & collection)
{

    Akonadi::ItemFetchJob *job = new Akonadi::ItemFetchJob(collection);

    connect(job,SIGNAL(result(KJob*)), SLOT(fetchItemsFinished(KJob*)));

    job->fetchScope().fetchFullPayload(true);


}

void TaskWidget::fetchItemsFinished(KJob * job)
{
    if (job->error()) {

        qDebug() << "fetchItems failed";

        return;
    }

    Akonadi::ItemFetchJob *fetchJob = qobject_cast<Akonadi::ItemFetchJob*>(job);

    const Akonadi::Item::List items = fetchJob->items();

    foreach ( const Akonadi::Item &item, items ) {

        TaskWidgetItem * contact;

        contact = new TaskWidgetItem(item,this);

        addItem(contact);

    }

}

void TaskWidget::addItem(TaskWidgetItem* item)
{

    m_layout->addItem(item);

}

void TaskWidget::clear()
{

    TaskWidgetItem * item;

    while (m_layout->count() > 0) {

        item = static_cast<TaskWidgetItem*>(m_layout->itemAt(0));

        m_layout->removeItem(item);

        item->deleteLater();

    }
    
}

