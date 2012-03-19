/*
    Akonadi google calendar plasmoid - agendawidgetlayout.cpp
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


#include "agendawidgetlayout.h"

AgendaWidgetLayout::AgendaWidgetLayout(Qt::Orientation orientation, QGraphicsLayoutItem * parent)
    : QGraphicsLinearLayout(orientation, parent)
{
}

bool AgendaWidgetLayout::existDateItem(QDate date)
{

    AgendaWidgetDateItem * dateItem;

    for (int i = 0; i < count(); i++) {

        dateItem = static_cast<AgendaWidgetDateItem *>(itemAt(i));

        if (dateItem->date() == date) {

            return true;
        }

    }
    
    return false;
    
}


void AgendaWidgetLayout::addDateItem(AgendaWidgetDateItem * item)
{
    AgendaWidgetDateItem * dtItem;

    for (int i = 0; i < count(); i++) {

        dtItem = static_cast<AgendaWidgetDateItem *>(itemAt(i));

        if (dtItem->date() > item->date()) {

            insertItem(i, item);
            return;

        }

    }

    addItem(item);

}


void AgendaWidgetLayout::addEventItem(QDate date, AgendaWidgetEventItem * item)
{

    AgendaWidgetDateItem * dateItem;

    for (int i = 0; i < count(); i++) {

        dateItem = static_cast<AgendaWidgetDateItem *>(itemAt(i));

        if (dateItem->date() == date) {

            item->setParentItem(dateItem);
            dateItem->addEvent(item);

            return;
        }

    }
    
}


QSizeF AgendaWidgetLayout::sizeHint(Qt::SizeHint which, const QSizeF & constraint) const
{
    QSizeF hint;
    int cnt = count();

    if (cnt == 0) {

        hint.setHeight(0);

    } else {

        hint = QGraphicsLinearLayout::sizeHint(which, constraint);

    }

    return hint;
}
