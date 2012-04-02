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
    : QGraphicsWidget(parent, wFlags),
      m_dateColor("#343E88"),
      m_weeks(1)
{
    m_layout = new AgendaWidgetLayout(Qt::Vertical,this);
	
    setLayout(m_layout);
    
    m_monitor = new Akonadi::Monitor();
    m_monitor->itemFetchScope().fetchFullPayload(true);

    connect(m_monitor, SIGNAL(itemAdded(Akonadi::Item, Akonadi::Collection)),
            SLOT(itemAdded(Akonadi::Item, Akonadi::Collection)));
    connect(m_monitor, SIGNAL(itemChanged(Akonadi::Item, QSet<QByteArray>)),
            SLOT(itemChanged(Akonadi::Item, QSet<QByteArray>)));
    connect(m_monitor, SIGNAL(itemRemoved(Akonadi::Item)),
            SLOT(itemRemoved(Akonadi::Item))); 

}

void AgendaWidget::setDateColor(QString color)
{

    m_dateColor = color;
    
}

void AgendaWidget::setWeeks(int weeks)
{
 
    m_weeks = weeks;
    
}

void AgendaWidget::setCalendarsColors(QMap< Akonadi::Entity::Id, QString > colors)
{

    m_calendarsColors = colors;
    
}


void AgendaWidget::setCollections(QList< Akonadi::Entity::Id > ids)
{

    m_layout->clear();
    
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

            m_monitor->setCollectionMonitored(collection);
            
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
    QTime time = KDateTime::currentLocalTime();
    QDate min = KDateTime::currentLocalDate();
    QDate max = min.addDays(7 * m_weeks); 
    
    KCalCore::Event::Ptr event = item.payload<KCalCore::Event::Ptr>();
    
    QDate dateStart = event->dtStart().date(); 
    QDate dateEnd = event->dtEnd().date();
    QDate date = dateStart;
    
    int daysTo = dateStart.daysTo(dateEnd );
         
    if (dateStart > max) {
	
	return;
	
    } else if (dateEnd == min && event->dtEnd().time() < time && !event->allDay()) {
        
        return;
        
    } else if (dateStart < min && dateEnd < min && !event->recurs()) {
	
	return;
	
    } else if (dateStart < min && event->recurs()) {

        date = event->recurrence()->getPreviousDateTime(KDateTime(min)).date();

        if (date.addDays(daysTo) < min) {
        
            date = event->recurrence()->getNextDateTime(KDateTime(date)).date();
        
        }
	
	if (date.addDays(daysTo) < min || date > max) {

            return;
	    
	}
	
    } 
            
    AgendaWidgetEventItem * newEvent;   
    AgendaWidgetDateItem * dateItem;
          
    for (int i = 1; i < daysTo; i++) {
	
        if (date.addDays(i) >= min  && date.addDays(i) <= max ) {

            newEvent = new AgendaWidgetEventItem(item.id());
            newEvent->setEventName(event->summary());
            newEvent->setColor(m_calendarsColors[item.storageCollectionId()]);
	
            if (!m_layout->existDateItem(date.addDays(i) )) {
	   
                dateItem = new AgendaWidgetDateItem(date.addDays(i) ,this);
                dateItem->setDateColor(m_dateColor);
                m_layout->addDateItem(dateItem);
	    
            }
	
            m_layout->addEventItem(date.addDays(i) ,newEvent);
            
        }
    }
    
    if (dateStart == dateEnd ) {

        newEvent = new AgendaWidgetEventItem(item.id());
	newEvent->setEventName(event->summary());
        newEvent->setColor(m_calendarsColors[item.storageCollectionId()]);
	
	if (!event->allDay()) {
	
	    newEvent->setEventTime(event->dtStart().time(),event->dtEnd().time());
	    
	}
	
	if (!m_layout->existDateItem(date)) {
	   
	    dateItem = new AgendaWidgetDateItem(date,this);
	    dateItem->setDateColor(m_dateColor);
	    m_layout->addDateItem(dateItem);
	    
	}
	
	m_layout->addEventItem(date,newEvent);
	
	return;
    }
    
    if (date >= min ) { 
         
        newEvent = new AgendaWidgetEventItem(item.id());
        newEvent->setEventName(event->summary());
        newEvent->setColor(m_calendarsColors[item.storageCollectionId()]);
    
        if (!event->allDay()) {
    
            newEvent->setEventStartTime(event->dtStart().time());
	
        }
    
        if (!m_layout->existDateItem(date )) {
	   
            dateItem = new AgendaWidgetDateItem(date ,this);
            dateItem->setDateColor(m_dateColor);
            m_layout->addDateItem(dateItem);
	    
        }
    
        m_layout->addEventItem(date,newEvent);
        
    } 

    if (dateEnd <= max ) {
    
        newEvent = new AgendaWidgetEventItem(item.id());
        newEvent->setEventName(event->summary());
        newEvent->setColor(m_calendarsColors[item.storageCollectionId()]);
    
        if (!event->allDay()) {
   
            newEvent->setEventEndTime(event->dtEnd().time());
            
        }
    
        if (!m_layout->existDateItem(date.addDays(daysTo) )) {
	   
            dateItem = new AgendaWidgetDateItem(date.addDays(daysTo) ,this);
            dateItem->setDateColor(m_dateColor);
            m_layout->addDateItem(dateItem);
	    
        }
    
        m_layout->addEventItem(date.addDays(daysTo),newEvent);
        
    }
      
}

void AgendaWidget::itemAdded(const Akonadi::Item & item, const Akonadi::Collection & collection)
{
    if (m_idList.contains(collection.id())) {
     
        addItem(item);
        
    }
}

void AgendaWidget::itemChanged(const Akonadi::Item & item, QSet< QByteArray > array)
{
    Q_UNUSED(array);
    
    itemRemoved(item);
    itemAdded(item,item.parentCollection());
}

void AgendaWidget::itemRemoved(const Akonadi::Item & item)
{
    m_layout->removeEvent(item.id());
}

