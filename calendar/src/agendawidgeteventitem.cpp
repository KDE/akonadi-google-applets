/*
    Akonadi google calendar plasmoid - agendawidgeteventitem.cpp
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


#include "agendawidgeteventitem.h"

AgendaWidgetEventItem::AgendaWidgetEventItem(Akonadi::Entity::Id id,QGraphicsWidget * parent)
    : Plasma::Frame(parent),
      m_line(0),
      m_eventName(0),
      m_timeText(0),
      m_hasStartTime(false),
      m_hasEndTime(false)
{
    m_id = id;
    
    m_mainLayout = new QGraphicsLinearLayout(this);
    m_textLayout = new QGraphicsLinearLayout(Qt::Vertical, m_mainLayout);
    m_textLayout->setContentsMargins(5,2,2,2);

    m_line = new QGraphicsWidget();

    m_line->setMinimumHeight(35);
    m_line->setMaximumHeight(35);
    m_line->setMaximumWidth(7);
    m_line->setAutoFillBackground(true);

    QColor color = Qt::green;
    color.setAlphaF(0.5);
    QPalette palette;
    palette = m_line->palette();
    palette.setColor(QPalette::Window, color);
    m_line->setPalette(palette);

    m_mainLayout->addItem(m_line);
    m_mainLayout->setAlignment(m_line,Qt::AlignHCenter);
    m_mainLayout->addItem(m_textLayout);
    m_mainLayout->setAlignment(m_textLayout,Qt::AlignCenter);

    setFrameShadow(Raised);
    
    setLayout(m_mainLayout);
}
void AgendaWidgetEventItem::setColor(QString color)
{
    QColor clr = QColor(color);
    clr.setAlphaF(0.5);
    QPalette palette;
    palette = m_line->palette();
    palette.setColor(QPalette::Window, clr);
    m_line->setPalette(palette);
}

void AgendaWidgetEventItem::setEventName(QString name)
{
    if (m_eventName) {
	
	delete m_eventName;
	m_eventName = 0;
	
    }
    
    QFont font = this->font();
    font.setBold(true);
    
    m_eventName = new Plasma::IconWidget(this);
    m_eventName->setOrientation(Qt::Horizontal);
    m_eventName->setMinimumWidth(50);
    m_eventName->setMaximumHeight(15);
    m_eventName->setText(name);

    if (!m_timeText) {

        m_textLayout->addItem(m_eventName);
	
    } else {
	
	m_textLayout->insertItem(0,m_eventName);
	
    }
    
    m_textLayout->setAlignment(m_eventName,Qt::AlignCenter);
    
}

void AgendaWidgetEventItem::setEventTime(QTime start, QTime end)
{
    QString time;

    m_startTime = start;
    m_endTime = end;
    m_hasStartTime = true;
    m_hasEndTime = true;
    
    time += m_startTime.toString("hh:mm");
    time += " - ";
    time += m_endTime.toString("hh:mm");

    QFont font = this->font();
    font.setPointSize(font.pointSize()-2);
    
    if (m_timeText) {
	
	delete m_timeText;
	m_timeText = 0;
	
    }
    
    m_timeText = new Plasma::IconWidget(this);
    m_timeText->setOrientation(Qt::Horizontal);
    m_timeText->setMinimumWidth(50);
    m_timeText->setMaximumHeight(15);
    m_timeText->setText(time);
    m_timeText->setFont(font);
    
    m_textLayout->addItem(m_timeText);

}

void AgendaWidgetEventItem::setEventStartTime(QTime start)
{

    QString time;
    
    m_startTime = start;
    m_hasEndTime = false;
    m_hasStartTime = true;
    
    time += i18n("From");
    time += " " + m_startTime.toString("hh:mm");
    
    QFont font = this->font();
    font.setPointSize(font.pointSize()-2);
    
    if (m_timeText) {
	
	delete m_timeText;
	m_timeText = 0;
	
    }
    
    m_timeText = new Plasma::IconWidget(this);
    m_timeText->setOrientation(Qt::Horizontal);
    m_timeText->setMinimumWidth(50);
    m_timeText->setMaximumHeight(15);
    m_timeText->setText(time);
    m_timeText->setFont(font);
    
    m_textLayout->addItem(m_timeText);

}

void AgendaWidgetEventItem::setEventEndTime(QTime end)
{

    QString time;
    
    m_endTime = end;
    m_hasEndTime = true;
    m_hasStartTime = false;
    
    time += i18n("Till");
    time += " " + m_endTime.toString("hh:mm");
    
    if (m_timeText) {
	
	delete m_timeText;
	m_timeText = 0;
	
    }
    
    QFont font = this->font();
    font.setPointSize(font.pointSize()-2);
    
    m_timeText = new Plasma::IconWidget(this);
    m_timeText->setOrientation(Qt::Horizontal);
    m_timeText->setMinimumWidth(50);
    m_timeText->setMaximumHeight(15);
    m_timeText->setText(time);
    m_timeText->setFont(font);
    
    m_textLayout->addItem(m_timeText);

}

bool AgendaWidgetEventItem::operator<(AgendaWidgetEventItem * item)
{
    if (m_hasStartTime && item->m_hasStartTime) {
        
        if (m_startTime == item->m_startTime) {
            
            if (m_hasEndTime && item->m_hasEndTime) {
                
                return (m_endTime > item->m_endTime);
                
            } else {
                
                if (m_hasEndTime) {
                    
                    return false;
                    
                } else {
                    
                    return true;
                    
                }
                
            }
            
        } else {
            
            return (m_startTime > item->m_startTime);
            
        }
        
    }
    
    if (m_hasStartTime && !item->m_hasStartTime) {
        
        if (!m_hasEndTime && item->m_hasEndTime) {
            
            return (m_startTime > item->m_endTime);
            
        }
        
        return false;
        
    } else if (!m_hasStartTime && item->m_hasStartTime) {
        
        if (m_hasEndTime && !item->m_hasEndTime) {
                    
            return (m_endTime > item->m_startTime);
            
        }
        
        return true;
        
    }
    
    if (m_hasEndTime && item->m_hasEndTime) {
        
        if (m_endTime == item->m_endTime) {
            
            if (m_hasEndTime && item->m_hasEndTime) {
                
                return (m_endTime > item->m_endTime);
                
            } else {
                
                if (m_hasEndTime) {
                    
                    return false;
                    
                } else {
                    
                    return true;
                    
                }
                
            }
            
        } else {
            
            return (m_endTime < item->m_endTime);
            
        }
        
    }
    
    if (m_hasEndTime && !item->m_hasEndTime) {    
        
        return false;
        
    } else if (!m_hasEndTime && item->m_hasEndTime) {
        
        return true;
        
    }
        
    return (m_eventName->text().toLower() > item->m_eventName->text().toLower());

}

bool AgendaWidgetEventItem::operator==(Akonadi::Entity::Id id)
{
    return (m_id == id);
}
