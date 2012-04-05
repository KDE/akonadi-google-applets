/*
    Akonadi google calendar plasmoid - calendarwidgetdayitem.h
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


#ifndef CALENDARWIDGETDAYITEM_H
#define CALENDARWIDGETDAYITEM_H

#include <Plasma/Frame>
#include <Plasma/IconWidget>

#include <QGraphicsLinearLayout>

class CalendarWidgetDayItem : public Plasma::IconWidget
{
    Q_OBJECT
    
    public:
        
        explicit CalendarWidgetDayItem(QGraphicsItem * parent = 0);
        virtual ~CalendarWidgetDayItem(){};
 
        void setDay(QDate date);
        void setActual(bool actual = true);
        
    private:
                
        QGraphicsLinearLayout * m_layout;
        
        QDate day;
        
};

#endif // CALENDARWIDGETDAYITEM_H
