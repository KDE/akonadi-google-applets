/*
    Akonadi google calendar plasmoid - calendarwidgetdayitem.cpp
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

CalendarWidgetDayItem::CalendarWidgetDayItem(QGraphicsItem * parent)
    : IconWidget(parent),
    m_layout(0)
{
    setMinimumSize(10,10);
    setDrawBackground(true);
    setAutoFillBackground(true);
    
    connect(this,SIGNAL(clicked()),SLOT(clicked()));
}

void CalendarWidgetDayItem::setDay(QDate date)
{
    m_date = date;
    
    setText(QString::number(m_date.day()));
    
    update();
}

void CalendarWidgetDayItem::setActualMonth(bool actual)
{
    QColor clr(Qt::black);
    
    QPalette palette;
    palette = this->palette();
    
    if (actual) {
    
        clr.setAlphaF(0.5);
        clr = clr.lighter();
        
    } 
    
    palette.setColor(QPalette::Window,clr);
    this->setPalette(palette);
}

void CalendarWidgetDayItem::setActualDay()
{
    QColor clr(Qt::blue);
    clr.setAlphaF(0.3);
    QPalette palette;
    palette = this->palette();
    palette.setColor(QPalette::Window,clr);
    this->setPalette(palette);
    
}

void CalendarWidgetDayItem::setEvent(bool actualMonth)
{
    Q_UNUSED(actualMonth);
    
    QColor clr(Qt::red);
    
    QPalette palette;
    palette = this->palette();
    
    if (actualMonth) {
    
        clr = clr.darker();
        
    } else {
        
        clr.setAlphaF(0.5);       
        
    }
    
    palette.setColor(QPalette::Window,clr);
    this->setPalette(palette);
}

void CalendarWidgetDayItem::clicked()
{
    emit clicked(m_date);
}
