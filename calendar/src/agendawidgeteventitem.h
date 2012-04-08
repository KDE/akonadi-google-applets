/*
    Akonadi google calendar plasmoid - agendawidgeteventitem.h
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


#ifndef AGENDAWIDGETEVENTITEM_H
#define AGENDAWIDGETEVENTITEM_H

#include <QGraphicsWidget>
#include <QGraphicsLinearLayout>

#include <Plasma/Frame>
#include <Plasma/IconWidget>

#include <Akonadi/Entity>
#include <KCalCore/Event>

class AgendaWidgetEventItem : public Plasma::Frame
{
    Q_OBJECT
    
    public:
	
        explicit AgendaWidgetEventItem(Akonadi::Entity::Id id,QGraphicsWidget * parent = 0);
        virtual ~AgendaWidgetEventItem(){};
	
        Akonadi::Entity::Id id() const {
            return m_id;
        }
        
	bool operator<(AgendaWidgetEventItem * item);
        bool operator==(Akonadi::Entity::Id id);
	
        void setColor(const QString color);
	void setEventName(const QString name);
	void setEventTime(const QTime start, const QTime end);
	void setEventStartTime(const QTime start);
	void setEventEndTime(const QTime end);
	
    private:
	
	QGraphicsLinearLayout * m_mainLayout;
	QGraphicsLinearLayout * m_textLayout;
	
        QGraphicsWidget * m_line;
        
	Plasma::IconWidget * m_eventName;
	Plasma::IconWidget * m_timeText;
		
	QTime m_startTime;
	QTime m_endTime;
	
	bool m_hasStartTime;
	bool m_hasEndTime;
        
        Akonadi::Entity::Id m_id;
};

#endif // AGENDAWIDGETEVENTITEM_H
