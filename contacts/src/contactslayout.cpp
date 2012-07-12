/*
    Akonadi google contact plasmoid - contactlayout.cpp
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



#include "contactslayout.h"

#include <KDebug>

ContactsLayout::ContactsLayout(Qt::Orientation orientation, QGraphicsLayoutItem * parent):
    QGraphicsLinearLayout(orientation, parent)
{
    setMinimumWidth(50);
}

void ContactsLayout::addItem(ContactWidgetItem * item)
{
    ContactWidgetItem * tmpItem;

    for (int i = 0; i < count(); i++) {
        tmpItem = static_cast<ContactWidgetItem *>(itemAt(i));

        if (item->operator<(tmpItem)) {
            insertItem(i, item);
            return;
        }
    }

    item->show();

    QGraphicsLinearLayout::addItem(item);
}

void ContactsLayout::clear()
{
    ContactWidgetItem * item;

    while (count() > 0) {
        item = static_cast<ContactWidgetItem *>(itemAt(0));
        removeItem(item);
        item->deleteLater();
    }
}

QSizeF ContactsLayout::sizeHint(Qt::SizeHint which, const QSizeF & constraint) const
{
    QSizeF hint;
    int cnt = count();

    if (cnt == 0)
        hint.setHeight(0);
    else
        hint = QGraphicsLinearLayout::sizeHint(which, constraint);

    return hint;
}
