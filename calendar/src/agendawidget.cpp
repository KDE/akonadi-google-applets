/*
    Copyright (C) 2012  Jan Grulich <jgrulich@redhat.com>

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

AgendaWidget::AgendaWidget(QGraphicsItem * parent, Qt::WindowFlags wFlags):
    QGraphicsWidget(parent, wFlags),
    m_layout(new AgendaWidgetLayout(Qt::Vertical, this)),
    m_dateColor("#343E88"),
    m_upcomingDateColor("#C00000"),
    m_eventBackgroundColor("#303030"),
    m_weeks(1),
    m_upcomingDays(3),
    m_monitor(new Akonadi::Monitor),
    m_date(KDateTime::currentLocalDate())
{
    setLayout(m_layout);

    m_monitor->itemFetchScope().fetchFullPayload(true);

    connect(m_monitor, SIGNAL(itemAdded(Akonadi::Item, Akonadi::Collection)),
            SLOT(itemAdded(Akonadi::Item, Akonadi::Collection)));
    connect(m_monitor, SIGNAL(itemChanged(Akonadi::Item, QSet<QByteArray>)),
            SLOT(itemChanged(Akonadi::Item, QSet<QByteArray>)));
    connect(m_monitor, SIGNAL(itemRemoved(Akonadi::Item)),
            SLOT(itemRemoved(Akonadi::Item)));
}

void AgendaWidget::setCollections(const QList< Akonadi::Entity::Id > & ids)
{
    m_layout->clear();

    m_idList = ids;

    if (!m_idList.isEmpty())
        fetchCollections();
}

QList< Akonadi::Entity::Id > AgendaWidget::collections() const
{
    return m_idList;
}

void AgendaWidget::setDateColor(const QString & color)
{
    m_dateColor = color;
}

QString AgendaWidget::dateColor() const
{
    return m_dateColor;
}

void AgendaWidget::setCalendarsColors(const QMap< Akonadi::Entity::Id, QString > & colors)
{
    m_calendarsColors = colors;
}

QMap< Akonadi::Entity::Id, QString > AgendaWidget::calendarsColors() const
{
    return m_calendarsColors;
}

void AgendaWidget::setUpcomingDateColor(const QString & color)
{
    m_upcomingDateColor = color;
}

QString AgendaWidget::upcomingDateColor() const
{
    return m_upcomingDateColor;
}

void AgendaWidget::setEventBackgroundColor(const QString & color)
{
    m_eventBackgroundColor = color;
}

QString AgendaWidget::eventBackgroundColor() const
{
    return m_eventBackgroundColor;
}

void AgendaWidget::setWeeks(const int & weeks)
{
    m_weeks = weeks;
}

int AgendaWidget::weeks() const
{
    return m_weeks;
}

void AgendaWidget::setUpcomingDays(const int & days)
{
    m_upcomingDays = days;
}

int AgendaWidget::upcomingDays() const
{
    return m_upcomingDays;
}

void AgendaWidget::fetchCollections()
{
    Akonadi::CollectionFetchJob * job = new Akonadi::CollectionFetchJob(Akonadi::Collection::root(),
            Akonadi::CollectionFetchJob::Recursive, this);
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

    foreach(const Akonadi::Collection & collection, collections) {

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

    foreach(const Akonadi::Item & item, items) {

        if (item.hasPayload<KCalCore::Event::Ptr>()) {
            addItem(item);
        }
    }
}

void AgendaWidget::addItem(const Akonadi::Item & item)
{
    QDate min = m_date;
    QDate max = min.addDays(7 * m_weeks);

    KCalCore::Event::Ptr event = item.payload<KCalCore::Event::Ptr>();

    QDate dateStart = event->dtStart().toLocalZone().date();
    QDate dateEnd = event->dtEnd().toLocalZone().date();
    QDate date = dateStart;

    int daysTo = dateStart.daysTo(dateEnd);

    if (dateStart > max) {
        return;
    } else if (dateStart < min && dateEnd < min && !event->recurs()) {
        return;
    } else if (dateStart < min && event->recurs()) {
        date = event->recurrence()->getPreviousDateTime(KDateTime(min)).toLocalZone().date();

        if (date.addDays(daysTo) < min) {
            date = event->recurrence()->getNextDateTime(KDateTime(date)).toLocalZone().date();
        }

        if (date.addDays(daysTo) < min || date > max) {
            return;
        }
    }

    while (date < max) {
        AgendaWidgetEventItem * newEvent;
        AgendaWidgetDateItem * dateItem;

        for (int i = 1; i < daysTo; i++) {

            if (date.addDays(i) >= min  && date.addDays(i) <= max) {
                newEvent = new AgendaWidgetEventItem(item.id());
                newEvent->setEventName(event->summary());
                newEvent->setColor(m_calendarsColors[item.storageCollectionId()]);

                if (!m_layout->existDateItem(date.addDays(i))) {
                    dateItem = new AgendaWidgetDateItem(date.addDays(i) , this);
                    dateItem->setBackgroundColor(m_eventBackgroundColor);

                    if (min.daysTo(date.addDays(i)) < m_upcomingDays) {
                        dateItem->setDateColor(m_upcomingDateColor);
                    } else {
                        dateItem->setDateColor(m_dateColor);
                    }

                    m_layout->addDateItem(dateItem);

                }

                m_layout->addEventItem(date.addDays(i) , newEvent);
            }
        }

        if (dateStart == dateEnd) {
            newEvent = new AgendaWidgetEventItem(item.id());
            newEvent->setEventName(event->summary());
            newEvent->setColor(m_calendarsColors[item.storageCollectionId()]);

            if (!event->allDay()) {
                newEvent->setEventTime(event->dtStart().toLocalZone().time(), event->dtEnd().toLocalZone().time());
            }

            if (!m_layout->existDateItem(date)) {
                dateItem = new AgendaWidgetDateItem(date, this);
                dateItem->setBackgroundColor(m_eventBackgroundColor);

                if (min.daysTo(date) < m_upcomingDays) {
                    dateItem->setDateColor(m_upcomingDateColor);
                } else {
                    dateItem->setDateColor(m_dateColor);
                }

                m_layout->addDateItem(dateItem);
            }

            m_layout->addEventItem(date, newEvent);

        } else {

            if (date >= min) {
                newEvent = new AgendaWidgetEventItem(item.id());
                newEvent->setEventName(event->summary());
                newEvent->setColor(m_calendarsColors[item.storageCollectionId()]);

                if (!event->allDay()) {
                    newEvent->setEventStartTime(event->dtStart().toLocalZone().time());
                }

                if (!m_layout->existDateItem(date)) {
                    dateItem = new AgendaWidgetDateItem(date , this);
                    dateItem->setBackgroundColor(m_eventBackgroundColor);

                    if (min.daysTo(date) < m_upcomingDays) {
                        dateItem->setDateColor(m_upcomingDateColor);
                    } else {
                        dateItem->setDateColor(m_dateColor);
                    }

                    m_layout->addDateItem(dateItem);
                }

                m_layout->addEventItem(date, newEvent);
            }

            if (dateEnd <= max) {
                newEvent = new AgendaWidgetEventItem(item.id());
                newEvent->setEventName(event->summary());
                newEvent->setColor(m_calendarsColors[item.storageCollectionId()]);

                if (!event->allDay()) {
                    newEvent->setEventEndTime(event->dtEnd().toLocalZone().time());
                }

                if (!m_layout->existDateItem(date.addDays(daysTo))) {
                    dateItem = new AgendaWidgetDateItem(date.addDays(daysTo) , this);
                    dateItem->setBackgroundColor(m_eventBackgroundColor);

                    if (min.daysTo(date.addDays(daysTo)) < m_upcomingDays) {
                        dateItem->setDateColor(m_upcomingDateColor);
                    } else {
                        dateItem->setDateColor(m_dateColor);
                    }

                    m_layout->addDateItem(dateItem);
                }

                m_layout->addEventItem(date.addDays(daysTo), newEvent);
            }
        }

        if (event->recurs()) {
            date = event->recurrence()->getNextDateTime(KDateTime(date)).toLocalZone().date();

            if (date.isNull()) {
                return;
            }

        } else {
            return;
        }
    }
}

void AgendaWidget::itemAdded(const Akonadi::Item & item, const Akonadi::Collection & collection)
{
    if (!item.hasPayload<KCalCore::Event::Ptr>())
        return;

    if (m_idList.contains(collection.id()))
        addItem(item);
}

void AgendaWidget::itemChanged(const Akonadi::Item & item, QSet< QByteArray > array)
{
    Q_UNUSED(array);

    if (!item.hasPayload<KCalCore::Event::Ptr>())
        return;

    itemRemoved(item);
    itemAdded(item, item.parentCollection());
}

void AgendaWidget::itemRemoved(const Akonadi::Item & item)
{
    m_layout->removeEvent(item.id());
}

void AgendaWidget::setDate(const QDate& date)
{
    m_date = date;

    m_layout->clear();

    fetchCollections();
}
