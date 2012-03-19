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

AgendaWidgetDateItem::AgendaWidgetDateItem(QDate date, QGraphicsWidget * parent)
    : Plasma::Frame(parent)
{    
    m_layout = new QGraphicsLinearLayout(Qt::Vertical,this);
    
    m_dateLabel = new AgendaWidgetDateLabel(this);
    
    m_layout->addItem(m_dateLabel);
    
    setDate(date);
    
    setLayout(m_layout);
}

void AgendaWidgetDateItem::addEvent(AgendaWidgetEventItem * event)
{
    AgendaWidgetEventItem * item;
    
    for (int i = 1; i < m_layout->count(); i++) {
	
	item = static_cast<AgendaWidgetEventItem*>(m_layout->itemAt(i));
	
	if (item->operator<(event)) {
	 
	    m_layout->insertItem(i,event);
	    return;
	}
	
    }
    
    m_layout->addItem(event);
    
}

void AgendaWidgetDateItem::setDate(QDate date)
{
    m_date = date;

    QDate dt = KDateTime::currentLocalDateTime().date();
    
    if (m_date == dt) {
	
	m_dateLabel->setText(i18n("Today"));
	
    } else if (m_date == dt.addDays(1)) {
	
	m_dateLabel->setText(i18n("Tomorrow"));
	
    } else {
    
	m_dateLabel->setText(m_date.toString(Qt::DefaultLocaleLongDate));
	
    }
}
