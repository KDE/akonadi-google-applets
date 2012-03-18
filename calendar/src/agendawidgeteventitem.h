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

#include <Plasma/IconWidget>

#include <Akonadi/Item>
#include <KCalCore/Event>

class AgendaWidgetEventItem : public Plasma::IconWidget
{
    Q_OBJECT
    
    public:
	
        explicit AgendaWidgetEventItem(const Akonadi::Item & item, QGraphicsItem * parent = 0);
        virtual ~AgendaWidgetEventItem(){};
	
    private:
	
	Akonadi::Item m_item;
	
	KCalCore::Event::Ptr m_event;
	
};

#endif // AGENDAWIDGETEVENTITEM_H
