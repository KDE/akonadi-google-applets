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


#include "contactwidgetiteminfolabel.h"

#include <KIcon>

ContactWidgetItemInfoLabel::ContactWidgetItemInfoLabel(QGraphicsWidget * parent)
    : Plasma::Frame(parent),
      m_icon(0),
      m_layout(0)
{
    m_layout = new QGraphicsLinearLayout(this);
    
    m_icon = new Plasma::IconWidget(this);
    m_icon->setMinimumWidth(20);
    m_icon->setMaximumHeight(20);
    m_icon->setMinimumHeight(20);
    m_icon->setOrientation(Qt::Horizontal);
    
    m_layout->addItem(m_icon);
    
    setLayout(m_layout);
    
    setFrameShadow(Raised);
    
    connect(m_icon,SIGNAL(clicked()),SLOT(clicked()));
}

void ContactWidgetItemInfoLabel::setText(const QString & value)
{
    m_icon->setText(value);
}

void ContactWidgetItemInfoLabel::setIcon(const QString & icon)
{
    m_icon->setIcon(KIcon(icon));
}

void ContactWidgetItemInfoLabel::clicked()
{
    emit clicked(m_icon->text());
}



