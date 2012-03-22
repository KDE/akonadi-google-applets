/*
    Akonadi google calendar plasmoid - agendawidgetdatelabel.cpp
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


#include "agendawidgetdatelabel.h"

AgendaWidgetDateLabel::AgendaWidgetDateLabel(QGraphicsWidget * parent)
    : Plasma::Frame(parent)
{
    m_layout = new QGraphicsLinearLayout(this);
    
    m_icon = new Plasma::IconWidget(this);
    m_icon->setOrientation(Qt::Horizontal);
    m_icon->setMinimumWidth(50);
    m_icon->setMaximumHeight(20);
    
    m_layout->addItem(m_icon);
    
    setLayout(m_layout);
    setAutoFillBackground(true);
    
    QColor color("#343e88");
    color.setAlphaF(0.3);
    QPalette palette;
    palette = this->palette();
    palette.setColor(QPalette::Window,color);
    this->setPalette(palette);
    
    setFrameShadow(Raised);
}

void AgendaWidgetDateLabel::setText(QString date)
{
    m_icon->setText(date);   
}

void AgendaWidgetDateLabel::setColor(QString color)
{
    QColor clr(color);
    clr.setAlphaF(0.3);
    QPalette palette;
    palette = this->palette();
    palette.setColor(QPalette::Window,clr);
    this->setPalette(palette);
    
}