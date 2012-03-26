/*
    Akonadi google tasks plasmoid - taskwidgetiteminfo.cpp
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


#include "taskwidgetiteminfo.h"

#include <KIcon>

TaskWidgetItemInfo::TaskWidgetItemInfo(QGraphicsWidget * parent): Frame(parent)
{
    m_layout = new QGraphicsLinearLayout(Qt::Horizontal,this);
    
    m_icon = new Plasma::IconWidget(this);
    m_icon->setOrientation(Qt::Horizontal);
    m_icon->setMinimumSize(20,20);
    m_icon->setMaximumSize(20,20);
    
    m_name = new Plasma::IconWidget(this);
    m_name->setMinimumWidth(50);
    m_name->setMinimumHeight(15);
    m_name->setMaximumHeight(15);
    m_name->setOrientation(Qt::Horizontal);
    
    m_layout->addItem(m_icon);
    m_layout->addItem(m_name);
    
    setLayout(m_layout);
   
    setFrameShadow(Raised);
    
    connect(m_icon,SIGNAL(clicked()),SIGNAL(changeCheckstate()));
    connect(m_name,SIGNAL(clicked()),SIGNAL(textClicked()));
}

void TaskWidgetItemInfo::setIcon(QString icon)
{
    m_icon->setIcon(KIcon(icon));
}

void TaskWidgetItemInfo::setText(QString text)
{
    m_name->setText(text);
}