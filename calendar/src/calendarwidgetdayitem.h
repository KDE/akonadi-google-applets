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

#ifndef CALENDARWIDGETDAYITEM_H
#define CALENDARWIDGETDAYITEM_H

#include <Plasma/Frame>
#include <Plasma/Label>

#include <QGraphicsLinearLayout>
#include <QGraphicsWidget>
#include <QPainter>

class CalendarWidgetDayItem : public QGraphicsWidget
{
    Q_OBJECT

public:
    explicit CalendarWidgetDayItem(QGraphicsWidget * parent = 0);
    virtual ~CalendarWidgetDayItem() {};

    void setDate(const QDate & date);
    QDate date() const;

    void setHasEvent(const bool & event);
    bool hasEvent() const;

    void setColor(const QString & color);

signals:
    void clicked(const QDate & date);

private:
    void mousePressEvent(QGraphicsSceneMouseEvent * event);
    void hoverEnterEvent(QGraphicsSceneHoverEvent * event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent * event);

    Plasma::Label * m_icon;
    QGraphicsLinearLayout * m_layout;

    QColor m_color;
    QDate m_date;
    bool m_event;

};

#endif // CALENDARWIDGETDAYITEM_H
