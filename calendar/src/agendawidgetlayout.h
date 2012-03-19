/*
    Akonadi google calendar plasmoid - agendawidgetlayout.h
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


#ifndef AGENDAWIDGETLAYOUT_H
#define AGENDAWIDGETLAYOUT_H

#include <QGraphicsLinearLayout>

#include "agendawidgetdateitem.h"
#include "agendawidgeteventitem.h"

class AgendaWidgetLayout : public QGraphicsLinearLayout
{
    public:

        AgendaWidgetLayout(Qt::Orientation orientation, QGraphicsLayoutItem * parent = 0);

        virtual ~AgendaWidgetLayout(){};

        virtual QSizeF sizeHint(Qt::SizeHint which, const QSizeF & constraint = QSizeF()) const;
	
	bool existDateItem(QDate date);
	
	void addDateItem(AgendaWidgetDateItem * item);
	void addEventItem(QDate date, AgendaWidgetEventItem * item);
};

#endif // AGENDAWIDGETLAYOUT_H
