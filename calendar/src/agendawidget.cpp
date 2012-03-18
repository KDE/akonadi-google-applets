/*
    Akonadi google calendar plasmoid - agendawidget.cpp
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


#include "agendawidget.h"

#include <Plasma/Label>

#include <Akonadi/Entity>
#include <Akonadi/ItemFetchScope>
#include <Akonadi/ItemFetchJob>

#include <KCalCore/Event>

AgendaWidget::AgendaWidget(QGraphicsItem * parent, Qt::WindowFlags wFlags)
    : QGraphicsWidget(parent, wFlags)
{
    m_layout = new AgendaWidgetLayout(Qt::Vertical,this);
	
    setLayout(m_layout);

}

void AgendaWidget::setCollections(QList< Akonadi::Entity::Id > ids)
{

    clear();
    
    m_idList = ids;
    
    if (!m_idList.isEmpty()) {

        fetchCollections();
	
    }
    
}

void AgendaWidget::fetchCollections()
{

    Akonadi::CollectionFetchJob * job = new Akonadi::CollectionFetchJob(Akonadi::Collection::root(), Akonadi::CollectionFetchJob::Recursive, this);

    job->fetchScope();

    connect(job, SIGNAL(result(KJob *)), SLOT(fetchCollectionsFinished(KJob *)));
    
}

void AgendaWidget::fetchCollectionsFinished(KJob * job)
{
    
    if (job->error()) {

        qDebug() << "fetchCollections failed";

        return;
    }

    Akonadi::CollectionFetchJob * fetchJob = qobject_cast<Akonadi::CollectionFetchJob *>(job);

    const Akonadi::Collection::List collections = fetchJob->collections();

    foreach (const Akonadi::Collection & collection, collections) {

        if (m_idList.contains(collection.id())) {

            fetchItems(collection);

        }

    }

}

void AgendaWidget::fetchItems(const Akonadi::Collection & collection)
{

    Akonadi::ItemFetchJob * job = new Akonadi::ItemFetchJob(collection);

    connect(job, SIGNAL(result(KJob *)), SLOT(fetchItemsFinished(KJob *)));

    job->fetchScope().fetchFullPayload(true);
    
}

void AgendaWidget::fetchItemsFinished(KJob * job)
{

    if (job->error()) {

        qDebug() << "fetchItems failed";

        return;
    }

    Akonadi::ItemFetchJob * fetchJob = qobject_cast<Akonadi::ItemFetchJob *>(job);

    const Akonadi::Item::List items = fetchJob->items();

    foreach (const Akonadi::Item & item, items) {

	if (item.hasPayload<KCalCore::Event::Ptr>()) {
	
	    addItem(item);
	    
	}

    }
    
}

void AgendaWidget::addItem(const Akonadi::Item & item)
{
    
    KDateTime min = KDateTime::currentLocalDateTime();
    KDateTime max = min.addDays(30);
    KDateTime eventDay;
    
    KCalCore::Event::Ptr event = item.payload<KCalCore::Event::Ptr>();
    
    eventDay = event->dtEnd();
    eventDay.setDateOnly(true);
    
    if (eventDay > max) {

	return;
	
    } else if (eventDay < min && !event->recurs()) {

	return;
	
    } else if (eventDay < min && event->recurs()) {
	
	KDateTime dt = event->recurrence()->getPreviousDateTime(max);
	dt.setDateOnly(true);
	
	if (dt < min || dt > max) {
	    
	    return;
	    
	}
	
	eventDay = dt;
	
    }
   
    AgendaWidgetDateItem * dateItem;
    AgendaWidgetEventItem * newEvent; 
    
    for (int i = 0; i < m_layout->count(); i++) {
	
	dateItem = static_cast<AgendaWidgetDateItem*>(m_layout->itemAt(i));

	if (dateItem->date() == eventDay) {
	 
	    newEvent = new AgendaWidgetEventItem(item,this);
	    
	    dateItem->addEvent(newEvent);
	    
	    return;
	}
	
    }
    
    dateItem = new AgendaWidgetDateItem(eventDay,this);
    
    newEvent = new AgendaWidgetEventItem(item,this);
	    
    dateItem->addEvent(newEvent);
    
    AgendaWidgetDateItem * dtItem;
    
    for (int i = 0; i < m_layout->count(); i++) {
	
	dtItem = static_cast<AgendaWidgetDateItem*>(m_layout->itemAt(i));
	
	if (dtItem->date() > dateItem->date()) {
	 
	    m_layout->insertItem(i,dateItem);
	    return;
	    
	}
	
    }
    
    m_layout->addItem(dateItem);
}

void AgendaWidget::clear()
{

    AgendaWidgetDateItem * item;

    while (m_layout->count() > 0) {

        item = static_cast<AgendaWidgetDateItem*>(m_layout->itemAt(0));

        m_layout->removeItem(item);

        item->deleteLater();

    }
    
}
