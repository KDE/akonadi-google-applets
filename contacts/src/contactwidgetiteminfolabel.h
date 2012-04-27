/*
    <one line to give the program's name and a brief idea of what it does.>
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


#ifndef CONTACTWIDGETITEMINFOLABEL_H
#define CONTACTWIDGETITEMINFOLABEL_H

#include <Plasma/Frame>
#include <Plasma/IconWidget>

#include <QGraphicsLinearLayout>

class ContactWidgetItemInfoLabel : public Plasma::Frame
{
    Q_OBJECT
    
    public:

        explicit ContactWidgetItemInfoLabel(QGraphicsWidget * parent = 0);
        virtual ~ContactWidgetItemInfoLabel(){};
        
        void setText(const QString & value);
        void setIcon(const QString & icon);
        
    private slots:

        void clicked();
        
    private:
            
        Plasma::IconWidget * m_icon;
        
        QGraphicsLinearLayout * m_layout;
        
    signals:
        
        void clicked(const QString & email);
};

#endif // CONTACTWIDGETITEMINFOLABEL_H
