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

#ifndef AGENDAWIDGET_H
#define AGENDAWIDGET_H

#include <QGraphicsWidget>
#include <QGraphicsLinearLayout>

#include <Akonadi/CollectionFetchJob>
#include <Akonadi/Collection>
#include <Akonadi/Monitor>

#include "agendawidgetlayout.h"
#include "agendawidgetdateitem.h"
#include "agendawidgeteventitem.h"

class AgendaWidget : public QGraphicsWidget
{
    Q_OBJECT

public:

    AgendaWidget(QGraphicsItem * parent = 0, Qt::WindowFlags wFlags = 0);
    virtual ~AgendaWidget() {};

    void setCollections(const QList<Akonadi::Collection::Id> & ids);
    QList<Akonadi::Collection::Id> collections() const;

    void setCalendarsColors(const QMap<Akonadi::Collection::Id, QString> & colors);
    QMap<Akonadi::Item::Id, QString> calendarsColors() const;

    void setDateColor(const QString  &color);
    QString dateColor() const;

    void setUpcomingDateColor(const QString & color);
    QString upcomingDateColor() const;

    void setEventBackgroundColor(const QString & color);
    QString eventBackgroundColor() const;

    void setWeeks(const int & weeks);
    int weeks() const;

    void setUpcomingDays(const int & days);
    int upcomingDays() const;

public slots:
    void setDate(const QDate & date);

    void fetchCollectionsFinished(KJob * job);
    void fetchItemsFinished(KJob * job);

    void itemAdded(const Akonadi::Item & item, const Akonadi::Collection & collection);
    void itemChanged(const Akonadi::Item & item, QSet< QByteArray > array);
    void itemRemoved(const Akonadi::Item & item);

private:
    void addItem(const Akonadi::Item & item);

    void fetchCollections();
    void fetchItems(const Akonadi::Collection & collections);

    AgendaWidgetLayout * m_layout;

    QList<Akonadi::Item::Id> m_idList;
    QMap<Akonadi::Item::Id, QString> m_calendarsColors;

    QString m_dateColor;
    QString m_upcomingDateColor;
    QString m_eventBackgroundColor;

    int m_weeks;
    int m_upcomingDays;

    Akonadi::Monitor * m_monitor;

    QDate m_date;
};

#endif // AGENDAWIDGET_H
