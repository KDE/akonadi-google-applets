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

#ifndef AGENDAWIDGETDATEITEM_H
#define AGENDAWIDGETDATEITEM_H

#include <QGraphicsWidget>
#include <QGraphicsLinearLayout>

#include <Plasma/Frame>
#include <Plasma/IconWidget>

#include "agendawidgetdatelabel.h"
#include "agendawidgeteventitem.h"

class AgendaWidgetDateItem : public QGraphicsWidget
{
    Q_OBJECT

public:
    AgendaWidgetDateItem(const QDate & date, QGraphicsWidget * parent = 0);
    virtual ~AgendaWidgetDateItem() {};

    QDate date() const {
        return m_date;
    }

    bool isEmpty() const {
        return (m_layout->count() == 1);
    }

    void addEvent(AgendaWidgetEventItem * event);
    void clear();
    void setBackgroundColor(const QString & color);
    void setDate(const QDate & date);
    void setDateColor(const QString & color);
    void removeEvent(const Akonadi::Entity::Id & eventId);

private:
    QGraphicsLinearLayout * m_layout;
    AgendaWidgetDateLabel * m_dateLabel;

    QDate m_date;
};

#endif // AGENDAWIDGETDATELABEL_H