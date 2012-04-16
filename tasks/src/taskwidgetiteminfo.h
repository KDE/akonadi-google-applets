/*
    Akonadi google tasks plasmoid - taskwidgetiteminfo.h
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


#ifndef TASKWIDGETITEMINFO_H
#define TASKWIDGETITEMINFO_H

#include <QGraphicsLinearLayout>

#include <Plasma/Frame>
#include <Plasma/IconWidget>

class TaskWidgetItemInfo : public Plasma::Frame
{
    Q_OBJECT
    
    public:
        
        explicit TaskWidgetItemInfo(QGraphicsWidget * parent = 0);
        virtual ~TaskWidgetItemInfo(){};
                
        void setCompleted(const bool & completed);
        void setText(const QString & text);
        void setCheckboxOrientation(const bool & orientation = false);
        
    signals:        
    
        void changeCheckstate();
        void textClicked();
        
    private:
        
        QGraphicsLinearLayout * m_layout;
        
        Plasma::IconWidget * m_icon;
        Plasma::IconWidget * m_name;
};

#endif // TASKWIDGETITEMINFO_H
