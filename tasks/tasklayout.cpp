/*
    Akonadi google tasks plasmoid - taskwidgetitem.cpp
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


#include "tasklayout.h"

TaskLayout::TaskLayout (Qt::Orientation orientation, QGraphicsLayoutItem *parent):
    QGraphicsLinearLayout (orientation, parent)
{

}

void TaskLayout::addItem(TaskWidgetItem * item)
{

    TaskWidgetItem * item2;

    if (!item->relatedTo().isEmpty()) {

        item->setRelated();

    }

    for (int i = 0; i < count(); i++) {

        item2 = static_cast<TaskWidgetItem*>(itemAt(i));

        if (item->operator<(item2)) {

            insertItem(i,item);

            return;

        }

    }

    QGraphicsLinearLayout::addItem(item);

    return;

}

void TaskLayout::clear()
{

    TaskWidgetItem * item;

    while (count() > 0) {

        item = static_cast<TaskWidgetItem*>(itemAt(0));

        removeItem(item);

        item->deleteLater();

    }

}

TaskLayout::~TaskLayout()
{

}
