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


#include "agendawidgetdateitem.h"

#include <KIcon>

AgendaWidgetDateItem::AgendaWidgetDateItem(const QDate & date, QGraphicsWidget * parent):
    Plasma::Frame(parent),
    m_layout(new QGraphicsLinearLayout(Qt::Vertical, this)),
    m_dateLabel(new AgendaWidgetDateLabel(this))
{
    m_layout->addItem(m_dateLabel);

    setDate(date);

    setLayout(m_layout);

    setAutoFillBackground(true);

    QColor color = QColor("#303030");
    color.setAlphaF(0.5);
    QPalette palette;
    palette = this->palette();
    palette.setColor(QPalette::Window, color);
    this->setPalette(palette);
}

void AgendaWidgetDateItem::addEvent(AgendaWidgetEventItem * event)
{
    AgendaWidgetEventItem * item;

    for (int i = 1; i < m_layout->count(); i++) {
        item = static_cast<AgendaWidgetEventItem*>(m_layout->itemAt(i));

        if (item->operator<(event)) {
            m_layout->insertItem(i, event);
            return;
        }
    }

    m_layout->addItem(event);
}

void AgendaWidgetDateItem::clear()
{
    AgendaWidgetEventItem * item;

    for (int i = 1; i < m_layout->count(); i++) {
        item = static_cast<AgendaWidgetEventItem*>(m_layout->itemAt(i));
        item->deleteLater();
        m_layout->removeItem(item);

        i--;
    }
}

void AgendaWidgetDateItem::setBackgroundColor(const QString & color)
{
    QColor clr = QColor(color);
    clr.setAlphaF(0.5);
    QPalette palette;
    palette = this->palette();
    palette.setColor(QPalette::Window, clr);
    this->setPalette(palette);
}

void AgendaWidgetDateItem::setDate(const QDate & date)
{
    m_date = date;

    m_dateLabel->setText(KGlobal::locale()->formatDate(date, KLocale::FancyLongDate));
}

void AgendaWidgetDateItem::setDateColor(const QString & color)
{
    m_dateLabel->setColor(color);
}

void AgendaWidgetDateItem::removeEvent(const Akonadi::Entity::Id & eventId)
{
    AgendaWidgetEventItem * item;

    for (int i = 1; i < m_layout->count(); i++) {
        item = static_cast<AgendaWidgetEventItem*>(m_layout->itemAt(i));

        if (item->operator==(eventId)) {
            m_layout->removeItem(item);
            item->deleteLater();

            i--;
        }
    }
}