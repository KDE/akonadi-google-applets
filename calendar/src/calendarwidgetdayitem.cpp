/*
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

#include "calendarwidgetdayitem.h"

CalendarWidgetDayItem::CalendarWidgetDayItem(QGraphicsWidget * parent):
    QGraphicsWidget(parent),
    m_icon(new Plasma::Label(this)),
    m_layout(new QGraphicsLinearLayout(this)),
    m_event(false)
{
    m_icon->setMinimumSize(25, 25);
    m_icon->setAlignment(Qt::AlignCenter);

    m_layout->setContentsMargins(0, 0, 0, 0);
    m_layout->addItem(m_icon);

    setLayout(m_layout);

    setAutoFillBackground(true);
    setAcceptHoverEvents(true);
}

void CalendarWidgetDayItem::setDate(const QDate & date)
{
    m_date = date;
    m_icon->setText(QString::number(m_date.day()));
}

QDate CalendarWidgetDayItem::date() const {
    return m_date;
}

void CalendarWidgetDayItem::setHasEvent(const bool & event)
{
    m_event = event;
}

bool CalendarWidgetDayItem::hasEvent() const {
    return m_event;
}

void CalendarWidgetDayItem::setColor(const QString & color)
{
    m_color = QColor(color);
    m_color.setAlphaF(0.5);
    QPalette palette;
    palette = this->palette();
    palette.setColor(QPalette::Window, m_color);
    this->setPalette(palette);
}

void CalendarWidgetDayItem::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    Q_UNUSED(event);

    emit clicked(m_date);
}

void CalendarWidgetDayItem::hoverEnterEvent(QGraphicsSceneHoverEvent* event)
{
    Q_UNUSED(event);

    m_color.setAlphaF(1);
    QPalette palette;
    palette = this->palette();
    palette.setColor(QPalette::Window, m_color);
    this->setPalette(palette);
}

void CalendarWidgetDayItem::hoverLeaveEvent(QGraphicsSceneHoverEvent* event)
{
    Q_UNUSED(event);

    m_color.setAlphaF(0.5);
    QPalette palette;
    palette = this->palette();
    palette.setColor(QPalette::Window, m_color);
    this->setPalette(palette);
}
