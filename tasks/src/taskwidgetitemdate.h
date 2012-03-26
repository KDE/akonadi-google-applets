/*
    Akonadi google tasks plasmoid - taskwidgetitemdate.h
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


#ifndef TASKWIDGETITEMDATE_H
#define TASKWIDGETITEMDATE_H

#include <QGraphicsLinearLayout>

#include <Plasma/IconWidget>
#include <Plasma/Frame>

class TaskWidgetItemDate : public Plasma::Frame
{
    Q_OBJECT
    
    public:
        
        explicit TaskWidgetItemDate(QGraphicsWidget * parent = 0);
        virtual ~TaskWidgetItemDate() {};
                
        void setColor(const QString color);
        void setText(const QString date);
        
    signals:
        
        void dateClicked();
    
    private:
        
        QGraphicsLinearLayout * m_layout;
        
        Plasma::IconWidget * m_icon;
};

#endif // TASKWIDGETITEMDATE_H
