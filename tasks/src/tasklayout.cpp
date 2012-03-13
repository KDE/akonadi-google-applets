/*
    Akonadi google tasks plasmoid - tasklayout.cpp
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

TaskLayout::TaskLayout(Qt::Orientation orientation, QGraphicsLayoutItem * parent)
   : QGraphicsLinearLayout(orientation, parent)
{

}

void TaskLayout::addItem(TaskWidgetItem * item)
{

    TaskWidgetItem * item2;

    if (!item->relatedTo().isEmpty()) {

        if (!hasParent(item)) {

            item->hide();
            m_list.push_back(item);

            return;

        } else {

            for (int i = 0; i < count(); i++) {

                item2 = static_cast<TaskWidgetItem *>(itemAt(i));

                if (item->operator<<(item2)) {

                    item->setRelated(item2);

                }
            }

        }

    }

    foreach (const int index, neighborsIndexes(item)) {

        item2 = static_cast<TaskWidgetItem *>(itemAt(index));

        if (item->operator<(item2)) {

            insertItem(index, item);

            updateHierarchy();

            return;
        }

    }

    insertItem(lastIndex(item), item);

    updateHierarchy();

    return;

}

void TaskLayout::updateHierarchy()
{
    for (int i = 0; i < m_list.count(); i++) {

        if (hasParent(m_list.at(i))) {

            TaskWidgetItem * item2 = m_list.at(i);

            m_list.removeAt(i);

            item2->show();

            addItem(item2);

            i--;

        }

    }

}

bool TaskLayout::hasParent(TaskWidgetItem * item)
{

    TaskWidgetItem * item2;

    for (int i = 0; i < count(); i++) {

        item2 = static_cast<TaskWidgetItem *>(itemAt(i));

        if (item->operator<<(item2)) {

            return true;

        }

    }

    return false;
}

QList< int > TaskLayout::neighborsIndexes(TaskWidgetItem * item)
{

    TaskWidgetItem * item2;
    QList<int> indexes;
    QString parent = item->relatedTo();

    for (int i = 0; i < count(); i++) {

        item2 = static_cast<TaskWidgetItem *>(itemAt(i));

        if (parent == item2->relatedTo()) {

            indexes.push_back(i);

        }

    }

    return indexes;

}

int TaskLayout::lastIndex(TaskWidgetItem * item)
{

    int i = 0;

    TaskWidgetItem * item2;
    int depth = 0;

    bool tmp = false;

    for (i = 0; i < count(); i++) {

        item2 = static_cast<TaskWidgetItem *>(itemAt(i));

        if (!tmp) {

            if (item->operator<<(item2)) {

                depth = item2->indent();
                tmp = true;

            }

        } else {

            if (depth >= item2->indent()) {

                return i;

            }

        }


    }

    return i;
}

void TaskLayout::updateItem(TaskWidgetItem * item)
{

    TaskWidgetItem * item2;
    TaskWidgetItem * item3;

    for (int i = 0; i < count(); i++) {

        item2 = static_cast<TaskWidgetItem *>(itemAt(i));

        if (item2->operator<<(item)) {

            for (int j = i; j < lastIndex(item2); j++) {

                item3 = static_cast<TaskWidgetItem *>(itemAt(j));

                item3->hide();

                m_list.push_back(item3);

                QGraphicsLinearLayout::removeItem(item3);

                j--;

            }

            break;

        }

    }
    
    QGraphicsLinearLayout::removeItem(item);

    addItem(item);

}


QSizeF TaskLayout::sizeHint(Qt::SizeHint which, const QSizeF & constraint) const
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

void TaskLayout::clear()
{

    TaskWidgetItem * item;

    while (count() > 0) {

        item = static_cast<TaskWidgetItem *>(itemAt(0));

        QGraphicsLinearLayout::removeItem(item);

        item->deleteLater();

    }

    while (!m_list.isEmpty()) {

        delete m_list.at(0);

    }

}

QList<TaskWidgetItem*> TaskLayout::updateCompletedTasks()
{
    QList<TaskWidgetItem*> returnedList;
    QList<QString> list;
    TaskWidgetItem * item;
    
    for (int i = count()-1; i >= 0; i--) {
	
	item = static_cast<TaskWidgetItem *>(itemAt(i));
	
	if (item->isCompleted()) {
	 
	    if (!list.contains(item->uid())) {
		
		returnedList.push_back(item);
		
	    } else {
		
		list.push_back(item->relatedTo());
	    }
	    
	} else {
	 
	    list.push_back(item->relatedTo());
	    
	}
    }
    
    return returnedList;
}

TaskLayout::~TaskLayout()
{

}
