/*
    Akonadi google calendar plasmoid - agendawidgetdateitem.cpp
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

AgendaWidgetDateItem::AgendaWidgetDateItem(KDateTime date, QGraphicsWidget * parent)
    : Plasma::Frame(parent)
{
    date.setDateOnly(true);
    m_date = date;
    
    m_layout = new QGraphicsLinearLayout(Qt::Vertical,this);
    
    m_dateIcon = new Plasma::IconWidget(this);
    m_dateIcon->setIcon(KIcon("view-calendar-day"));
    m_dateIcon->setOrientation(Qt::Horizontal);
    m_dateIcon->setMinimumWidth(50);
    m_dateIcon->setMaximumHeight(20);
    m_dateIcon->setTextBackgroundColor(Qt::red);
    m_dateIcon->setText(m_date.toString(KDateTime::LocalDate));
    
    m_layout->addItem(m_dateIcon);
    
    setLayout(m_layout);
}

void AgendaWidgetDateItem::addEvent(AgendaWidgetEventItem * event)
{

    m_layout->addItem(event);
    
}


void AgendaWidgetDateItem::setDate(KDateTime date)
{
    m_date = date;
    
    m_dateIcon->setText(m_date.toString(KDateTime::LocalDate));
}
